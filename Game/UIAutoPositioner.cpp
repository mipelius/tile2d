// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Tile2D.h"
#include "Window.h"
#include "UIAutoPositioner.h"

Tile2DComponent *UIAutoPositioner::clone() {
    return new UIAutoPositioner(*this);
}

void UIAutoPositioner::awake() {

}

void UIAutoPositioner::update() {

}

void UIAutoPositioner::lateUpdate() {
    auto windowSize = Tile2D::window().getSize();

    Vecf center = {
            windowSize.x / 2.0f,
            windowSize.y / 2.0f
    };

    transform()->setPosition(center + relativePosition_);


}

const Vecf &UIAutoPositioner::getRelativePosition() const {
    return relativePosition_;
}

void UIAutoPositioner::setRelativePosition(const Vecf &relativePosition) {
    relativePosition_ = relativePosition;
}
