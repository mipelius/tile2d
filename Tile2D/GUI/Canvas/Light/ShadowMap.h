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

#ifndef __ShadowMap_H_
#define __ShadowMap_H_

#include <SDL2/SDL_opengl.h>

class PointLight;
class Map;
class ShadowMask;
class Canvas;

class ShadowMap {

public:
    ShadowMap(Map* map);
    ~ShadowMap();

    void draw(Canvas* canvas);

    void update(PointLight* light);

private:
    Map* map_;
    double* staticLightMap_;

    ShadowMask* shadowMask;

    GLuint glShadowTextureId_;
    static const int SHADOW_TEXTURE_SIZE = 32;

    void createShadowTexture();
};

#endif //__ShadowMap_H_
