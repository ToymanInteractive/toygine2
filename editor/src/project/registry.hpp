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
  \file   registry.hpp
  \brief  Compile-time declaration of every setting the editor knows.

  Defines \ref toy::editor::project::c_registry, one \ref toy::editor::project::Descriptor per
  \ref toy::editor::project::Key. Adding a setting means adding an enumerator, a descriptor and the static_assert
  pinning the two together.
*/
#ifndef EDITOR_SRC_PROJECT_REGISTRY_HPP_
#define EDITOR_SRC_PROJECT_REGISTRY_HPP_

#include <core.hpp>

#include "registry_descriptor.hpp"
#include "registry_key.hpp"

namespace toy::editor::project {

/// Declaration of every setting, indexed by \ref toy::editor::project::Key.
inline constexpr std::array c_registry{
  Descriptor{"name", Scope::Common, Type::String, "Untitled", 0, 0.0, false},
};

static_assert(c_registry.size() == c_keyCount, "every Key must have exactly one descriptor");
static_assert(c_registry[std::to_underlying(Key::Name)].name == "name",
              "descriptor order must match the Key enumeration");

} // namespace toy::editor::project

#endif // EDITOR_SRC_PROJECT_REGISTRY_HPP_
