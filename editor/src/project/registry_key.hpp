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
  \file   registry_key.hpp
  \brief  Identifiers of the settings the editor knows.

  Defines \ref toy::editor::project::Key and \ref toy::editor::project::c_keyCount, the number of settings it
  enumerates. A setting is always named by its key, so a typo in a setting name fails to compile.
*/
#ifndef EDITOR_SRC_PROJECT_REGISTRY_KEY_HPP_
#define EDITOR_SRC_PROJECT_REGISTRY_KEY_HPP_

#include <core.hpp>

namespace toy::editor::project {

/*!
  \enum   Key
  \brief  Every setting the editor knows, and the slot each one occupies.

  An enumerator's value is the index of its descriptor in \ref toy::editor::project::c_registry and of its value in
  \ref toy::editor::project::Project.
*/
enum class Key : size_t {
  Name,  ///< Project name, \ref toy::editor::project::Type::String
  Count, ///< Number of settings; not a setting
};

/// Number of settings in the registry.
inline constexpr size_t c_keyCount = std::to_underlying(Key::Count);

} // namespace toy::editor::project

#endif // EDITOR_SRC_PROJECT_REGISTRY_KEY_HPP_
