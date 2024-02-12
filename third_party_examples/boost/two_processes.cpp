#include <boost/process.hpp>

#include <iostream>
#include <string>

namespace bp = boost::process;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " 'command with its arguments'  \n";
    return 1;
  }

  bp::pipe p;
  bp::child nm(argv[1], bp::std_out > p);
  bp::child filt("bzip2 -c -8", bp::std_in<p, bp::std_out> "output.bz2");

  nm.wait();
  filt.wait();
  return 0;
}
