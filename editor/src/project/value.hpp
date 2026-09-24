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
  \file   value.hpp
  \brief  Value kinds a setting can hold, and the storage for one value.

  Defines \ref toy::editor::project::Type and \ref toy::editor::project::Value. The kind a setting declares decides
  which alternative of the variant holds its value.
*/
#ifndef EDITOR_SRC_PROJECT_VALUE_HPP_
#define EDITOR_SRC_PROJECT_VALUE_HPP_

#include <cstdint>
#include <string>
#include <variant>

namespace toy::editor::project {

/*!
  \enum   Type
  \brief  Kind of value a setting holds.
*/
enum class Type {
  String,      ///< UTF-8 text
  Integer,     ///< Signed 64-bit integer
  Double,      ///< Double-precision floating point
  Boolean,     ///< \c true or \c false
  Enumeration, ///< Enumerator name held as text; nothing checks it against a set of allowed names
};

/*!
  \brief  Storage for one setting's value.

  Holds std::string for \ref toy::editor::project::Type::String and \ref toy::editor::project::Type::Enumeration,
  int64_t for \ref toy::editor::project::Type::Integer, double for \ref toy::editor::project::Type::Double, and bool
  for \ref toy::editor::project::Type::Boolean.
*/
using Value = std::variant<std::string, int64_t, double, bool>;

} // namespace toy::editor::project

#endif // EDITOR_SRC_PROJECT_VALUE_HPP_
