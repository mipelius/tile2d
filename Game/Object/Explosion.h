// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __Explosion_H_
#define __Explosion_H_

class Texture;
class AnimatedTexture;
class SpriteContainer;
class PointLight;

#include "Point.h"

class Explosion {

    friend class AnimatedTexture_Stopped;

public:
    Explosion(Point point, double radius);

private:
    ~Explosion();

    static Texture* explosionTexture_;
    AnimatedTexture* explosionAnimation_;
    SpriteContainer* spriteContainer_;

    class AnimatedTexture_Stopped;
};

#endif //__Explosion_H_
