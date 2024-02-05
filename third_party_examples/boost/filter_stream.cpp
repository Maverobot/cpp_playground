#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <fstream>
#include <iostream>

namespace io = boost::iostreams;

int main() {
  std::ifstream ifs("Makefile");

  io::filtering_stream<io::output> os;
  os.push(io::gzip_compressor());
  os.push(io::bzip2_compressor());

  std::ofstream ofs("Makefile.gz.bz2");
  os.push(ofs);

  io::copy(ifs, os);
}
