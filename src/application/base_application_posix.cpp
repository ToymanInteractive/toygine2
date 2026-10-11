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
  \file   base_application_posix.cpp
  \brief  Implementations of toy::application::BaseApplication::pid(), sleep(), and yield() for Linux and macOS.
*/

#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "application.hpp"

namespace toy::application {

uint32_t BaseApplication::pid() noexcept {
  static_assert(sizeof(uint32_t) >= sizeof(pid_t), "uint32_t must hold a POSIX process ID");

  return bit_cast<uint32_t>(::getpid());
}

void BaseApplication::sleep(size_t milliseconds) noexcept {
  timespec ts = {.tv_sec  = static_cast<time_t>(milliseconds / 1000),
                 .tv_nsec = static_cast<long>((milliseconds % 1000) * 1000000)};

  while (::nanosleep(&ts, &ts) == -1) {
    if (errno != EINTR) {
      // Only EINTR is reachable: the timespec is always valid.
      break;
    }
  }
}

void BaseApplication::yield() noexcept {
  ::sched_yield();
}

} // namespace toy::application
