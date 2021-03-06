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


#ifndef __DrawableBase_H_
#define __DrawableBase_H_

#include "Transform.h"
#include "Tile2DComponent.h"
#include "Canvas.h"
#include "Color.h"

class DrawableBase : public Tile2DComponent, public ISerializable {
    friend class Canvas;
public:
    DrawableBase();

    void draw(const Canvas& canvas);
    // getters and setters

    bool isVisible() const;
    void setIsVisible(bool isVisible);
    float getOpacity() const;
    void setOpacity(float opacity);
    const Color &getColor() const;
    void setColor(const Color &color);
    const SortingLayer& getSortingLayer() const;
    void setSortingLayer(const SortingLayer& sortingLayer);
    bool isUIDrawable() const;
    void setIsUIDrawable(bool isUIDrawable);

    Transform &localTransform();

    void deserialize(const json::Object &jsonObject) override;

protected:
    virtual void drawActual(const Canvas& canvas) = 0;

    void init() override;
    void onDestroy() override;

private:
    const SortingLayer defaultSortingLayer = { -1, "__DEFAULT__", 0 };

    Transform localTransform_;
    bool isVisible_;
    float opacity_;
    Color color_;
    const SortingLayer* sortingLayer_;
    bool isUIDrawable_;
};


#endif //__DrawableBase_H_
