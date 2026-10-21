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

#ifdef __APPLE__

#include <cstring>

#include <application.hpp>
#include <unistd.h>

#include "terminal_screen.hpp"

namespace {

toy::assertion::Action reportAssertion(const char * assertionString) noexcept {
  terminal::leave();

  const ssize_t written = write(STDERR_FILENO, assertionString, std::strlen(assertionString));
  static_cast<void>(written);

  return toy::assertion::Action::Break;
}

class TerminalGuard final {
public:
  TerminalGuard() noexcept = default;

  TerminalGuard(const TerminalGuard &)             = delete;
  TerminalGuard & operator=(const TerminalGuard &) = delete;
  TerminalGuard(TerminalGuard &&)                  = delete;
  TerminalGuard & operator=(TerminalGuard &&)      = delete;

  ~TerminalGuard() noexcept {
    terminal::leave();
  }
};

void reportEnterFailure(terminal::EnterResult enterResult) noexcept {
  switch (enterResult) {
    case terminal::EnterResult::ok:
      break;
    case terminal::EnterResult::notATerminal:
      toy::application::ConsoleApplication::writeErrorLine(
        toy::StringView{"Tetris sample needs an interactive terminal"}
      );
      break;
    case terminal::EnterResult::tooSmall:
      toy::application::ConsoleApplication::writeErrorLine(
        toy::StringView{"Terminal must fit between 48x24 and 160x60"}
      );
      break;
    case terminal::EnterResult::setupFailed:
      toy::application::ConsoleApplication::writeErrorLine(
        toy::StringView{"Failed to switch the terminal to raw mode"}
      );
      break;
  }
}

} // namespace

class TetrisSampleApp final : public toy::application::ConsoleApplication {
public:
  constexpr explicit TetrisSampleApp(const toy::application::Version & version) noexcept
    : ConsoleApplication(version) {}

private:
  bool runInternal() noexcept override;
};

bool TetrisSampleApp::runInternal() noexcept {
  toy::assertion::initialize();
  toy::assertion::setCallbacks(reportAssertion, nullptr);

  const terminal::EnterResult enterResult = terminal::enter();
  if (enterResult != terminal::EnterResult::ok) {
    reportEnterFailure(enterResult);
    toy::assertion::deInitialize();
    return false;
  }

  const TerminalGuard terminalGuard{};

  char          byte{};
  const ssize_t bytesRead = read(STDIN_FILENO, &byte, 1);
  static_cast<void>(bytesRead);

  toy::assertion::deInitialize();
  return true;
}

IMPLEMENT_CONSOLE_APP(TetrisSampleApp)

#endif // __APPLE__
