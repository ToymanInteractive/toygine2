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
  \file   volume_type.hpp
  \brief  Selector of the volume channel an audio call addresses.

  Defines \ref toy::audio::VolumeType, which names the music, sound, or voice channel a volume call addresses.

  \note Included by audio.hpp only; do not include this file directly.
*/

#ifndef INCLUDE_AUDIO_VOLUME_TYPE_HPP_
#define INCLUDE_AUDIO_VOLUME_TYPE_HPP_

namespace toy::audio {

/*!
  \enum  VolumeType
  \brief Category of audio output a volume call addresses.

  The three values name the channels the audio module keeps a separate volume for: background music, sound effects, and
  spoken dialogue.
*/
enum class VolumeType : uint8_t {
  Music, ///< Background track.
  Sound, ///< One-shot effect.
  Voice, ///< Spoken dialogue.
};

} // namespace toy::audio

#endif // INCLUDE_AUDIO_VOLUME_TYPE_HPP_
