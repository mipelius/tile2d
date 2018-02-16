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

#include "PolygonCollider.h"
#include <iostream>
#include <cfloat>
#include "LineSegment.h"
#include "Tile2D.h"
#include "Body.h"
#include "Tile2DMath.h"

class Projection {

public:
    Projection(const Vecf& axis, const PolygonCollider& collider) {
        min = axis.dot(collider.points_[0].rotated(collider.rot_) + collider.pos_);
        max = min;
        for (auto i = 1u; i < collider.points_.size(); ++i) {
            float p = axis.dot(collider.points_[i].rotated(collider.rot_) + collider.pos_);
            if (p < min) {
                min = p;
            } else if (p > max) {
                max = p;
            }
        }
    }

    bool overlap(const Projection& other, float& penetration, Vecf& currentContactNormal) {
        penetration = FLT_MAX;
        bool overlap = (min >= other.min || max >= other.min) && (min <= other.max || max <= other.max);
        if (!overlap) { return false; }

        if (min <= other.min && max >= other.min) {
            penetration = max - other.min;
            currentContactNormal *= -1;
            return true;
        } else if (min <= other.max && max >= other.max) {
            if (min <= other.min) {

            }
            penetration = other.max - min;
            return true;
        }
        return overlap;
    }

    float min, max;
};

bool PolygonCollider::castOneWay_(
        const Vecf &direction,
        const PolygonCollider &collider,
        const PolygonCollider &otherCollider,
        Vecf &contactNormal,
        Vecf &toCollision
) {
    bool collided = false;

    toCollision = {0.0, 0.0};

    const PolygonCollider& c1 = collider;
    const PolygonCollider& c2 = otherCollider;

    const Vecf& c1pos = collider.pos_;
    const float& c1rot = collider.rot_;
    const Vecf& c2pos = otherCollider.pos_;
    const float& c2rot = otherCollider.rot_;

    Vecf intersectionPoint;
    float maxDistanceSqr = 0.0f;
    float curDistanceSqr;
    Vecf curVec;

    for (const auto& pointOrig : c1.points_) {
        Vecf castingPoint = pointOrig.rotated(c1rot) + c1pos;
        LineSegment castingLine(castingPoint, castingPoint + direction);

        for (auto i = 0u; i < otherCollider.points_.size(); i++) {
            Vecf startPoint = c2.points_[i].rotated(c2rot) + c2pos;
            Vecf endPoint = c2.points_[(i+1) % c2.points_.size()].rotated(c2rot) + c2pos;
            LineSegment polygonLine(startPoint, endPoint);

            if (castingLine.intersection(polygonLine, intersectionPoint)) {
                collided = true;
                curVec = (intersectionPoint - castingPoint);
                curDistanceSqr = curVec.lengthSqr();
                if (maxDistanceSqr < curDistanceSqr) {
                    maxDistanceSqr = curDistanceSqr;
                    toCollision = curVec;
                    Vecf tmp = startPoint - endPoint;
                    contactNormal = {tmp.y, -tmp.x};
                }
            }
        }
    }

    return collided;
}

bool PolygonCollider::cast(
        const Vecf &direction,
        const PolygonCollider& otherCollider,
        Vecf& contactNormal,
        Vecf& toCollision
) {
    Vecf contactNormalTmp, toCollisionTmp;

    bool collided1 = castOneWay_(direction, *this, otherCollider, contactNormal, toCollision);
    bool collided2 = castOneWay_(direction * -1, otherCollider, *this, contactNormalTmp, toCollisionTmp);

    if (!collided1 && !collided2) {
        return false;
    }

    toCollisionTmp *= -1.0;

    if (
            toCollisionTmp.lengthSqr() > toCollision.lengthSqr()
    ) {
        toCollision = toCollisionTmp;
        contactNormal = contactNormalTmp;
    }

    contactNormal = contactNormal.normalized();
    if (contactNormal.dot(toCollision) < 0) { // flip (if necessary)
        contactNormal = contactNormal * -1;
    }

    return true;
}

PolygonCollider::PolygonCollider() :
        boundingBox_        (   {-1, -1, 1, 1}                                              ),
        collision           (   Event<PolygonCollider, CollisionEventArgs>(this)            ),
        terrainCollision    (   Event<PolygonCollider, TerrainCollisionEventArgs>(this)     )
{
    if (Tile2D::tileMap().isLoaded()) {
        TileSet* tileSet = Tile2D::tileMap().getTileSet();
        sweepingStrategyThreshold_ = (float)sqrt(tileSet->getTileW() * tileSet->getTileW()) / 4.0f;
    } else {
        sweepingStrategyThreshold_ = 2.0f;
    }
}

const Rect &PolygonCollider::boundingBox() const {
    return boundingBox_;
}

bool PolygonCollider::overlap(
        const PolygonCollider&  otherCollider,
        Vecf&                   contactNormal,
        float&                  penetration
) const {
    penetration = FLT_MAX;
    float currentPenetration;
    Vecf currentContactNormal;

    const std::vector<Vecf>& axes1 = getAxes();
    const std::vector<Vecf>& axes2 = otherCollider.getAxes();

    for (const auto& axis : axes1) {
        Projection p1(axis, *this);
        Projection p2(axis, otherCollider);
        currentContactNormal = axis;
        if (!p1.overlap(p2, currentPenetration, currentContactNormal)) {
            return false;
        } else if (currentPenetration < penetration) {
            penetration = currentPenetration;
            contactNormal = currentContactNormal;
        }
    }

    for (const auto& axis : axes2) {
        Projection p1(axis, *this);
        Projection p2(axis, otherCollider);
        currentContactNormal = axis;
        if (!p1.overlap(p2, currentPenetration, currentContactNormal)) {
            return false;
        } else if (currentPenetration < penetration){
            penetration = currentPenetration;
            contactNormal = currentContactNormal;
        }
    }

    return true;
}

std::vector<Vecf> PolygonCollider::getAxes() const {
    std::vector<Vecf> axes;

    for (auto i = 0u; i < points_.size(); i++) {
        const Vecf& p1 = points_[i];
        const Vecf& p2 = points_[(i+1) % points_.size()];
        Vecf direction = (p2 - p1).rotated(rot_);
        Vecf vec = Vecf(direction.y, -direction.x).normalized();
        axes.push_back(vec);
    }

    return axes;
}

void PolygonCollider::setPoints(std::vector<Vecf> points) {
    points_ = std::move(points);

    Vecf farMost = Vecf(0, 0);

    for (const auto& vec : this->points_) {
        float length = vec.length();
        float farMostLength = Vecf(farMost.x, farMost.y).length();
        if (length > farMostLength) farMost = vec;
    }

    float length = Vecf(farMost.x, farMost.y).length();
    boundingBox_ = Rect(-length, -length, length, length);
}

const std::vector<Vecf> &PolygonCollider::points() const {
    return points_;
}

void PolygonCollider::init() {
    Tile2D::physicsWorld().colliders_.push_back(this);
    body_ = gameObject()->getComponent<Body>();
}

void PolygonCollider::onDestroy() {
    Tile2D::physicsWorld().colliders_.remove(this);
}

bool PolygonCollider::detectTerrainCollision_(float deltaTime) {
    TileMap* map = &Tile2D::tileMap();

    if (!map->isLoaded()) {
        return false;
    }

    bool collided = false;

    pos_ = transform()->position_;
    rot_ = transform()->rotation_;

    Rect boundingBox = boundingBox_;

    boundingBox.x1 += transform()->position_.x;
    boundingBox.x2 += transform()->position_.x;
    boundingBox.y1 += transform()->position_.y;
    boundingBox.y2 += transform()->position_.y;

    int blockSizeW = map->getTileSet()->getTileW();
    int blockSizeH = map->getTileSet()->getTileH();

    int iBegin = (int)boundingBox.x1 - ((int)boundingBox.x1) % blockSizeW;
    int iEnd = (int)boundingBox.x2 + (int)boundingBox.x2 % blockSizeW;
    int jBegin = (int)boundingBox.y1 - ((int)boundingBox.y1) % blockSizeH;
    int jEnd = (int)boundingBox.y2 + (int)boundingBox.y2 % blockSizeH;

    Vecf direction =                {0.0f, 0.0f};
    Vecf velocityBeforeCollision =  {0.0f, 0.0f};

    if (body_ != nullptr) {
        velocityBeforeCollision = body_->velocity_;
        direction = body_->velocity_ * deltaTime * Tile2D::physicsWorld().metersPerPixel_;
    }

    Tile* tile;

    float w = blockSizeW;
    float h = blockSizeH;
    PolygonCollider tileCollider;
    tileCollider.rot_ = 0.0;
    tileCollider.setPoints({{0.0, 0.0}, {0.0, h}, {w, h}, {w, 0.0}});

    // if slowly moving object or body doesn't exist -> use SAT strategy
    if (direction.length() < sweepingStrategyThreshold_ || body_ == nullptr) {
        Vecf contactNormal;
        float penetration;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vecf(i, j));
                if (tile != nullptr && tile->getDensity() > 0.0) {
                    tileCollider.pos_ = {(float)i, (float)j};

                    if (overlap(tileCollider, contactNormal, penetration)) {
                        pos_ += contactNormal * (penetration + 0.05);
                        collided = true;

                        terrainCollision.raise(
                                {
                                        deltaTime,
                                        contactNormal,
                                        tileCollider.pos_,
                                        tile,
                                        velocityBeforeCollision
                                }
                        );
                    }
                }
            }
        }

        if (collided) {
            if (body_ != nullptr)  {
                body_->velocity_ = {0, 0};
            }
            transform()->position_ = pos_;
        }

    } else { // if fast object -> use polygon casting/sweeping
        Vecf contactNormal;
        Vecf currentContactNormal;

        Vecf toCollision = {0, 0};
        Vecf currentToCollision;

        Tile* collisionTile = nullptr;
        Vecf collisionTileCoord;

        for (int i=iBegin; i <= iEnd; i += blockSizeW) {
            for (int j=jBegin; j <= jEnd ; j += blockSizeH) {
                tile = map->getValueScaled(Vecf(i, j));
                if (tile != nullptr && tile->getDensity() > 0.0) {
                    tileCollider.pos_ = {(float)i, (float)j};

                    if (cast(direction * -1.0, tileCollider, currentContactNormal, currentToCollision))
                    {
                        collided = true;

                        if (currentToCollision.lengthSqr() > toCollision.lengthSqr()) {
                            collisionTile = tile;
                            collisionTileCoord = tileCollider.pos_;

                            toCollision = currentToCollision;
                            contactNormal = currentContactNormal;
                        }
                    }
                }
            }
        }
        if (collided) {
            const Vecf v = body_->velocity_;
            const Vecf& n = contactNormal;

            Vecf proj_n_v = n * v.dot(n);
            Vecf reflVel = v + proj_n_v * -2.0;

            body_->velocity_ = reflVel * 0.2;
            transform()->position_ += toCollision * (1.01);

            terrainCollision.raise(
                    {
                            deltaTime,
                            n,
                            collisionTileCoord,
                            collisionTile,
                            velocityBeforeCollision
                    }
            );
        }
    }

    return collided;
}

bool PolygonCollider::detectCollisionWith_(PolygonCollider& otherCollider) {
    if (!boundingBox_.intersectsWith(otherCollider.boundingBox_)) return false;

    Vecf contactNormal;
    float penetration;

    if (overlap(otherCollider, contactNormal, penetration)) {
        collision.raise(
                {
                        &otherCollider,
                        contactNormal
                }
        );
        return true;
    }

    return false;
}

float PolygonCollider::getSweepingStrategyThreshold() const {
    return sweepingStrategyThreshold_;
}

void PolygonCollider::setSweepingStrategyThreshold(float sweepingStrategyThreshold) {
    sweepingStrategyThreshold_ = sweepingStrategyThreshold;
    if (sweepingStrategyThreshold_ < 0) {
        sweepingStrategyThreshold_ = 0;
    }
}

unsigned int PolygonCollider::getLayer() const {
    return layer_;
}

void PolygonCollider::setLayer(unsigned int layer) {
    layer_ = layer;
}
