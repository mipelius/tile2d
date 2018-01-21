// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#ifndef __PolygonCollider_H
#define __PolygonCollider_H

#include <vector>
#include "Tile2DComponent.h"
#include "Vec.h"
#include "Rect.h"

class PolygonCollider : public Tile2DComponent {
    friend class PolygonCollider;
    friend class Body;
    friend class Projection;

public:
    PolygonCollider();
    ~PolygonCollider() = default;

    bool cast(
            const Vecf&                  direction,
            const PolygonCollider&      otherCollider,
            Vecf&                        contactNormal,
            Vecf&                        toCollision
    );

    const Rect& boundingBox() const;
    const std::vector<Vecf>& points() const;

    bool overlap(
            const PolygonCollider&   otherCollider,
            Vecf&                        contactNormal,
            float&                     penetration
    ) const;

    void setPoints(std::vector<Vecf> points);
    std::vector<Vecf> getAxes() const;

protected:
    void init() override;
    void onDestroy() override;

private:
    Vecf pos_;
    float rot_;

    bool castOneWay_(
            const Vecf &direction,
            const PolygonCollider &collider,
            const PolygonCollider &otherCollider,
            Vecf &contactNormal,
            Vecf &toCollision
    );

    Rect boundingBox_;

    std::vector<Vecf> points_;
};

#endif //__PolygonCollider_H
