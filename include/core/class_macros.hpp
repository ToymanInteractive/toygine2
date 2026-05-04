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
  \file   class_macros.hpp
  \brief  Class-level utility macros for explicit Rule of Five compliance.

  Defines TOYGINE_NO_COPY_MOVE to explicitly delete all four copy and move special member functions. Use in classes that
  inherit from a non-copyable, non-movable base to satisfy MSVC warnings C4625, C4626, C5026, C5027 without repeating
  four declarations in every subclass.

  \note Included by core.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_CORE_CLASS_MACROS_HPP_
#define INCLUDE_CORE_CLASS_MACROS_HPP_

/*!
  \def TOYGINE_NO_COPY_MOVE(ClassName)

  \brief Deletes all four copy and move special member functions of \a ClassName.

  Expands to four \c = \c delete declarations for the copy constructor, copy assignment operator, move constructor, and
  move assignment operator. Satisfies MSVC C4625, C4626, C5026, C5027 on classes that inherit from a non-copyable,
  non-movable base by making implicit deletions explicit.

  Place in the \c public section after the destructor declaration.

  \param ClassName Unqualified name of the enclosing class.

  \note The destructor must be declared separately; this macro does not cover it.
*/
#define TOYGINE_NO_COPY_MOVE(ClassName)                                                                                \
  ClassName(const ClassName &)             = delete;                                                                   \
  ClassName & operator=(const ClassName &) = delete;                                                                   \
  ClassName(ClassName &&)                  = delete;                                                                   \
  ClassName & operator=(ClassName &&)      = delete

#endif // INCLUDE_CORE_CLASS_MACROS_HPP_
