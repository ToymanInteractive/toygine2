#include "gba_filtering_stream_buf.hpp"

std::streambuf::int_type GbaFilteringStreamBuf::overflow(int_type c) {
  if (c != traits_type::eof()) {
    char ch = traits_type::to_char_type(c);
    std::cout.put(ch);
  }

  return c;
}

std::streamsize GbaFilteringStreamBuf::xsputn(const char * s, std::streamsize n) {
  std::cout.write(s, n);

  return n;
}

int GbaFilteringStreamBuf::sync() {
  std::cout.flush();

  return 0;
}
