#pragma once

#include <cstdint>

enum MessageType { MSG_START = 0, MSG_STOP, MSG_DATA };

struct FileData {
  uint64_t seq;
  double timestamp;
};

struct Data {
  MessageType message_type;
  union {
    struct {
      uint16_t version;
      char identifier[15];
    } connect;

    struct {
      std::size_t data_size;
      FileData data;
    } payload;
  };
} __attribute__((packed));
