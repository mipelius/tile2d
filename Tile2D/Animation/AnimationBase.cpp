// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "AnimationBase.h"

AnimationBase::AnimationBase(unsigned int framesPerSecond, unsigned int frameAmount, bool enableLoop)
{
    fps_ = framesPerSecond;
    frameAmount_ = frameAmount;
    loopEnabled_ = enableLoop;

    AnimationBase::stop();
}

void AnimationBase::update(float seconds) {
    if (isStopped_ || !isPlaying_) return;

    timeElapsedAfterPreviousFrame_ += seconds;

    if (timeElapsedAfterPreviousFrame_ >= 1.0 / fps_) {
        timeElapsedAfterPreviousFrame_ = 0;
        currentFrame_++;

        if (currentFrame_ >= frameAmount_) {
            currentFrame_ = 0;

            if (!loopEnabled_) {
                stop();
            }
        }

        updateActual();
    }
}

void AnimationBase::stop() {
    timeElapsedAfterPreviousFrame_ = 0;
    currentFrame_ = 0;
    isStopped_ = true;
    isPlaying_ = false;
}

void AnimationBase::play() {
    isStopped_ = false;
    isPlaying_ = true;
}

void AnimationBase::pause() {
    isPlaying_ = false;
}
