#pragma once
#include <cstdint>

enum class Channel : uint8_t {
  None = 0,
  Blue = 1 << 0,
  Green = 1 << 1,
  Red = 1 << 2,
  All = Blue | Green | Red,
};

constexpr Channel operator|(Channel a, Channel b) {
  return static_cast<Channel>(static_cast<uint8_t>(a) |
                              static_cast<uint8_t>(b));
}

constexpr Channel operator&(Channel a, Channel b) {
  return static_cast<Channel>(static_cast<uint8_t>(a) &
                              static_cast<uint8_t>(b));
}

constexpr Channel operator~(Channel a) {
  return static_cast<Channel>(~static_cast<uint8_t>(a) &
                              static_cast<uint8_t>(Channel::All));
}

constexpr Channel &operator|=(Channel &a, Channel b) { return a = a | b; }

constexpr Channel &operator&=(Channel &a, Channel b) { return a = a & b; }

constexpr bool hasFlag(Channel value, Channel flag) {
  return (value & flag) == flag;
}

constexpr Channel channelAt(int index) {
  return static_cast<Channel>(1 << index);
}
