// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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


#include <cfloat>
#include "Prefabs.h"
#include "PolygonCollider.h"
#include "WalkingEnemyAI.h"
#include "Tile2D.h"
#include "Tags.h"
#include "Sprite.h"
#include "PulseLightBehaviour.h"
#include "ParticleSystem.h"
#include "Tile2DMath.h"
#include "Health.h"
#include "BombBehaviour.h"
#include "SparkleBehaviour.h"

// ---- ENEMIES ----

GameObject *Prefabs::walkingEnemy() {
    auto enemy = spawnEnemy_(
            "walking_alien_green",
            {
                    {-10, -25},
                    {10, -25},
                    {10, 15},
                    {5, 25},
                    {-5, 25},
                    {-10, 15}
            },
            {-25, -25, 25, 25},
            2.0f
    );

    enemy->getComponent<PolygonCollider>()->setSweepingStrategyThreshold(FLT_MAX);

    auto AI = enemy->attachComponent<WalkingEnemyAI>();

    AI->setGroundCheckSensors(
            {
                    {-10, 26.0f},
                    {-5, 26.0f},
                    {0, 26.0f},
                    {5, 26.0f},
                    {10, 26.0f}
            }
    );

    return enemy;
}


GameObject *Prefabs::crabKindOf() {
    return spawnEnemy_(
            "crab_kindof_colored",
            {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
            {-25, -25, 25, 25},
            0.0f
    );
}

GameObject *Prefabs::fourwayCyclops() {
    return spawnEnemy_(
            "fourwaycyclops_colored",
            {{-30, -24}, {30, -24}, {30, 24}, {-30, 24}},
            {-30, -30, 30, 30},
            0.0f
    );
}

GameObject *Prefabs::twoHorn() {
    return spawnEnemy_(
            "twohorn_colored",
            {{-10, -25}, {10, -25}, {10, 25}, {-10, 25}},
            {-25, -25, 25, 25},
            0.0f
    );
}

GameObject *Prefabs::spawnEnemy_(
        std::string textureName,
        std::vector<Vecf> colliderPoints,
        Rect spriteRect,
        float gravityFactor
) {
    auto enemy = Tile2D::createGameObject();
    enemy->transform().setRotation(0.0f);

    enemy->tag = Tags::enemy;

    auto enemyBody = enemy->attachComponent<Body>();
    enemyBody->setMass(100.0);
    enemyBody->setGravityFactor(gravityFactor);

    auto polygonCollider = enemy->attachComponent<PolygonCollider>();
    polygonCollider->setPoints(colliderPoints);

    auto enemySprite = enemy->attachComponent<Sprite>();
    enemySprite->setRect(spriteRect);
    enemySprite->setTexturePtr(Tile2D::resources().textures[textureName]);
    enemySprite->setColor({1, 1, 1});

    auto health = enemy->attachComponent<Health>();
    health->setMaxHealth(100);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        health->gameObject()->destroy();
        GameObject* newBloodBurst = bloodBurst();
        newBloodBurst->transform().setPosition(health->transform()->getPosition());
    });

    return enemy;
}

// ---- PLAYER STUFF TO DROP / SHOOT ----


GameObject *Prefabs::bomb() {
    static const int explosionRadius = 10; // tiles

    auto bomb = Tile2D::createGameObject();

    auto bompBody = bomb->attachComponent<Body>();
    bompBody->setMass(50);

    auto bompCollider = bomb->attachComponent<PolygonCollider>();
    bompCollider->setPoints({{-9, -4}, {9, -4}, {9, 4}, {-9, 4}});
    bompCollider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
        for (auto x=-explosionRadius; x<explosionRadius; ++x) {
            for (auto y=-explosionRadius; y<explosionRadius; ++y) {
                if (Vecf((float)x, (float)y).length() > explosionRadius) {
                    continue;
                }
                Vecf offset = {
                        (float)(x * Tile2D::tileMap().getTileSet()->getTileW()),
                        (float)(y * Tile2D::tileMap().getTileSet()->getTileH())
                };
                Tile2D::tileMap().setValueScaled(
                        args.tileCoordinates + offset + args.velocityBeforeCollision.normalized() * 30.0f,
                        Tile2D::tileMap().getTileSet()->getEmptyBlock()
                );
            }
        }

        collider->gameObject()->getComponent<Sprite>()->setIsVisible(false);
        collider->gameObject()->destroy();

        auto explosion = Prefabs::explosion();

        explosion->transform() = *(collider->transform());
        explosion->transform().setPosition(
                explosion->transform().getPosition() +
                        args.velocityBeforeCollision.normalized() * 30.0f
        );

        explosion->transform().setScale({0.75, 0.75});
    });

    auto bompSprite = bomb->attachComponent<Sprite>();
    bompSprite->setTexturePtr(Tile2D::resources().textures["bomb"]);
    bompSprite->setRect({-10, -10, 10, 10});

    auto bombBehaviour = bomb->attachComponent<BombBehaviour>();



    return bomb;
}

GameObject *Prefabs::light() {
    auto light = Tile2D::createGameObject();

    auto lightBody = light->attachComponent<Body>();
    lightBody->setMass(10.0);
    lightBody->setVelocity(Vecf(0, 0));

    auto collider = light->attachComponent<PolygonCollider>();
    collider->setPoints({
            {-6, -6},
            {6,  -6},
            {6,  6},
            {-6, 6}
    });

    auto lightSprite = light->attachComponent<Sprite>();
    lightSprite->setRect({-40, -40, 40, 40});
    lightSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto lightLight = light->attachComponent<PointLight>();
    lightLight->setRadius(100.0);
    lightLight->setIntensity(1.0);

    return light;
}

GameObject *Prefabs::laser() {
    auto laser = Tile2D::createGameObject();

    auto laserBody = laser->attachComponent<Body>();
    laserBody->setMass(10.0);
    laserBody->setDrag(0.2);

    auto laserSprite = laser->attachComponent<Sprite>();
    laserSprite->setRect({-20,-5,20,5});
    laserSprite->setTexturePtr(Tile2D::resources().textures["missile"]);

    auto laserAge = laser->attachComponent<LimitedLifetimeBehaviour>();
    laserAge->getTimer().setInterval(1000);

    auto laserLight = laser->attachComponent<PointLight>();
    laserLight->setRadius(80.0);
    laserLight->setIntensity(1.0);

    auto collider = laser->attachComponent<PolygonCollider>();
    collider->setPoints({
            {-18, -5},
            {18,  -5},
            {18,  5},
            {-18, 5}
    });
    collider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
        collider->gameObject()->destroy();
        Tile2D::tileMap().setValueScaled(args.tileCoordinates, Tile2D::tileMap().getTileSet()->getEmptyBlock());

        sparkles(args.tileCoordinates, args.contactNormal, {1, 1, 1});
        pulseLight(collider->transform()->getPosition());
    });
    collider->collision.add([] (PolygonCollider* collider, CollisionEventArgs args) {
        if (args.otherCollider->gameObject()->tag == Tags::enemy) {
            auto otherBody = args.otherCollider->gameObject()->getComponent<Body>();
            float scaleX = args.otherCollider->transform()->getScale().x;
            args.otherCollider->transform()->setScale({-scaleX, 1});

            auto missileBody = collider->gameObject()->getComponent<Body>();

            otherBody->setVelocity(otherBody->getVelocity() + missileBody->getVelocity() / 100.0);
            sparkles(collider->transform()->getPosition(), args.contactNormal, {1, 0, 0});
            missileBody->gameObject()->destroy();

            auto health = args.otherCollider->gameObject()->getComponent<Health>();

            if (health != nullptr) {
                health->damage(10, missileBody->gameObject());
            }

            pulseLight(collider->transform()->getPosition());
        }
    });
    return laser;
}

// ---- EFFECTS ----

GameObject* Prefabs::bloodBurst() {
    auto bloodBurst = Tile2D::createGameObject();

    auto light = bloodBurst->attachComponent<PointLight>();
    light->setRadius(300.0f);
    light->setIntensity(1.0f);

    auto pulseLightBehaviour = bloodBurst->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->getTimer().setInterval(2000);
    pulseLightBehaviour->setTimeToStartDiminish(1000);
    pulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    pulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto particles = bloodBurst->attachComponent<ParticleSystem>();
    particles->setPlaysOnce(true);
    particles->setInitFunc([] (Particle* particle){
        Vecf pos = {(rand() % 10) * 5.0f - 25.0f, (rand() % 10) * 5.0f - 25.0f};
        pos *= 2;
        particle->getTransform().setPosition(pos);
        particle->getTransform().setRotation(rand() % 360);
        float size = 0.25f + (rand() % 255) / 255.0f;
        particle->getTransform().setScale({size, size});
        particle->setVelocity(pos.normalized() * (rand() % 2 + 3.0f));
        particle->setColor({1.0f, 0.0f, 0.0f});
        particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
    });
    particles->setUpdateFunc([] (Particle* particle){
        if (particle->getTimeLived() > 2000) {
            particle->destroy();
        } else {
            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f);
            particle->getTransform().setPosition(pos + particle->getVelocity());
            particle->setOpacity(particle->getOpacity() - 0.01f);
            float newSize = particle->getTransform().getScale().x - 0.007f;
            Mathf::clamp(newSize, 0.0f, 100.0f);
            particle->getTransform().setScale({newSize, newSize});
        }
    });
    particles->setParticleRect({-32, -32, 32, 32});
    particles->setTexturePtr(Tile2D::resources().textures["explosion_particle"]);
    particles->setMaxParticles(100);
    particles->setSpawnFrequency(300);
    particles->setBlendSourceFactor(GL_SRC_ALPHA);
    particles->setBlendDestinationFactor(GL_ONE);

    bloodBurst->transform().setScale({0.5, 0.5});

    return bloodBurst;
}

GameObject *Prefabs::explosion() {
    auto explosion = Tile2D::createGameObject();

    auto explosionLight = explosion->attachComponent<PointLight>();
    explosionLight->setRadius(300.0f);
    explosionLight->setIntensity(1.0f);

    auto explosionPulseLightBehaviour = explosion->attachComponent<PulseLightBehaviour>();
    explosionPulseLightBehaviour->getTimer().setInterval(2000);
    explosionPulseLightBehaviour->setTimeToStartDiminish(1000);
    explosionPulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    explosionPulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto explosionParticles = explosion->attachComponent<ParticleSystem>();
    explosionParticles->setPlaysOnce(true);
    explosionParticles->setInitFunc([] (Particle* particle){
        Vecf pos = {(rand() % 10) * 5.0f - 25.0f, (rand() % 10) * 5.0f - 25.0f};
        pos *= 2;
        particle->getTransform().setPosition(pos);
        particle->getTransform().setRotation(rand() % 360);
        float size = 0.5f + (rand() % 255) / 255.0f;
        particle->getTransform().setScale({size, size});
        particle->setVelocity(pos.normalized() * (rand() % 2 + 3.0f));
        particle->setColor({1.0f, 1.0f, 1.0f});
        particle->setOpacity((rand() % 200) / 400.0f + 0.5f);
    });
    explosionParticles->setUpdateFunc([] (Particle* particle){
        if (particle->getTimeLived() > 2000) {
            particle->destroy();
        } else {
            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f);
            particle->getTransform().setPosition(pos + particle->getVelocity());
            particle->setOpacity(particle->getOpacity() - 0.01f);
            float newSize = particle->getTransform().getScale().x - 0.007f;
            Mathf::clamp(newSize, 0.0f, 100.0f);
            particle->getTransform().setScale({newSize, newSize});
        }
    });
    explosionParticles->setParticleRect({-64, -64, 64, 64});
    explosionParticles->setTexturePtr(Tile2D::resources().textures["explosion_particle"]);
    explosionParticles->setMaxParticles(100);
    explosionParticles->setSpawnFrequency(300);
    explosionParticles->setBlendSourceFactor(GL_SRC_ALPHA);
    explosionParticles->setBlendDestinationFactor(GL_ONE);

    return explosion;
}

void Prefabs::pulseLight(Vecf position) {
    auto obj = Tile2D::createGameObject();
    obj->transform().setPosition(position);

    auto light = obj->attachComponent<PointLight>();
    light->setRadius(80.0);
    light->setIntensity(1.0);

    auto pulseLightBehaviour = obj->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->getTimer().setInterval(1000);
}

void Prefabs::sparkle(Vecf position, Vecf velocity, Color color) {
    auto sparkle = Tile2D::createGameObject();
    sparkle->transform().setPosition(position);
    sparkle->transform().setRotation(velocity.angle());

    auto sparkleBody = sparkle->attachComponent<Body>();
    sparkleBody->setMass(10.0);
    sparkleBody->setVelocity(velocity);

    auto sparkleSprite = sparkle->attachComponent<Sprite>();
    sparkleSprite->setRect({-20, -20, 20, 20});
    sparkleSprite->setColor(color);
    sparkleSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto sparkleBehaviour = sparkle->attachComponent<SparkleBehaviour>();
}

void Prefabs::sparkles(Vecf position, Vecf normal, Color color) {
    Vecf& n = normal;
    Vecf perp = Vecf(n.y, -n.x);

    float div = 100.0;

    Vecf random = Vecf(rand() % 100, rand() % 100) / div;
    sparkle(position, (n + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    sparkle(position, (perp + n + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    sparkle(position, ((perp * -1 + random) + n) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    sparkle(position, (perp + random) * 1000, color);
    random = Vecf(rand() % 100, rand() % 100) / div;
    sparkle(position, (perp + random) * -1000, color);
}
