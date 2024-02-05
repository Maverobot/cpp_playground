#include <boost/process.hpp>

#include <iostream>
#include <string>

using namespace boost::process;

int main() {
  boost::process::pipe pipe_stream;
  child c(search_path("bash"),
          std::vector<std::string>{
              "-c", "count=0; while true; do count=$((count+1)); echo $count; sleep 1; done"},
          std_out > pipe_stream);

  // Pipe the output of the child process to the `wc` command
  // to count the number of lines
  child wc("wc", std_in<pipe_stream, std_out> "stdout");

  // Without this line, the program will exit before the child process
  // has finished executing. The exit code could be 127.
  c.wait();
  return c.exit_code();
}
