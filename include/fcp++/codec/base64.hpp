#ifndef FPC_CODEC_BASE64_HPP
#define FPC_CODEC_BASE64_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <string_view>

namespace fcp::codec::base64 {

std::array<char, 256> constexpr alphabet = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '~', '-'
};

template<class InputIterator>
inline std::string
encode(InputIterator begin, InputIterator end)
{
  const int input_size = end - begin;
  const int output_size = (input_size / 3 + (input_size % 3 > 0)) << 2;

  std::string result(output_size, '_');

  int j = 0;
  for (int i = 0; i < input_size;) {
    int val = (begin[i++] & 0xFF) << 16;
    if (i < input_size) {
      val |= (begin[i++] & 0xFF) << 8;
    }
    if (i < input_size) {
      val |= (begin[i++] & 0xFF);
    }
    result[j++] = alphabet[(val >> 18) & 0x3F];
    result[j++] = alphabet[(val >> 12) & 0x3F];
    result[j++] = alphabet[(val >> 6) & 0x3F];
    result[j++] = alphabet[val & 0x3F];
  }
  return result;
}

inline std::string
encode(std::string_view data)
{
  return encode(std::begin(data), std::end(data));
}

template<class Output>
inline Output
decode(std::string_view data)
{
  if (data.empty()) {
    return Output();
  }

  return Output();
}

inline std::string
decode(std::string_view data)
{
  return decode<std::string>(data);
}

}

#endif // !FCP_CODEC_BASE64_HPP