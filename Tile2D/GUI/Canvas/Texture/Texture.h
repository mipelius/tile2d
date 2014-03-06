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

#ifndef __Texture_H_
#define __Texture_H_

#include <string>
#include "ITexture.h"
#include <SDL2/SDL_opengl.h>

class Texture : public ITexture {
private:
    GLuint texture;
    int w;
    int h;
public:
    int getW();
    int getH();
    Texture(std::string filename);
    void glBind();
    void glUnbind();
    void glTexCorner(Corner corner);
};


#endif //__Texture_H_