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

#ifndef __SpriteContainer_H_
#define __SpriteContainer_H_

class Sprite;
template <typename T> class Property;

#include <list>
#include "IDrawable.h"

class SpriteContainer : public IDrawable {

public:
    SpriteContainer();
    ~SpriteContainer();

    void draw(Canvas* canvas);
    void addSprite(Sprite* sprite);

    Property<Point>* const location;
    Property<double>* const angle;

private:
    std::list<Sprite*> sprites_;

    Point location_;
    double angle_;
};


#endif //__SpriteContainer_H_