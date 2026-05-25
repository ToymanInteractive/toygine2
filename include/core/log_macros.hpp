//
// Copyright (c) 2025-2026 Toyman Interactive
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
  \file   log_macros.hpp
  \brief  Call-site macros that build \ref toy::log::Metadata and dispatch through \ref toy::log::Backend.

  Defines \c LOG_TRACE, \c LOG_DEBUG, \c LOG_INFO, \c LOG_WARN, and \c LOG_ERROR. Each macro instantiates a \c static
  \c constexpr \ref toy::log::Metadata at the call site, gates the call on \c LOG_MAX_LEVEL via \c if \c constexpr, and
  forwards the format pattern and arguments to \ref toy::log::Backend::push().

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_LOG_MACROS_HPP_
#define INCLUDE_CORE_LOG_MACROS_HPP_

/*!
  \def TOY_LOG_AT

  \brief Internal helper that expands a single \c LOG_* call.

  Instantiates a \c static \c constexpr \ref toy::log::Metadata when \a level passes the \c LOG_MAX_LEVEL gate, then
  forwards \a fmt and \c __VA_ARGS__ to \ref toy::log::Backend::push(). On retro targets without dead-code elimination
  the discarded \c if \c constexpr branch may still emit the metadata; raise \c LOG_MAX_LEVEL or enable optimizations to
  avoid it.

  \sa LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_MAX_LEVEL
*/
#define TOY_LOG_AT(level, fmt, ...)                                                                                    \
  do {                                                                                                                 \
    if constexpr (static_cast<int>(level) <= LOG_MAX_LEVEL) {                                                          \
      static_assert(std::is_array_v<std::remove_reference_t<decltype(fmt)>>, "LOG_*: fmt must be a string literal");   \
      static constexpr ::toy::log::Metadata _toy_log_meta{(fmt), __FILE__, __LINE__, (level)};                         \
      ::toy::log::Backend::instance().push(&_toy_log_meta __VA_OPT__(, ) __VA_ARGS__);                                 \
    }                                                                                                                  \
  } while (false)

/*!
  \def LOG_TRACE

  \brief Emits a \ref toy::log::Level::Trace record through the active \ref toy::log::Backend.

  Compiled out when \c LOG_MAX_LEVEL < \c 50.

  \sa LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_MAX_LEVEL, \ref toy::log::Backend
*/
#define LOG_TRACE(fmt, ...) TOY_LOG_AT(::toy::log::Level::Trace, fmt, __VA_ARGS__)

/*!
  \def LOG_DEBUG

  \brief Emits a \ref toy::log::Level::Debug record through the active \ref toy::log::Backend.

  Compiled out when \c LOG_MAX_LEVEL < \c 40.

  \sa LOG_TRACE, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_MAX_LEVEL, \ref toy::log::Backend
*/
#define LOG_DEBUG(fmt, ...) TOY_LOG_AT(::toy::log::Level::Debug, fmt, __VA_ARGS__)

/*!
  \def LOG_INFO

  \brief Emits a \ref toy::log::Level::Info record through the active \ref toy::log::Backend.

  Compiled out when \c LOG_MAX_LEVEL < \c 30.

  \sa LOG_TRACE, LOG_DEBUG, LOG_WARN, LOG_ERROR, LOG_MAX_LEVEL, \ref toy::log::Backend
*/
#define LOG_INFO(fmt, ...) TOY_LOG_AT(::toy::log::Level::Info, fmt, __VA_ARGS__)

/*!
  \def LOG_WARN

  \brief Emits a \ref toy::log::Level::Warn record through the active \ref toy::log::Backend.

  Compiled out when \c LOG_MAX_LEVEL < \c 20.

  \sa LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_ERROR, LOG_MAX_LEVEL, \ref toy::log::Backend
*/
#define LOG_WARN(fmt, ...) TOY_LOG_AT(::toy::log::Level::Warn, fmt, __VA_ARGS__)

/*!
  \def LOG_ERROR

  \brief Emits a \ref toy::log::Level::Error record through the active \ref toy::log::Backend.

  Compiled out when \c LOG_MAX_LEVEL < \c 10.

  \sa LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_MAX_LEVEL, \ref toy::log::Backend
*/
#define LOG_ERROR(fmt, ...) TOY_LOG_AT(::toy::log::Level::Error, fmt, __VA_ARGS__)

#endif // INCLUDE_CORE_LOG_MACROS_HPP_
