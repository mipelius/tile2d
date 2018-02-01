// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#ifndef SPACEGAME_TRANSFORM_H
#define SPACEGAME_TRANSFORM_H

#include "Vec.h"

class Transform {
    friend class Body;
    friend class PolygonCollider;

public:
    Transform();
    // getters and setters

    const Vecf &getPosition() const;
    void setPosition(const Vecf &position);
    float getRotation() const;
    void setRotation(float rotation);
    const Vecf &getScale() const;
    void setScale(const Vecf &scale);

    void glTransform();
    void glInvTransform();

private:
    Vecf scale_ = {1.0, 1.0};
    Vecf position_ = {0.0f, 0.0f};
    float rotation_ = 0.0f;
};


#endif //SPACEGAME_TRANSFORM_H
