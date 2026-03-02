#include <iostream>
#include <streambuf>

class GbaFilteringStreamBuf final : public std::streambuf {
protected:
  int_type overflow(int_type c) override;
  std::streamsize xsputn(const char * s, std::streamsize n) override;
  int sync() override;
};
