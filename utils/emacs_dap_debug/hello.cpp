/**
 * Sample code to try out debugging with dap layer in spacemacs.
 *
 * + Build project: `mkdir build -p && cd build && cmake .. && cmake --build .`
 * + Add breakpoint: `SPC-d-b-a`
 * + Remove breakpoint: `SPC-d-b-d`
 * + Start debugging: `SPC-d-d-d` -> Select "gdb: hello"
 * + Show dap-hydra: `SPC-d-.`
 */

int main(int argc, char* argv[]) {
  int i = 0;
  for (; i < 10;) {
    i++;
  }
  return i;
}
