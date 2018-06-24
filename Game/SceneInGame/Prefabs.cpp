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


#include <cfloat>
#include <Game/CollisionEffects.h>
#include "Healer.h"
#include "ItemTags.h"
#include "Cannon.h"
#include "BombDropper.h"
#include "Inventory.h"
#include "AnimatedSprite.h"
#include "Scenes.h"
#include "Prefabs.h"
#include "PolygonCollider.h"
#include "WalkingEnemyAI.h"
#include "Tile2D.h"
#include "Sprite.h"
#include "PulseLightBehaviour.h"
#include "ParticleSystem.h"
#include "Tile2DMath.h"
#include "Health.h"
#include "BombBehaviour.h"
#include "SparkleBehaviour.h"
#include "FlyingEnemyAI.h"
#include "PlayerController.h"
#include "DebugBehaviour.h"
#include "BackgroundBehaviour.h"
#include "HUD.h"
#include "Resources.h"
#include "SceneManager.h"
#include "t2Time.h"
#include "Power.h"
#include "SwirlingBehaviour.h"
#include "PlayerTargetingComponent.h"
#include "EnemyTargetingComponent.h"
#include "AmmoComponent.h"
#include "PickupSpawner.h"
#include "Pickup.h"
#include "PhysicsWorld.h"

GameObject *Prefabs::player() {
    auto player = Tile2D::createGameObject();
    player->transform().setRotation(0.0f);

    player->setTag(Tile2D::getTag(0));

    auto debugBehaviour = player->attachComponent<DebugBehaviour>();

    auto spaceshipBody = player->attachComponent<Body>();
    spaceshipBody->setMass(100.0);

    auto polygonCollider = player->attachComponent<PolygonCollider>();
    polygonCollider->setPoints({
                                       {-20, -18},
                                       {-5, -18},
                                       {18, 0},
                                       {-5, 18},
                                       {-20, 18}
                               });
    polygonCollider->setLayer(Tile2D::physicsWorld().getColliderLayerMatrix().getColliderLayer(2));

    auto spaceshipSprite = player->attachComponent<Sprite>();
    spaceshipSprite->setRect({-20, -20, 20, 20});
    spaceshipSprite->setTexturePtr(Tile2D::resources().textures["spaceship"]);
    spaceshipSprite->setSortingLayer(Tile2D::canvas().getSortingLayer(5));

    auto light = player->attachComponent<PointLight>();
    light->setIntensity(1.0);
    light->setRadius(600.0);

    auto playerController = player->attachComponent<PlayerController>();
    playerController->moveForce = 100000.0f;

    auto health = player->attachComponent<Health>();
    health->setMaxHealth(300);
    health->setAutoHealingRate(30);
    health->onDeath.add([] (Health* health, GameObjectDiedEventArgs args) {
        GameObject* spaceShipExplosion = explosion();
        spaceShipExplosion->transform().setPosition(health->transform()->getPosition());
        health->gameObject()->setIsActive(false);

        Tile2D::executeDelayedFunction(health->gameObject(), 2000, [] (GameObject* gameObject) {
            gameObject->transform().setPosition({500.0f, 250.0f});
            gameObject->getComponent<Body>()->setVelocity({0.0f, 0.0f});
            gameObject->setIsActive(true);
            gameObject->getComponent<Health>()->reset();
        });
    });

    auto power = player->attachComponent<Power>();
    power->setMaxPower(300);
    power->setReloadSpeed(200);

    auto targetingComponent = player->attachComponent<PlayerTargetingComponent>();

    // --- ITEMS ---

    auto inventory = player->attachComponent<Inventory>();

    // -- 1 -- LASER
    auto laserCannon = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["laser_cannon"],
            ItemTags::laser,
            true
    );
    laserCannon->setAmmoPrefab(Tile2D::resources().prefabs["laserAmmo"]);
    laserCannon->setOffsets({
                                    {-10, -13},
                                    {-10, 13}
                            });
    laserCannon->setPowerConsumption(50);
    laserCannon->setReloadDelay(100);
    laserCannon->setIsActivated(true);

    // -- 2 -- GATLING
    auto gatlingGun = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["gatling"],
            ItemTags::gatling,
            true
    );
    gatlingGun->setAmmoPrefab(Tile2D::resources().prefabs["gatlingAmmo"]);
    gatlingGun->setOffsets({
                                   {-10, -13},
                                   {0,   0},
                                   {-10, 13}
                           });
    gatlingGun->setPowerConsumption(20);
    gatlingGun->setReloadDelay(10);
    gatlingGun->setIsActivated(false);

    // -- 3 -- BOMBS
    auto bombDropper = inventory->attachItem<BombDropper>(
            Tile2D::resources().textures["bomb"],
            ItemTags::bombDropper,
            false
    );
    bombDropper->setPowerConsumption(200);
    bombDropper->setCount(0);
    bombDropper->setIsActivated(false);

    // -- 4 -- PLASMA
    auto plasmaCannon = inventory->attachItem<Cannon>(
            Tile2D::resources().textures["plasma_cannon"],
            ItemTags::plasmaCannon,
            true
    );
    plasmaCannon->setAmmoPrefab(Tile2D::resources().prefabs["plasmaAmmo"]);
    plasmaCannon->setPowerConsumption(300);
    plasmaCannon->setReloadDelay(500);
    plasmaCannon->setIsActivated(false);

    // -- 5 -- HEALER
    auto healer = inventory->attachItem<Healer>(
            Tile2D::resources().textures["healer"],
            ItemTags::healer,
            false
    );
    healer->setPowerConsumption(100);
    healer->setHealingAmount(100);
    healer->setIsActivated(false);
    healer->setCount(0);

    return player;
}


GameObject *Prefabs::bomb() {
    static const int explosionRadius = 10; // tiles

    auto bomb = Tile2D::createGameObject();

    auto body = bomb->attachComponent<Body>();
    body->setMass(50);

    auto collider = bomb->attachComponent<PolygonCollider>();
    collider->setPoints({{-9, -4}, {9, -4}, {9, 4}, {-9, 4}});
    collider->terrainCollision.add([] (PolygonCollider* collider, TerrainCollisionEventArgs args) {
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

    auto sprite = bomb->attachComponent<Sprite>();
    sprite->setTexturePtr(Tile2D::resources().textures["bomb"]);
    sprite->setRect({-10, -10, 10, 10});

    auto bombBehaviour = bomb->attachComponent<BombBehaviour>();

    return bomb;
}

// ---- PICK UPS ----

GameObject *Prefabs::gatlingPickup() {
    return createPickup_(
            Tile2D::resources().textures["gatling_box"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto inventory = args.otherCollider->gameObject()->getComponent<Inventory>();
                    auto item = inventory->getItem(ItemTags::gatling);
                    item->setIsActivated(true);
                }
            }
    );
}

GameObject *Prefabs::plasmaCannonPickup() {
    return createPickup_(
            Tile2D::resources().textures["plasma_cannon_box"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto inventory = args.otherCollider->gameObject()->getComponent<Inventory>();
                    auto item = inventory->getItem(ItemTags::plasmaCannon);
                    item->setIsActivated(true);
                }
            }
    );
}


GameObject* Prefabs::bombPickup() {
    return createPickup_(
            Tile2D::resources().textures["bomb_box"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto inventory = args.otherCollider->gameObject()->getComponent<Inventory>();
                    auto item = inventory->getItem(ItemTags::bombDropper);
                    item->setIsActivated(true);
                    int countBefore = item->getCount();
                    item->setCount(countBefore + 10);
                }
            }
    );
}

GameObject* Prefabs::healerPickup() {
    return createPickup_(
            Tile2D::resources().textures["healer_box"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto inventory = args.otherCollider->gameObject()->getComponent<Inventory>();
                    auto item = inventory->getItem(ItemTags::healer);
                    item->setIsActivated(true);
                    int countBefore = item->getCount();
                    item->setCount(countBefore + 5);
                }
            }
    );
}

GameObject* Prefabs::laserCannonUpgradePickup() {
    return createPickup_(
            Tile2D::resources().textures["laser_cannon_upgrade_box"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto inventory = args.otherCollider->gameObject()->getComponent<Inventory>();
                    auto item = inventory->getItem(ItemTags::laser);
                    auto laserCannon = dynamic_cast<Cannon*>(item);

                    laserCannon->setOffsets({
                                                    {-10, -13},
                                                    {0,   0},
                                                    {-10, 13}
                                            });

                    inventory->setItemTexture(
                            ItemTags::laser,
                            Tile2D::resources().textures["laser_cannon_upgraded"]
                    );
                }
            }
    );
}

GameObject* Prefabs::healthUpgradePickup() {
    return createPickup_(
            Tile2D::resources().textures["health_upgrade"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto health = args.otherCollider->gameObject()->getComponent<Health>();
                    auto currentMaxHealth = health->getMaxHealth();
                    health->setMaxHealth(currentMaxHealth + 100);
                    health->reset();
                }
            }
    );
}

GameObject* Prefabs::powerUpgradePickup() {
    return createPickup_(
            Tile2D::resources().textures["power_upgrade"],
            [] (PolygonCollider* polygonCollider, CollisionEventArgs args) {
                if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
                    auto power = args.otherCollider->gameObject()->getComponent<Power>();
                    auto currentMaxPower = power->getMaxPower();
                    power->setMaxPower(currentMaxPower + 100);
                }
            }
    );
}


GameObject* Prefabs::createPickup_(
        Texture *pickupTexture,
        void (*onCollisionFunctionPtr)(PolygonCollider *, CollisionEventArgs)
) {
    auto pickup = Tile2D::createGameObject();

    auto pickupSprite = pickup->attachComponent<Sprite>();
    pickupSprite->setTexturePtr(pickupTexture);
    pickupSprite->setRect({-20.0f, -20.0f, 20.0f, 20.0f});

    auto pickupBgSprite = pickup->attachComponent<Sprite>();
    pickupBgSprite->setTexturePtr(
            Tile2D::resources().textures["pickup_bg"]
    );
    pickupBgSprite->setRect({-32.0f, -32.0f, 32.0f, 32.0f});

    auto pickupBehaviour = pickup->attachComponent<Pickup>();
    pickupBehaviour->setPickupBgSprite(pickupBgSprite);
    pickupBehaviour->setBlinkingSpeed(1.0f);

    auto pickupBody = pickup->attachComponent<Body>();
    pickupBody->setMass(100.0f);
    pickupBody->setDrag(1.0f);

    auto pickupCollider = pickup->attachComponent<PolygonCollider>();
    pickupCollider->setPoints({
                                      {-20, -20},
                                      {20, -20},
                                      {20, 20},
                                      {-20, 20}
                              });
    pickupCollider->setLayer(Tile2D::physicsWorld().getColliderLayerMatrix().getColliderLayer(4));
    pickupCollider->collision.add(onCollisionFunctionPtr);
    pickupCollider->collision.add([] (PolygonCollider* collider, CollisionEventArgs args) {
        if (&args.otherCollider->gameObject()->getTag() == &Tile2D::getTag(0)) {
            collider->gameObject()->destroy();
        }
    });

    return pickup;
}

// ---- EFFECTS ----

GameObject *Prefabs::light() {
    auto light = Tile2D::createGameObject();

    auto lightBody = light->attachComponent<Body>();
    lightBody->setMass(10.0);
    lightBody->setVelocity(Vecf(0, 0));

    auto collider = light->attachComponent<PolygonCollider>();
    collider->setPoints({
                                {-5, -5},
                                {5,  -5},
                                {5,  5},
                                {-5, 5}
                        });

    auto lightSprite = light->attachComponent<Sprite>();
    lightSprite->setRect({-40, -40, 40, 40});
    lightSprite->setTexturePtr(Tile2D::resources().textures["light"]);

    auto lightLight = light->attachComponent<PointLight>();
    lightLight->setRadius(100.0);
    lightLight->setIntensity(1.0);

    return light;
}

GameObject* Prefabs::bloodBurst() {
    auto bloodBurst = Tile2D::createGameObject();

    auto light = bloodBurst->attachComponent<PointLight>();
    light->setRadius(300.0f);
    light->setIntensity(1.0f);

    auto pulseLightBehaviour = bloodBurst->attachComponent<PulseLightBehaviour>();
    pulseLightBehaviour->setTimeToLive(2000);
    pulseLightBehaviour->setTimeToStartDiminish(1000);
    pulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    pulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto particles = bloodBurst->attachComponent<ParticleSystem>();
    particles->setPlaysOnce(true);
    particles->setInitializer([](Particle *particle) {
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
    particles->setUpdater([](Particle *particle) {
        if (particle->getTimeLived() > 2000) {
            particle->destroy();
        } else {
            auto deltaTime = Tile2D::time().getDeltaTime() * 60.0f;

            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f * deltaTime);
            particle->getTransform().setPosition(pos + particle->getVelocity() * deltaTime);
            particle->setOpacity(particle->getOpacity() - 0.01f * deltaTime);
            float newSize = particle->getTransform().getScale().x - 0.007f * deltaTime;
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
    explosionPulseLightBehaviour->setTimeToLive(2000);
    explosionPulseLightBehaviour->setTimeToStartDiminish(1000);
    explosionPulseLightBehaviour->setRadiusDiminishSpeed(0.5f);
    explosionPulseLightBehaviour->setIntensityDiminishSpeed(1.5f);

    auto explosionParticles = explosion->attachComponent<ParticleSystem>();
    explosionParticles->setPlaysOnce(true);
    explosionParticles->setInitializer([](Particle *particle) {
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
    explosionParticles->setUpdater([](Particle *particle) {
        if (particle->getTimeLived() > 2000) {
            particle->destroy();
        } else {
            auto deltaTime = Tile2D::time().getDeltaTime() * 60.0f;

            Vecf pos = particle->getTransform().getPosition();
            particle->getTransform().setRotation(particle->getTransform().getRotation() + 1.0f * deltaTime);
            particle->getTransform().setPosition(pos + (particle->getVelocity() * deltaTime));
            particle->setOpacity(particle->getOpacity() - (0.01f * deltaTime));
            float newSize = particle->getTransform().getScale().x - 0.007f * deltaTime;
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

// --- OTHER ---

GameObject *Prefabs::background(Rect area, const char *texture, Color color) {
    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setRatio(0.5f);
    bg->setTexturePtr(Tile2D::resources().textures[texture]);
    bg->setColor(color);
    bg->setOpacity(0.0f);
    bg->setSortingLayer(Tile2D::canvas().getSortingLayer(0));
    auto bgBehaviour = background->attachComponent<BackgroundBehaviour>();
    bgBehaviour->setArea(area);
    return background;
}

GameObject *Prefabs::enemySpawner(
        Rect area,
        GameObject *target,
        const std::string& prefabString,
        Uint32 spawningDelay)
{
    GameObject* spawnerObj = Tile2D::createGameObject();
    auto spawnerBehaviour = spawnerObj->attachComponent<EnemySpawner>();
    spawnerBehaviour->setPlayer(target);
    spawnerBehaviour->setPrefab(prefabString);
    spawnerBehaviour->setSpawningDelay(spawningDelay);
    spawnerBehaviour->setAreaRect(area);
    spawnerBehaviour->setInnerRect({-600.0f, -400.0f, 600.0f, 400.0f});
    spawnerBehaviour->setOuterRect({-1000.0f, -800.0f, 1000.0f, 800.0f});

    return spawnerObj;
}
GameObject* Prefabs::pickupSpawner(
        Rect area,
        GameObject *target,
        const std::string& prefabString,
        Uint32 spawningDelay,
        int itemTag,
        int maxItemCount,
        float minDistanceToTarget
) {
    GameObject* spawnerObj = Tile2D::createGameObject();
    auto spawnerBehaviour = spawnerObj->attachComponent<PickupSpawner>();
    spawnerBehaviour->setPlayer(target);
    spawnerBehaviour->setPrefab(prefabString);
    spawnerBehaviour->setSpawningDelay(spawningDelay);
    spawnerBehaviour->setItemTag(itemTag);
    spawnerBehaviour->setMaxItemCount(maxItemCount);
    spawnerBehaviour->setMinDistanceToTarget(minDistanceToTarget);
    spawnerBehaviour->setAreaRect(area);

    return spawnerObj;
}

GameObject *Prefabs::hud(GameObject* player) {
    auto hud = Tile2D::createGameObject();
    auto hudBehaviour = hud->attachComponent<HUD>();
    hudBehaviour->setPlayer(player);
    return hud;
}