#include <mlpack/core.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <mlpack/methods/ann/rnn.hpp>

arma::vec addNoise(const arma::vec &sequence, const double noise = 0.3) {
  return arma::randu(sequence.size()) * noise + sequence +
         arma::as_scalar(arma::randu(1) - 0.5) * noise;
}

void GenerateNoisySines(arma::cube &data, arma::mat &labels,
                        const size_t num_points, const size_t num_sequences,
                        const double noise = 0.3) {
  arma::colvec x = arma::linspace<arma::colvec>(0, num_points - 1, num_points) /
                   num_points * 20.0;
  arma::colvec y1 = arma::sin(x + arma::as_scalar(arma::randu(1)) * 3.0);
  arma::colvec y2 = arma::sin(x / 2.0 + arma::as_scalar(arma::randu(1)) * 3.0);

  data = arma::zeros(1 /* single dimension */, num_sequences * 2, num_points);
  labels = arma::zeros(2 /* 2 classes */, num_sequences * 2);

  for (size_t seq = 0; seq < num_sequences; seq++) {
    // Adds noise to y1
    arma::vec sequence = addNoise(y1);
    for (size_t i = 0; i < num_points; ++i) {
      data(0, seq, i) = sequence[i];
    }
    labels(0, seq) = 1;

    // Adds noise to y2
    sequence = addNoise(y2);
    for (size_t i = 0; i < num_points; ++i) {
      data(0, num_sequences + seq, i) = sequence[i];
    }
    labels(1, num_sequences + seq) = 1;
  }
  std::cout << data << std::endl;
  std::cout << labels << std::endl;
}

using namespace mlpack::ann;
using namespace mlpack::optimization;
using namespace mlpack;

int main(int argc, char *argv[]) {
  size_t successes = 0;
  const size_t rho = 3;

  arma::cube input;
  arma::mat labelsTemp;
  GenerateNoisySines(input, labelsTemp, rho, 6);

  arma::cube labels = arma::zeros<arma::cube>(1, labelsTemp.n_cols, rho);
  for (size_t i = 0; i < labelsTemp.n_cols; ++i) {
    const int value =
        arma::as_scalar(
            arma::find(arma::max(labelsTemp.col(i)) == labelsTemp.col(i), 1)) +
        1;
    labels.tube(0, i).fill(value);
  }

  /**
   * Construct a network with 1 input unit, 4 hidden units and 10 output
   * units. The hidden layer is connected to itself. The network structure
   * looks like:
   *
   *  Input         Hidden        Output
   * Layer(1)      Layer(4)      Layer(10)
   * +-----+       +-----+       +-----+
   * |     |       |     |       |     |
   * |     +------>|     +------>|     |
   * |     |    ..>|     |       |     |
   * +-----+    .  +--+--+       +-----+
   *            .     .
   *            .     .
   *            .......
   */
  Add<> add(4);
  Linear<> lookup(1, 4);
  SigmoidLayer<> sigmoidLayer;
  Linear<> linear(4, 4);
  Recurrent<> *recurrent =
      new Recurrent<>(add, lookup, linear, sigmoidLayer, rho);

  RNN<> model(rho);
  model.Add<IdentityLayer<>>();
  model.Add(recurrent);
  model.Add<Linear<>>(4, 10);
  model.Add<LogSoftMax<>>();

  StandardSGD opt(0.1, 1, 500 * input.n_cols, -100);
  model.Train(input, labels, opt);

  arma::cube prediction;
  model.Predict(input, prediction);

  size_t error = 0;
  for (size_t i = 0; i < prediction.n_cols; ++i) {
    const int predictionValue = arma::as_scalar(
        arma::find(arma::max(prediction.slice(rho - 1).col(i)) ==
                       prediction.slice(rho - 1).col(i),
                   1) +
        1);

    const int targetValue =
        arma::as_scalar(
            arma::find(arma::max(labelsTemp.col(i)) == labelsTemp.col(i), 1)) +
        1;

    if (predictionValue == targetValue) {
      error++;
    }
  }

  double classificationError = 1 - double(error) / prediction.n_cols;
  std::cout << "classification error: " << classificationError << std::endl;
}
