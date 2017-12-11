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

#include <Tile2D/Core/Tile2D.h>
#include "precompile.h"
#include "TileMap.h"
#include "Body.h"
#include "CollisionShape.h"
#include "TileSet.h"
#include "WorldMapModifiedEventArgs.h"


TileMap::TileMap() :
    modification(this),
    blocks_(nullptr)
{ }

TileMap::~TileMap() {
    if (blocks_ != nullptr) {
        delete blocks_;
    }
}

void TileMap::setValue(int x, int y, Tile* value) {
    if (blocks_->isInsideBounds(x, y)) {
        Tile* oldValue = blocks_->getValue(x, y);

        if (oldValue != value) {
            blocks_->setValue(x, y, value);

            WorldMapModifiedEventArgs args;
            args.x = x;
            args.y = y;
            args.oldValue = oldValue;
            args.newValue = value;

            modification.raise(args);
        }
    }
}


Tile* TileMap::getValue(int x, int y) {
    if (blocks_->isInsideBounds(x, y)) {
        return blocks_->getValue(x, y);
    }

    return nullptr;
}

void TileMap::setValueScaled(Vec point, Tile *value) {
    setValue(
            (int)(point.x / blockSizeW),
            (int)(point.y / blockSizeH),
            value
    );
}

Tile *TileMap::getValueScaled(Vec point) {
    return getValue(
            (int)(point.x / blockSizeW),
            (int)(point.y / blockSizeH)
    );
}

int TileMap::getW() {
    return blocks_->getW();
}

int TileMap::getH() {
    return blocks_->getH();
}

long TileMap::getActualW() {
    return blocks_->getW() * blockSizeW;
}

long TileMap::getActualH() {
    return blocks_->getH() * blockSizeH;
}

bool TileMap::detectCollisionWith(Body* body) {
    if (body->getCollisionShape() == nullptr) {
        return false;
    }

    Tile* tile = this->getValueScaled(body->position.get());

    if (tile != nullptr && tile->density.get() > 0.0) return true;

    std::vector<Vec> points = body->getCollisionShape()->getRotatedPoints();

    for (int i=0; i< body->getCollisionShape()->points.get().size(); i++) {
        Vec point(
                points[i].x + body->position.get().x,
                points[i].y + body->position.get().y
        );

        tile = this->getValueScaled(point);
        if (tile != nullptr && tile->density.get() > 0.0) return true;
    }

    Rect boundingBox = body->getCollisionShape()->getBoundingBox();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    for (int i=iBegin; i <= iEnd; i += blockSizeW) {
        for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
            tile = this->getValueScaled(Vec(i, j));
            if (tile != nullptr && tile->density.get() > 0.0) {
                Rect rect = Rect(i, j, i + blockSizeW, j + blockSizeH);
                if (body->getCollisionShape()->intersectsWith(rect)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int TileMap::getBlockW() {
    return this->blockSizeW;
}


int TileMap::getBlockH() {
    return this->blockSizeH;
}

void TileMap::load(std::string path, TileSet* tileSet, int blockSizeW, int blockSizeH) {
    mapping_ = tileSet;

    this->blockSizeW = blockSizeW;
    this->blockSizeH = blockSizeH;

    SDL_Surface* surface = IMG_Load(path.data());
    if (!surface) {
        std::fprintf(stderr, "Error during loading map: %s\n", SDL_GetError());
        exit(1);
    }

    Uint8* pixels = (Uint8*)surface->pixels;

    blocks_ = new Array2d<Tile*>(surface->w, surface->h);

    for (int i=0; i<surface->w; i++) {
        for (int j=0; j<surface->h; j++) {
            unsigned char id = *(pixels + j * surface->w + i);
            Tile* value = tileSet->getTile(id);
            blocks_->setValue(i, j, value);
        }
    }

    SDL_FreeSurface(surface);
}

void TileMap::init() {
    if (Tile2D::physicsWorld().map_ != nullptr) {
        throw std::runtime_error("Tile2D doesn't support multiple maps!");
    }
    Tile2D::physicsWorld().map_ = this;
}

void TileMap::onDestroy() {
    Tile2D::physicsWorld().map_ = nullptr;
    Tile2DObject::onDestroy();
}
