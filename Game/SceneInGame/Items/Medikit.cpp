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

#include "AudioManager.h"
#include "Health.h"
#include "Medikit.h"
#include "GameObject.h"
#include "Prefab.h"
#include "Resources.h"
#include "Body.h"
#include "t2Time.h"

bool Medikit::useActual(GameObject *user) {
    auto health = user->getComponent<Health>();
    if (health == nullptr) {
        return false;
    }
    auto power = user->getComponent<Power>();
    power->consume(power->getPower());

    auto hearts = Tile2D::resources().prefabs["fx_hearts"]->instantiate();

    AudioManager::getInstance()->play(Tile2D::resources().audioClips["healing"]);

    auto position =
            user->transform().getPosition() +
            user->getComponent<Body>()->getVelocity() * Tile2D::time().getDeltaTime();

    hearts->transform().setPosition(position);
    hearts->getComponent<Body>()->setVelocity(user->getComponent<Body>()->getVelocity());


    health->heal(healingAmount_);
    return true;
}

float Medikit::getHealingAmount() const {
    return healingAmount_;
}

void Medikit::setHealingAmount(float healingAmount) {
    Medikit::healingAmount_ = healingAmount;
}

ItemBase *Medikit::clone() {
    return new Medikit(*this);
}

void Medikit::deserialize(const json::Object &jsonObject) {
    ItemBase::deserialize(jsonObject);

    if (jsonObject.HasKey("healingAmount")) {
        healingAmount_ = jsonObject["healingAmount"];
    }
}

