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
#include "Camera.h"
#include "Body.h"
#include "PhysicsWorld.h"
#include "SimpleProperty.h"

class Camera::LocationProperty : public Property<Point> {

public:

    LocationProperty(Camera* camera) : Property<Point>((void*)camera) { }

    Point getActual() {
        Rect rect = ((Camera*)actualData_)->areaRect->get();
        double x = rect.x1 + rect.getWidth() / 2;
        double y = rect.y1 + rect.getHeight() / 2;
        return Point(x, y);
    }

    void setActual(Point value) {
        Rect areaRect = ((Camera*)actualData_)->areaRect->get();
        Rect boundsRect = ((Camera*)actualData_)->boundsRect->get();

        double x = value.x - areaRect.getWidth() / 2;
        double y = value.y - areaRect.getHeight() / 2;
        double w = areaRect.getWidth();
        double h = areaRect.getHeight();

        double x1 = boundsRect.x1;
        double y1 = boundsRect.y1;
        double x2 = boundsRect.x2;
        double y2 = boundsRect.y2;

        if (x < x1)     x = x1;
        if (x + w > x2) x = x2 - w;
        if (y < y1)     y = y1;
        if (y + h > y2) y = y2 - h;

        ((Camera*)actualData_)->areaRect->set(
                Rect(
                        x,
                        y,
                        x + w,
                        y + h
                )
        );
    }

};

Camera::Camera() :
    // properties

    location(       new LocationProperty      (this           )   ),
    areaRect(       new SimpleProperty<Rect>  (&areaRect_     )   ),
    boundsRect(     new SimpleProperty<Rect>  (&boundsRect_   )   ),

    // member objects
    areaRect_(      Rect(0, 0, 0, 0)    ),
    boundsRect_(    Rect(0, 0, 0, 0)    )
{

}

Camera::~Camera() {
    delete location;
    delete areaRect;
    delete boundsRect;
}


void Camera::zoom(double amount) {
    double ratio = areaRect_.getHeight() / areaRect_.getWidth();

    areaRect_.x1 += amount;
    areaRect_.y1 += amount * ratio;
    areaRect_.x2 -= amount;
    areaRect_.y2 -= amount * ratio;
    areaRect->updateDependentProperties();
}