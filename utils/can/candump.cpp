#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <can_interface_name>" << std::endl;
    return 1;
  }

  const std::string can_interface_name = argv[1];

  int s;
  sockaddr_can addr;
  ifreq ifr;
  can_frame frame;

  // Open socket
  s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
  if (s < 0) {
    perror("Socket");
    return 1;
  }

  // Specify can0 device
  strcpy(ifr.ifr_name, can_interface_name.c_str());
  ioctl(s, SIOCGIFINDEX, &ifr);

  // Bind the socket to can0
  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;
  if (bind(s, (sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("Bind");
    return 1;
  }

  std::cout << "Listening on can0..." << std::endl;

  // Receive loop
  while (true) {
    int nbytes = read(s, &frame, sizeof(can_frame));

    if (nbytes < 0) {
      perror("Read");
      return 1;
    }

    printf("CAN ID: 0x%03X DLC: %d Data:", frame.can_id,
           frame.can_dlc);  // DLC stands for Data Length Code
    for (int i = 0; i < frame.can_dlc; i++) {
      printf(" %02X", frame.data[i]);
    }
    printf("\n");
  }

  close(s);
  return 0;
}
