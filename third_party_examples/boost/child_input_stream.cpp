#include <boost/process.hpp>
#include <iostream>

namespace bp = boost::process;

int main(int /* argc */, char** /* argv */) {
  boost::process::opstream in{};
  boost::process::child child("bzip2 -c -8", bp::std_in<in, bp::std_out> "output.bz2");

  in << "test1\n";
  in << "test2\n";
  in << "test3\n";
  in << std::flush;

  std::cerr << "Closing the stream…\n";
  in.close();
  in.pipe().close();  // This line is necessary to avoid getting
  std::cerr << "Waiting for the child to exit…\n";
  child.wait();  // Parent seems to hang here.

  return 0;
}
