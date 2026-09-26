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
  \file   registry_scope.hpp
  \brief  Sections a manifest divides its settings into.

  Defines \ref toy::editor::project::Scope, which each descriptor carries to place its setting in the manifest.
*/
#ifndef EDITOR_SRC_PROJECT_REGISTRY_SCOPE_HPP_
#define EDITOR_SRC_PROJECT_REGISTRY_SCOPE_HPP_

namespace toy::editor::project {

/*!
  \enum   Scope
  \brief  Section of the manifest a setting belongs to.
*/
enum class Scope {
  Common,  ///< Shared by every target
  Desktop, ///< Desktop platform section
  MD,      ///< Mega Drive platform section
  GBA,     ///< Game Boy Advance platform section
};

} // namespace toy::editor::project

#endif // EDITOR_SRC_PROJECT_REGISTRY_SCOPE_HPP_
