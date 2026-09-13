//
// Copyright (c) 2026 Toyman Interactive
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
/*!
  \file   line_buffer.test.cpp
  \brief  Unit tests for the line buffer the console benchmark report is built in.
*/

#include <cstddef>
#include <string>

#include <doctest/doctest.h>

#include "line_buffer.hpp"

namespace {

struct CapturedLines final {
  std::string text;
  std::size_t calls;
};

// The writer data is a pointer to a const pointer, so the capture stays mutable without a const_cast.
void captureLines(const char * text, std::size_t length, const void * writerData) noexcept {
  CapturedLines & captured = **static_cast<CapturedLines * const *>(writerData);

  captured.text.append(text, length);
  ++captured.calls;
}

} // namespace

// A flush hands the collected fields to the writer in one call that ends in a newline.
TEST_CASE("benchmark::detail/line_buffer/flush_writes_one_line") {
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  toy::benchmark::detail::LineBuffer line{
    {.write = &captureLines, .writerData = &capturedPointer}
  };

  line.addText("#csv core/fake/title,fast,search,");
  line.addDecimal(63695246);
  line.addText(",");
  line.addDecimal(7);
  line.flush();

  CHECK(captured.text == "#csv core/fake/title,fast,search,63695246,7\n");
  CHECK(captured.calls == 1);
}

// A field without delimiters is written as it stands; a comma, a quote or a tab puts it in quotes.
TEST_CASE("benchmark::detail/line_buffer/quotes_a_field_with_delimiters") {
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  toy::benchmark::detail::LineBuffer line{
    {.write = &captureLines, .writerData = &capturedPointer}
  };

  line.addField("toy::StringView");
  line.addText(",");
  line.addField("toy::FixedString<32, char>");
  line.addText(",");
  line.addField("say \"hi\"");
  line.flush();

  CHECK(captured.text == "toy::StringView,\"toy::FixedString<32, char>\",\"say \"\"hi\"\"\"\n");
}

// A line break inside a field becomes a space: the host reads one line at a time.
TEST_CASE("benchmark::detail/line_buffer/field_never_breaks_the_line") {
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;

  toy::benchmark::detail::LineBuffer line{
    {.write = &captureLines, .writerData = &capturedPointer}
  };

  line.addField("two\nlines\rback");
  line.flush();

  CHECK(captured.text == "two lines back\n");
  CHECK(captured.calls == 1);
}

// A line longer than the buffer is cut, and its last byte is still the newline.
TEST_CASE("benchmark::detail/line_buffer/cut_line_keeps_its_newline") {
  CapturedLines         captured{};
  CapturedLines * const capturedPointer = &captured;
  const std::string     longText(300, 'x');

  toy::benchmark::detail::LineBuffer line{
    {.write = &captureLines, .writerData = &capturedPointer}
  };

  line.addText(longText.c_str());
  line.flush();

  CHECK(captured.text.size() == toy::benchmark::detail::LineBuffer::c_capacity);
  CHECK(captured.text.back() == '\n');
  CHECK(captured.text.find_first_not_of('x') == toy::benchmark::detail::LineBuffer::c_capacity - 1);
}
