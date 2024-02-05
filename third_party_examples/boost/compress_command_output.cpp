#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/process.hpp>
#include <fstream>
#include <iostream>

namespace io = boost::iostreams;
namespace bp = boost::process;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <command> [argument1] [argument2] [...] \n";
    return 1;
  }

  // Concatenate the argv[1:] to a single string
  std::string command;
  for (int i = 1; i < argc; ++i) {
    command += argv[i];
    if (i < argc - 1) {
      command += " ";
    }
  }

  bp::ipstream is;
  bp::child c(command, bp::std_out > is);

  io::filtering_stream<io::output> os;
  os.push(io::gzip_compressor());
  std::ofstream ofs("output.gz");
  os.push(ofs);

  io::copy(is, os);
}
