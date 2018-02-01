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


#ifndef SPACEGAME_BOMBBEHAVIOUR_H
#define SPACEGAME_BOMBBEHAVIOUR_H

#include "Tile2DBehaviour.h"
#include "Body.h"
#include "Sprite.h"

class BombBehaviour : public Tile2DBehaviour {

protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

    static const int explosionRadius = 10; // tiles

    Body* body_;
};

#endif //SPACEGAME_BOMBBEHAVIOUR_H
