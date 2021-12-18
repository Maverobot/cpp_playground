#include <iostream>

#include <jsonbuilder/JsonRenderer.h>
#include <lttng-consume/LttngConsumer.h>

int main(int /* argc */, char* /* argv */[]) {
  LttngConsume::LttngConsumer lttng_consumer("net://localhost/host/PowerLinuxFairy/my-session",
                                             std::chrono::milliseconds(10));

  auto callback = [](jsonbuilder::JsonBuilder&& builder) {
    jsonbuilder::JsonRenderer renderer;
    renderer.Pretty(true);

    std::string_view jsonString = renderer.Render(builder);
    std::cout << jsonString << std::endl;
  };

  lttng_consumer.StartConsuming(callback);
  return 0;
}
