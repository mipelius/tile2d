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

#include "GameObject.h"
#include "Tile2D.h"

void GameObject::initializeComponents_() {
    for (auto& component : uninitializedComponents_) {
        component->init();
        components_.push_back(component);
    }
    uninitializedComponents_.clear();
}


void GameObject::destroy() {
    Tile2D::instance_().destroy_(this);
}


GameObject::GameObject() {
    Tile2D::instance_().objectsToInit_.push_back(this);
}


GameObject::~GameObject() {
    for (auto& component : uninitializedComponents_) {
        delete component;
    }
    for (auto& component : components_) {
        component->onDestroy();
    }
    for (auto& component : components_) {
        delete component;
    }
}
