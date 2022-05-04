#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;        // from <boost/asio/ip/tcp.hpp>

class Websocket {
 public:
  Websocket(std::string host, std::string port) {
    // Look up the domain name
    auto const results = resolver_.resolve(host, port);

    // Make the connection on the IP address we get from a lookup
    auto ep = net::connect(ws_.next_layer(), results);

    // Update the host_ string. This will provide the value of the
    // Host HTTP header during the WebSocket handshake.
    // See https://tools.ietf.org/html/rfc7230#section-5.4
    host += ':' + std::to_string(ep.port());

    // Set a decorator to change the User-Agent of the handshake
    ws_.set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
      req.set(http::field::user_agent,
              std::string(BOOST_BEAST_VERSION_STRING) + " websocket-client-coro");
    }));

    // Perform the websocket handshake
    ws_.handshake(host, "/");

    is_running_ = true;
    send_thread_ = std::make_unique<std::thread>(&Websocket::doSend, this);
  };

  ~Websocket() {
    is_running_ = false;
    send_thread_->join();
    ws_.close(websocket::close_code::normal);
  }

  void send(std::string data) {
    if (!data_queue_.push(data)) {
      std::cerr << "Failed to push the data. The queue is likely full\n";
    }
  }

  void doSend() {
    while (is_running_) {
      std::string data;
      if (data_queue_.pop(data)) {
        try {
          ws_.write(net::buffer(data));
        } catch (std::exception const& e) {
          std::cerr << "Error: " << e.what() << std::endl;
        }
      }
      std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
  }

 private:
  std::atomic<bool> is_running_{false};
  net::io_context ioc_;
  tcp::resolver resolver_{ioc_};
  websocket::stream<tcp::socket> ws_{ioc_};
  boost::lockfree::spsc_queue<std::string> data_queue_{100};

  std::unique_ptr<std::thread> send_thread_;
};

// Sends a WebSocket message and prints the response
int main(int argc, char** argv) {
  try {
    // Check command line arguments.
    if (argc != 4) {
      std::cerr << "Usage: websocket-client-sync <host> <port> <text>\n"
                << "Example:\n"
                << "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
      return EXIT_FAILURE;
    }
    std::string host = argv[1];
    auto const port = argv[2];
    auto const text = argv[3];

    Websocket ws(host, port);
    ws.send(text);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cerr << "Finished.\n";
  } catch (std::exception const& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
