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

#include "precompile.h"
#include "Background.h"
#include "Texture.h"
#include "Camera.h"
#include "Canvas.h"

Background::Background() :
        // values
        texturePtr_(nullptr),
        ratio_(1.0)
{

}

void Background::drawActual(const Canvas& canvas) {
    Rect rect = canvas.getCamera()->getAreaRect();

    if (texturePtr_ != nullptr) {
        texturePtr_->glBind();

        Vecf position = canvas.getCamera()->getPosition();

        GLfloat x = (position.x * ratio_) / texturePtr_->getW();
        GLfloat y = (position.y * ratio_) / texturePtr_->getH();
        GLfloat w = rect.getWidth() / texturePtr_->getW();
        GLfloat h = rect.getHeight() / texturePtr_->getH();

        glBegin(GL_QUADS);
        glTexCoord2f(x - w / 2, y - h / 2);
        glVertex2f(rect.x1, rect.y1);
        glTexCoord2f(x + w / 2, y - h / 2);
        glVertex2f(rect.x2, rect.y1);
        glTexCoord2f(x + w / 2, y + h / 2);
        glVertex2f(rect.x2, rect.y2);
        glTexCoord2f(x - w / 2, y + h / 2);
        glVertex2f(rect.x1, rect.y2);
        glEnd();

        texturePtr_->glUnbind();
    }
    else {
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(rect.getWidth(), 0);
        glVertex2f(rect.getWidth(), rect.getHeight());
        glVertex2f(0, rect.getHeight());
        glEnd();
    }
}

// getters and setters

Texture *Background::getTexturePtr() const {
    return texturePtr_;
}

void Background::setTexturePtr(Texture *texturePtr) {
    texturePtr_ = texturePtr;
}

float Background::getRatio() const {
    return ratio_;
}

void Background::setRatio(float ratio) {
    ratio_ = ratio;
}