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

#include "precompile.h"
#include "MapRenderer.h"
#include "Camera.h"
#include "Map.h"
#include "MapTexture.h"

void MapRenderer::render() {
    if (!mapTexture_ || !map_) return;

    Rect rect = camera_->getRect();

    double x = rect.x1;
    double y = rect.y1;
    double w = rect.getWidth();
    double h = rect.getHeight();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int iStart = 0;
    int jStart = 0;

    if (x > 0) iStart = int(x / map_->getBlockW());
    if (y > 0) jStart = int(y / map_->getBlockH());

    if (iStart < 0) iStart = 0;
    if (jStart < 0) jStart = 0;

    int iEnd = map_->getW();
    int jEnd = map_->getH();

    if (x + w < map_->getW() * map_->getBlockW()) iEnd = int((x + w) / map_->getBlockW() + 1);
    if (y + h < map_->getH() * map_->getBlockH()) jEnd = int((y + h) / map_->getBlockH() + 1);

    // NOTE: the textureNumber that is used for the current block is return value from getValue(i, j) - 1;
    // Maybe you should use more sophisticated way to indicate the correspondence between mapValues and textureNumbers

    mapTexture_->glBind();

    for (int i = iStart; i < iEnd; i++) {
        for (int j = jStart; j < jEnd; j++) {
            if (!map_->getValue(i, j)) continue; // continue if the block is empty
            GLdouble color = sin(((i*map_->getBlockW() - x) / (float)w) * (M_PI / 1.0)) * sin(((j*map_->getBlockH() - y) / (float)h) * (M_PI / 1.0));
            color /= 1.5;
            color += 0.25;
            if (color > 1.0) color = 1.0;
            glColor4d(color, color, color, color);

            mapTexture_->renderBlock(
                    (int)(i * map_->getBlockW() - x),
                    (int)(j * map_->getBlockH() - y),
                    map_->getBlockW(),
                    map_->getBlockH(),
                    map_->getValue(i, j) - 1
            );
        }
    }

    mapTexture_->glUnbind();
}

MapRenderer::MapRenderer(Rect const &renderingAreaRect) : MapRendererBase(renderingAreaRect) { }

void MapRenderer::setMapTexture(MapTexture* mapTexture) {
    mapTexture_ = mapTexture;
}
