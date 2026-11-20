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
  \file   console_sample_app.cpp
  \brief  Console sample: hello world and a chrono demo.

  The app prints the greeting and version, reads the wall clock from toy::chrono::SystemClock and the steady counter's
  frequency and tick count from toy::chrono::SteadyClock, and reports the output time its toy::chrono::Stopwatch
  measured.
*/

#include "toygine.hpp"

namespace {

// Left-pads a number with zeros to width; the clock fields are non-negative.
template <std::integral T>
toy::FixedString<24> zeroPadded(T value, std::size_t width) {
  toy::array<char, 24> buffer{};
  const char *         text;
  if constexpr (std::is_signed_v<T>)
    text = toy::string_utils::itoa(toy::span{buffer}, value);
  else
    text = toy::string_utils::utoa(toy::span{buffer}, value);

  toy::FixedString<24> result{};
  const auto           length = std::char_traits<char>::length(text);
  for (std::size_t index = length; index < width; ++index)
    result += '0';
  result += text;
  return result;
}

} // namespace

class ConsoleSampleApp final : public toy::application::ConsoleApplication {
public:
  constexpr explicit ConsoleSampleApp(const toy::application::Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override;
};

bool ConsoleSampleApp::runInternal() noexcept {
  const toy::chrono::Stopwatch watch;
  toy::array<char, 24>         buffer{};

  writeOutputLine(toy::StringView{"Hello world from Console sample app"});
  if constexpr (supportsUtf8()) {
    writeOutputLine(toy::StringView{"Привет, мир из консольного тестового приложения"});
    writeOutputLine(toy::StringView{"コンソールサンプルアプリからこんにちは世界"});
    writeOutputLine(toy::StringView{"来自控制台示例应用的世界你好"});
    writeOutputLine(toy::StringView{"콘솔 샘플 앱에서 안녕하세요 세계"});
    writeOutputLine(toy::StringView{"कंसोल नमूना ऐप से हैलो वर्ल्ड"});
    writeOutputLine(toy::StringView{"Γεια σου κόσμε από την εφαρμογή δείγματος κονσόλας"});
    writeOutputLine(toy::StringView{"🌍👋 Hello World! 🎮🕹️👾🐛"});
  }
  writeOutputLine(toy::StringView{""});

  toy::FixedString<64> versionLine{"App Version: "};
  versionLine += toy::string_utils::utoa(toy::span{buffer}, version().major);
  versionLine += '.';
  versionLine += toy::string_utils::utoa(toy::span{buffer}, version().minor);
  versionLine += '.';
  versionLine += toy::string_utils::utoa(toy::span{buffer}, version().patch);
  versionLine += " (";
  writeOutput(versionLine);
  writeOutput(toy::StringView{version().revision.c_str()});
  writeOutputLine(toy::StringView{")"});
  writeOutputLine(toy::StringView{""});

  if (argsCount() > 1) {
    writeOutputLine(toy::StringView{""});
    writeOutputLine(toy::StringView{"arguments:"});
    for (size_t index = 1; index < argsCount(); ++index)
      if (const char * value = arg(index); value != nullptr) {
        writeOutput(toy::StringView{"- "});
        writeOutputLine(toy::StringView{value});
      }
  }

  if (pid() != 0) {
    toy::FixedString<64> pidLine{"System pid: "};
    pidLine += toy::string_utils::utoa(toy::span{buffer}, pid());
    writeOutputLine(pidLine);
  }

  const auto            calendar = toy::chrono::SystemClock::now();
  toy::FixedString<160> clockLine{"System clock: "};
  clockLine += zeroPadded(calendar.year, 4);
  clockLine += '-';
  clockLine += zeroPadded(calendar.month, 2);
  clockLine += '-';
  clockLine += zeroPadded(calendar.day, 2);
  clockLine += ' ';
  clockLine += zeroPadded(calendar.hour, 2);
  clockLine += ':';
  clockLine += zeroPadded(calendar.minute, 2);
  clockLine += ':';
  clockLine += zeroPadded(calendar.second, 2);
  clockLine += '.';
  clockLine += zeroPadded(calendar.millisecond, 3);
  clockLine += " dow: ";
  clockLine += toy::string_utils::utoa(toy::span{buffer}, calendar.dayOfWeek);
  clockLine += " tz: utc";
  if (calendar.tzOffsetMinutes >= 0)
    clockLine += '+';
  clockLine += toy::string_utils::itoa(toy::span{buffer}, calendar.tzOffsetMinutes);
  clockLine += " min";
  writeOutputLine(clockLine);

  toy::FixedString<96> frequencyLine{"Steady clock frequency: "};
  frequencyLine += toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::frequency());
  frequencyLine += " Hz";
  writeOutputLine(frequencyLine);

  toy::FixedString<96> ticksLine{"Steady clock ticks: "};
  ticksLine += toy::string_utils::itoa(toy::span{buffer}, toy::chrono::SteadyClock::nowTicks());
  writeOutputLine(ticksLine);

  toy::FixedString<96> elapsedLine{"Steady clock elapsed: "};
  const auto           elapsedNs  = toy::chrono::duration_cast<toy::chrono::nanoseconds>(watch.elapsed()).count();
  elapsedLine                    += toy::string_utils::itoa(toy::span{buffer}, elapsedNs);
  elapsedLine                    += " ns";
  writeOutputLine(elapsedLine);

  writeOutputLine(toy::StringView{""});
  writeErrorLine(toy::StringView("!!!Error message demo:: this is not an error!!!"));
  writeOutputLine(toy::StringView{""});

  return true;
}

IMPLEMENT_CONSOLE_APP(ConsoleSampleApp)
