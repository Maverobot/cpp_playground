#include <boost/process.hpp>

#include <iostream>
#include <string>

using namespace boost::process;

int main() {
  ipstream pipe_stream;
  child c("gcc --version", std_out > pipe_stream);

  std::string line;

  while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    std::cerr << line << std::endl;

  // Without this line, the program will exit before the child process
  // has finished executing. The exit code could be 127.
  c.wait();
  return c.exit_code();
}
