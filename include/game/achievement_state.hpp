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
  \file   achievement_state.hpp
  \brief  Achievement visibility and unlock state for the game module.
*/

#ifndef INCLUDE_GAME_ACHIEVEMENT_STATE_HPP_
#define INCLUDE_GAME_ACHIEVEMENT_STATE_HPP_

namespace toy::game {

/*!
  \enum AchievementState
  \brief State of an achievement: visibility and whether it is unlocked.

  Used to drive UI (e.g. show, grey out, or hide) and logic (e.g. grant rewards only when \c Unlocked).
*/
enum class AchievementState : uint8_t {
  /// Achievement is not shown to the user (e.g. secret or not yet available).
  Hidden,
  /// Achievement is visible but not yet earned.
  Locked,
  /// Achievement has been earned.
  Unlocked
};

} // namespace toy::game

#endif // INCLUDE_GAME_ACHIEVEMENT_STATE_HPP_
