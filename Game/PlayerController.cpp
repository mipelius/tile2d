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

#include "Tile2D.h"
#include "PlayerController.h"
#include "MissileBehaviour.h"

void PlayerController::awake() {
    body = gameObject()->getComponent<Body>();
    sprite = gameObject()->getComponent<Sprite>();
    lastShotTimestamp = SDL_GetTicks();
}

void PlayerController::update() {
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    double angularVelocity = 0;

    if (state[SDL_SCANCODE_UP]) {
        body->force.set(Vec::byAngle(body->angle.get(), moveForce));
    }
    if (state[SDL_SCANCODE_LEFT]) {
        angularVelocity -= 5;
    }
    if (state[SDL_SCANCODE_RIGHT]) {
        angularVelocity += 5;
    }

    body->angularVelocity.set(angularVelocity);

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_RETURN) {
                Tile2D::isDebugMode = !Tile2D::isDebugMode;
                if (Tile2D::isDebugMode) {
                    sprite->opacity.set(0.5);
                } else {
                    sprite->opacity.set(1.0);
                }
            }
        }
    }

    if (state[SDL_SCANCODE_SPACE]) {
        shoot();
    }
}

void PlayerController::shoot() {
    if (SDL_GetTicks() - lastShotTimestamp >= shootingInterval) {
        shootOnce({0, 0});
        shootOnce(Vec(-10, -15).rotated(body->angle.get()));
        shootOnce(Vec(-10, 15).rotated(body->angle.get()));
        lastShotTimestamp = SDL_GetTicks();
    }
}

void PlayerController::shootOnce(Vec offset) {
    auto missile = Tile2D::createGameObject();

    auto missileBody = missile->attachComponent<Body>();
    missileBody->mass.set(10.0);
    missileBody->position.set(body->position.get() + offset);
    missileBody->velocity.set(Vec::byAngle(body->angle.get(), 30000.0) + body->velocity.get());
    missileBody->angle.set(body->angle.get());
    auto collider = new PolygonCollider(); // TODO NO NEW !
    collider->setPoints({
                                {-18, -5},
                                {18,  -5},
                                {18,  5},
                                {-18, 5}
                        });
    missileBody->setCollider(collider);

    auto missileSprite = missile->attachComponent<Sprite>();
    missileSprite->rect.set({-20,-5,20,5});
    missileSprite->position.bind(missileBody->position);
    missileSprite->angle.bind(missileBody->angle);
    missileSprite->texturePtr.set(Tile2D::resources().textures["missile"]);

    auto missileBehaviour = missile->attachComponent<MissileBehaviour>();
}
