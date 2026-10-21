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

#include <csignal>
#include <cstddef>

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "terminal_screen.hpp"

namespace terminal {
namespace {

termios savedTermios{};
Size    originalSize{0, 0};
bool    insideAlternateScreen = false;

constexpr char c_enterSequence[] = "\x1b[?1049h\x1b[?25l";
constexpr char c_leaveSequence[] = "\x1b[0m\x1b[?25h\x1b[?1049l";

void writeStdout(const char * text, size_t length) noexcept {
  const ssize_t written = write(STDOUT_FILENO, text, length);
  static_cast<void>(written);
}

void restoreTerminalAndReraise(int signalNumber) noexcept {
  leave();

  struct sigaction defaultAction{};
  defaultAction.sa_handler = SIG_DFL;
  const int restored       = sigaction(signalNumber, &defaultAction, nullptr);
  static_cast<void>(restored);

  std::raise(signalNumber);
}

bool installTerminationHandlers() noexcept {
  struct sigaction action{};
  action.sa_handler = restoreTerminalAndReraise;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  return sigaction(SIGINT, &action, nullptr) == 0 && sigaction(SIGTERM, &action, nullptr) == 0
         && sigaction(SIGHUP, &action, nullptr) == 0;
}

} // namespace

Size querySize() noexcept {
  winsize windowSize{};
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowSize) != 0)
    return {0, 0};

  return {windowSize.ws_col, windowSize.ws_row};
}

EnterResult enter() noexcept {
  if (isatty(STDIN_FILENO) == 0 || isatty(STDOUT_FILENO) == 0)
    return EnterResult::notATerminal;

  const Size size = querySize();
  if (size.columns < c_minColumns || size.rows < c_minRows || size.columns > c_maxColumns || size.rows > c_maxRows)
    return EnterResult::tooSmall;

  if (tcgetattr(STDIN_FILENO, &savedTermios) != 0)
    return EnterResult::setupFailed;

  if (!installTerminationHandlers())
    return EnterResult::setupFailed;

  termios rawMode = savedTermios;
  cfmakeraw(&rawMode);
  if (tcsetattr(STDIN_FILENO, TCSANOW, &rawMode) != 0)
    return EnterResult::setupFailed;

  originalSize = size;
  writeStdout(c_enterSequence, sizeof(c_enterSequence) - 1);
  insideAlternateScreen = true;
  // Полная перерисовка появится в итерации 3 вместе с буфером кадра; пока alternate screen входит пустым.
  return EnterResult::ok;
}

void leave() noexcept {
  if (!insideAlternateScreen)
    return;

  writeStdout(c_leaveSequence, sizeof(c_leaveSequence) - 1);

  const int restored = tcsetattr(STDIN_FILENO, TCSANOW, &savedTermios);
  static_cast<void>(restored);
  insideAlternateScreen = false;
}

} // namespace terminal
