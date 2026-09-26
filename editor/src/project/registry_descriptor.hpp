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
  \file   registry_descriptor.hpp
  \brief  Declaration of one setting: its name, section, type and default.

  Defines \ref toy::editor::project::Descriptor, the element type of \ref toy::editor::project::c_registry.
*/
#ifndef EDITOR_SRC_PROJECT_REGISTRY_DESCRIPTOR_HPP_
#define EDITOR_SRC_PROJECT_REGISTRY_DESCRIPTOR_HPP_

#include <core.hpp>

#include "registry_scope.hpp"
#include "value.hpp"

namespace toy::editor::project {

/*!
  \struct Descriptor
  \brief  Everything the editor knows about one setting.

  The registry of descriptors is the single source of truth: reading consults it for a value's type, writing for what
  belongs in the file, and validation for what is legal. Only the default matching the descriptor's type is read.
*/
struct Descriptor {
  StringView name;           ///< Node name in the manifest
  Scope      scope;          ///< Section the setting lives in
  Type       type;           ///< Kind of value the setting holds
  StringView defaultString;  ///< Default for \ref toy::editor::project::Type::String and \c Enumeration
  int64_t    defaultInteger; ///< Default for \ref toy::editor::project::Type::Integer
  double     defaultDouble;  ///< Default for \ref toy::editor::project::Type::Double
  bool       defaultBoolean; ///< Default for \ref toy::editor::project::Type::Boolean
};

} // namespace toy::editor::project

#endif // EDITOR_SRC_PROJECT_REGISTRY_DESCRIPTOR_HPP_
