//
// Copyright (c) 2025 by Toyman Interactive
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
  \brief  Logging level enumeration for categorized message filtering.
*/

#ifndef INCLUDE_CORE_LOG_LEVEL_HPP_
#define INCLUDE_CORE_LOG_LEVEL_HPP_

namespace toy::log {

/*!
  \enum level
  \brief Logging level enumeration for categorized message filtering.

  This enum class provides a hierarchical set of logging levels that can be used to control the verbosity and importance
  of log messages. Lower numeric values represent more detailed/verbose logging, while higher values represent more
  critical messages.

  The levels are designed to allow filtering: messages at a level and above (higher or equal numeric value) can be
  enabled or disabled together. Special levels \c ALL and \c OFF provide convenient control points for enabling or
  disabling all logging.

  \note Level values are intentionally spaced to allow future intermediate levels to be added.
*/
enum class level : std::uint8_t {
  ALL = 0, //!< Special level to enable logging for all levels. Used for filter configuration.

  FINEST = 30, //!< Most detailed tracing level. Use for highly verbose diagnostic information.

  FINER = 40, //!< Fairly detailed tracing level. Use for detailed diagnostic information.

  FINE = 50, //!< Tracing level. Use for general diagnostic information and flow tracking.

  CONFIG = 70, //!< Configuration level. Use for static configuration and setup messages.

  INFO = 80, //!< Informational level. Use for general informational messages about normal operation.

  WARNING = 90, //!< Warning level. Use for potential problems or unexpected conditions that don't stop execution.

  SEVERE = 100, //!< Severe level. Use for serious failure messages that may indicate errors requiring attention.

  SHOUT = 120, //!< Extra debugging level. Use for critical debugging messages that need maximum visibility.

  OFF = 200, //!< Special level to disable logging for all levels. Used for filter configuration.
};

} // namespace toy::log

#endif // INCLUDE_CORE_LOG_LEVEL_HPP_
