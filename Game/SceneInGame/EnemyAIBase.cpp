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


#include "EnemyAIBase.h"

#include "Tile2D.h"
#include "GameObject.h"
#include "TileMap.h"
#include "Reflector.h"

void EnemyAIBase::awake() {
    body_ = gameObject()->getComponent<Body>();
}

GameObject *EnemyAIBase::getTarget() const {
    return target_;
}

void EnemyAIBase::setTarget(GameObject *target) {
    target_ = target;
}

void EnemyAIBase::shootTarget_() {
    switch (state) {
        case WAIT:
            if (shootingPauseTimer_.getTime() > shootingPauseInterval_) {
                state = SHOOT;
            }
            break;
        case SHOOT:
            if (weapon_ != nullptr && canSeeTarget_()) {
                if (weapon_->use(gameObject())) {
                    ++shots_;
                }
            }
            if (shots_ >= shotsAtOnce_ && shotsAtOnce_ != -1) {
                shots_ = 0;
                state = WAIT;
                shootingPauseTimer_.reset();
            }
            break;
    }
}

void EnemyAIBase::update() {
    float distanceSqr = (target_->transform().getPosition() - transform()->getPosition()).lengthSqr();

    if (distanceSqr > (maxDistance_ * maxDistance_)) {
        gameObject()->destroy();
    }
}

float EnemyAIBase::getMaxDistance() const {
    return maxDistance_;
}

void EnemyAIBase::setMaxDistance(float maxDistance) {
    maxDistance_ = maxDistance;
}

bool EnemyAIBase::canSeeTarget_() {
    Vecf collisionPoint;
    return !Tile2D::tileMap().castLine(
            transform()->getPosition(),
            target_->transform().getPosition(),
            collisionPoint
    );
}

void EnemyAIBase::deserialize(const json::Object &jsonObject) {
    if (jsonObject.HasKey("maxDistance")) {
        maxDistance_ = jsonObject["maxDistance"].ToFloat();
    }
    if (jsonObject.HasKey("weapon")) {
        auto weaponJson = jsonObject["weapon"];
        auto className = weaponJson["class"].ToString();
        auto propertiesJson = weaponJson["properties"].ToObject();

        auto weapon = Tile2D::reflector().instantiate(className);
        weapon->deserialize(propertiesJson);

        weapon_ = (WeaponBase*)weapon;
    }
    if (jsonObject.HasKey("shotsAtOnce")) {
        shotsAtOnce_ = jsonObject["shotsAtOnce"].ToInt();
    }
    if (jsonObject.HasKey("shootingPauseInterval")) {
        shootingPauseInterval_ = jsonObject["shootingPauseInterval"].ToInt();
    }
}

EnemyAIBase::~EnemyAIBase() {
    delete weapon_;
}