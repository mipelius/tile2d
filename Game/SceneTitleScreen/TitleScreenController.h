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


#ifndef SPACEGAME_TITLESCREENCONTROLLER_H
#define SPACEGAME_TITLESCREENCONTROLLER_H


#include "Tile2DBehaviour.h"
#include "Timer.h"

class TitleScreenController : public Tile2DBehaviour {
protected:
    void awake() override;
    void update() override;
    void lateUpdate() override;

private:
    Timer startGameTimer_;
};


#endif //SPACEGAME_TITLESCREENCONTROLLER_H
