#include <algorithm>
#include <random>
#include <torch/torch.h>

std::tuple<std::vector<float>, std::vector<float>>
getTrainingData(double i_max = 1000000, double x_min = -M_PI,
                double x_max = M_PI) {
  std::vector<float> x(i_max);
  std::vector<float> y(i_max);

  size_t i = 0;
  while (i < i_max) {
    x[i] = x_min + (x_max - x_min) * i / i_max;
    y.at(i) = std::cos(x.at(i));
    i++;
  }

  return std::make_tuple(x, y);
};

class OneDimMappingDataset : public torch::data::Dataset<OneDimMappingDataset> {
private:
  torch::Tensor states_, labels_;

public:
  explicit OneDimMappingDataset(const std::string &loc_states,
                                const std::string &loc_labels)
      : states_(loc_states), labels_(loc_labels){};
  torch::data::Example<> get(size_t index) override {
    return {states_[index], labels_[index]};
  };
};

using namespace torch;

int main(int argc, char *argv[]) {

  const size_t data_size = train_x.size(0);
  const size_t epoch_size = 100;
  const size_t batch_size = 1;
  const int64_t batches_per_epoch =
      std::ceil(data_size / static_cast<double>(batch_size));
  const int64_t kLogInterval = 1000;
  const int64_t kCheckpointEvery = 10000;

  // Use GPU when present, CPU otherwise.
  torch::Device device(torch::kCPU);
  if (torch::cuda::is_available()) {
    device = torch::Device(torch::kCUDA);
    std::cout << "CUDA is available! Training on GPU." << std::endl;
  }

  // Get training data.
  std::vector<float> train_x_raw;
  std::vector<float> train_y_raw;
  std::tie(train_x_raw, train_y_raw) = getTrainingData();

  // Shuffle training data.
  auto rng = std::default_random_engine{};
  std::shuffle(train_x_raw.begin(), train_x_raw.end(), rng);
  std::shuffle(train_y_raw.begin(), train_y_raw.end(), rng);

  // Create tensors based on the training data.
  Backend b;
  torch::Tensor train_x =
      torch::from_blob(train_x_raw.data(), train_x_raw.size())
          .toBackend(c10::Backend::CUDA);
  torch::Tensor train_y =
      torch::from_blob(train_y_raw.data(), train_y_raw.size())
          .toBackend(c10::Backend::CUDA);
  std::cout << "train_x size: " << train_x.size(0) << std::endl;
  std::cout << "train_y size: " << train_y.size(0) << std::endl;

  // Generate a dataset
  auto data_set = OneDimMappingDataset(loc_states, loc_labels)
                      .map(torch::data::transforms::Stack<>());

  // Generate a data loader.
  auto data_loader =
      torch::data::make_data_loader<torch::data::samplers::SequentialSampler>(
          std::move(data_set), batch_size);

  // Define network
  nn::Sequential func_approximator(
      nn::Linear(nn::LinearOptions(1, 100).with_bias(true)),
      nn::Functional(torch::leaky_relu, 0.2),
      nn::Linear(nn::LinearOptions(100, 1).with_bias(true)),
      nn::Functional(torch::tanh));
  func_approximator->to(device);

  // Define Optimizer
  optim::Adam optimizer(func_approximator->parameters(),
                        optim::AdamOptions(2e-4).beta1(0.5));

  const bool kRestoreFromCheckpoint = false;
  if (kRestoreFromCheckpoint) {
    torch::load(func_approximator, "cos-func-approx-checkpoint.pt");
  }

  size_t epoch_idx = 0;
  size_t batch_index = 0;
  while (epoch_idx < epoch_size) {
    for (auto &batch : data_loader) {
      // Train discriminator with real images.
      func_approximator->zero_grad();
      auto data = batch.data;
      auto labels = batch.target;

      torch::Tensor real_output = func_approximator->forward(data);
      torch::Tensor d_loss_real = torch::mse_loss(real_output, labels);
      d_loss_real.backward();

      if (batch_index % kLogInterval == 0) {
        std::printf("\r[%2ld/%2ld][%3ld/%3ld] loss: %.4f \n", epoch_idx,
                    epoch_size, batch_index, batches_per_epoch,
                    d_loss_real.item<float>());
        auto test_x = torch::randn(1) * M_PI;
        auto test_y =
            func_approximator->forward(test_x.toBackend(c10::Backend::CUDA));
        std::printf("x = %.5f, target y = %.5f, predicted y = %.5f \n",
                    test_x[0].item<float>(), std::cos(test_x[0].item<float>()),
                    test_y[0].item<float>());
      }

      if (batch_index % kCheckpointEvery == 0) {
        // Checkpoint the model and optimizer state.
        torch::save(func_approximator, "cos-func-approx-checkpoint.pt");
      }

      batch_index++;
    }
    epoch_idx++;
  }

  return 0;
}
