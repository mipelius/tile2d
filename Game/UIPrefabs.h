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



#ifndef __UIPrefabs_H
#define __UIPrefabs_H

#include "GameObject.h"
#include "Button.h"
#include "Vec.h"
#include "Text.h"

class UIPrefabs {
public:
    static GameObject* button(
            const Vecf &position,
            const char *string,
            const float width,
            void (*handler) (Button* button, Button::ButtonEventArgs args)
    );

    static GameObject* text(
            const Vecf& position,
            const char* string,
            float size,
            Text::HorizontalAlignment horizontalAlignment = Text::HorizontalAlignment::center,
            Text::VerticalAlignment verticalAlignment = Text::VerticalAlignment::center
    );

};


#endif //__UIPrefabs_H
