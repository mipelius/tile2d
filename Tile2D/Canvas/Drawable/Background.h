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

#ifndef __Background_H_
#define __Background_H_

#include "DrawableBase.h"

class Texture;

class Background : public DrawableBase {

public:
    Background();

    void drawActual(const Canvas& canvas) override;

    // getters and setters

    Texture *getTexturePtr() const;
    void setTexturePtr(Texture *texturePtr);
    float getRatio() const;
    void setRatio(float ratio);

private:
    Texture* texturePtr_;

private:

    float ratio_;

};


#endif //__BackgroundRenderer_H_