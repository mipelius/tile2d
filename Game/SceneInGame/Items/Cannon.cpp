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

#include "Cannon.h"
#include "GameObject.h"
#include "Body.h"
#include "PolygonCollider.h"
#include "Prefab.h"
#include "Resources.h"

void Cannon::shoot(const Vecf &from, const Vecf &direction, const Vecf &shooterVelocity) {
    if (offsets_.empty()) {
        shootOnce_(from, direction, shooterVelocity);
    }
    for (const auto& cannonOffset : offsets_) {
        Vecf fromActual = from + cannonOffset.rotated(direction.angle());
        shootOnce_(fromActual, direction, shooterVelocity);
    }
}

void Cannon::shootOnce_(const Vecf &from, const Vecf &direction, const Vecf &shooterVelocity) {
    if (ammoPrefab_ == nullptr) {
        return;
    }
    auto ammo = ammoPrefab_->instantiate();
    ammo->transform().setPosition(from);
    ammo->transform().setRotation(direction.angle());

    auto laserBody = ammo->getComponent<Body>();
    laserBody->setVelocity(direction.normalized() * 2000.0 + shooterVelocity);
}

void Cannon::setAmmoPrefab(Prefab *ammoPrefab) {
    ammoPrefab_ = ammoPrefab;
}

void Cannon::setOffsets(const std::list<Vecf> &offsets) {
    offsets_ = offsets;
}

void Cannon::deserialize(const json::Object &jsonObject) {
    WeaponBase::deserialize(jsonObject);

    if (jsonObject.HasKey("offsets")) {
        offsets_.clear();

        auto offsetsJson = jsonObject["offsets"].ToArray();
        for (const auto& offsetJson : offsetsJson) {
            auto offset = Vecf();
            offset.deserialize(offsetJson.ToObject());
            offsets_.push_back(offset);
        }
    }

    if (jsonObject.HasKey("ammoPrefab")) {
        auto prefabName = jsonObject["ammoPrefab"].ToString();
        ammoPrefab_ = Tile2D::resources().prefabs[prefabName];
    }
}

ItemBase *Cannon::clone() {
    return new Cannon(*this);
}