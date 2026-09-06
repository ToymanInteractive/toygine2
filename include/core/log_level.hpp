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
  \file   log_level.hpp
  \brief  Severity scale of the logging module and the macro that trims it at build time.

  Defines \ref toy::log::Level, the scale every log call carries, and LOG_MAX_LEVEL, the number a call's level must not
  exceed to reach the binary.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_LEVEL_HPP_
#define INCLUDE_CORE_LOG_LEVEL_HPP_

namespace toy::log {

/*!
  \enum  Level
  \brief Severity of a log message, ordered by verbosity.

  \ingroup Logger

  \ref toy::log::Level::Off admits nothing, \ref toy::log::Level::Trace admits everything, and the values between them
  grow with detail. A message passes a ceiling when the number behind its level is at most that ceiling.

  Steps of ten leave room for a severity between two neighbors. The values are contract wherever a level is stored or
  transmitted: never renumbered, never reused.

  \note \ref toy::log::Level::Off is a ceiling value only; no message carries it as its own severity.

  \sa LOG_MAX_LEVEL
*/
enum class Level : int8_t {
  Off = 0,    ///< Nothing passes.

  Error = 10, ///< Failed operation; execution may continue.

  Warn = 20,  ///< Condition the engine recovered from.

  Info = 30,  ///< Milestone of normal operation: subsystem start, asset load.

  Debug = 40, ///< Coarse diagnostic detail, read while developing the engine.

  Trace = 50, ///< Finer detail than \ref toy::log::Level::Debug, down to a single call or frame.
};

} // namespace toy::log

#ifndef LOG_MAX_LEVEL
/*!
  \def   LOG_MAX_LEVEL
  \brief Number of the most verbose \ref toy::log::Level a build keeps.

  \ingroup Logger

  A call reaches the binary when its level is at most this value; anything more verbose compiles out. Define it ahead of
  core.hpp, on the compiler command line or in the target's CMake definitions, to cut the verbose end of the scale on a
  constrained target. It defaults to \c 50, the value of \ref toy::log::Level::Trace, which keeps every level.

  \note A number rather than an enumerator, so a build system sets it without \ref toy::log::Level in scope.
  \note Fixed at compile time; nothing raises the ceiling at runtime.
*/
#define LOG_MAX_LEVEL 50
#endif // LOG_MAX_LEVEL

#endif // INCLUDE_CORE_LOG_LEVEL_HPP_
