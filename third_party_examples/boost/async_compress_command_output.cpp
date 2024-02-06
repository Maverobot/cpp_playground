/**
 * This program takes a command as input and runs it asynchronously. It reads the output of the
 * command and compresses it in chunks of 1000 lines. The compressed output is written to a file
 * with the name `outputN.gz`, where `N` is the file count.
 */

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/process.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

namespace io = boost::iostreams;
namespace bp = boost::process;

void compress_output(std::istringstream& is, size_t file_count) {
  io::filtering_stream<io::output> os;
  os.push(io::gzip_compressor());
  std::ofstream ofs("output" + std::to_string(file_count) + ".gz");
  os.push(ofs);

  io::copy(is, os);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <command> [argument1] [argument2] [...] \n";
    return 1;
  }

  std::string command;
  for (int i = 1; i < argc; ++i) {
    command += argv[i];
    if (i < argc - 1) {
      command += " ";
    }
  }

  // TODO: Add std_err > is to capture error output
  bp::ipstream stdout_is;
  bp::child c(command, bp::std_out > stdout_is);
  std::string line;
  std::ostringstream oss;
  size_t file_count = 0;

  size_t line_count = 0;
  while (std::getline(stdout_is, line)) {
    oss << line << '\n';
    if (++line_count == 1000) {
      std::istringstream is_chunk(oss.str());
      compress_output(is_chunk, file_count++);
      oss.str("");  // Clear the string stream
      line_count = 0;
    }
  }

  // Compress remaining lines if any
  if (line_count > 0) {
    std::istringstream is_chunk(oss.str());
    compress_output(is_chunk, file_count++);
  }

  return 0;
}
