#include <cpprest/filestream.h>
#include <cpprest/http_client.h>

#include <cxxabi.h>

using web::uri_builder;

using web::http::http_response;
using web::http::methods;
using web::http::client::http_client;

using concurrency::streams::fstream;
using concurrency::streams::ostream;

int main(int argc, char* argv[]) {
  const auto output_filename = "results.html";
  auto fileStream = std::make_shared<concurrency::streams::ostream>();

  // Open stream to output file.
  pplx::task<void> requestTask =
      fstream::open_ostream(output_filename)
          .then([=](ostream outFile) {
            *fileStream = outFile;

            // Create http_client to send the request.
            http_client client(U("http://www.bing.com/"));

            // Build request URI and start the request.
            uri_builder builder(U("/search"));
            builder.append_query(U("q"), U("cpprestsdk github"));
            return client.request(methods::GET, builder.to_string());
          })

          // Handle response headers arriving.
          .then([=](http_response response) {
            printf("Received response status code:%u\n", response.status_code());

            // Write response body into the file.
            return response.body().read_to_end(fileStream->streambuf());
          })

          // Close the file stream.
          .then([=](size_t) {
            printf("Search result is saved in:  %s\n", output_filename);
            return fileStream->close();
          });

  // Wait for all the outstanding I/O to complete and handle any exceptions
  try {
    requestTask.wait();
  } catch (const std::exception& e) {
    printf("Error exception:%s\n", e.what());
  }

  return 0;
}
