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

#ifndef __TILE2D_H
#define __TILE2D_H

#include <map>
#include <assert.h>
#include <set>
#include "LightSystem.h"
#include "PhysicsWorld.h"
#include "SceneManager.h"
#include "Resources.h"
#include "Window.h"
#include "Canvas.h"
#include "GameObject.h"
#include "Tile2DBehaviour.h"
#include "precompile.h"
#include "Particle.h"
#include "Pool.h"
#include "ColliderLayerMatrix.h"
#include "PathFinder.h"
#include "Input.h"

class Tile2D {
    friend class Tile2DObject;
    friend class GameObject;
    friend class Tile2DBehaviour;
    friend class SceneManager;
    friend class ParticleSystem;
    friend class Particle;

public:
    Tile2D(Tile2D const &)              = delete;
    Tile2D& operator=(Tile2D const &)   = delete;
    Tile2D(Tile2D &&)                   = delete;
    Tile2D& operator=(Tile2D &&)        = delete;

    void static load(
            const std::string&                      configFile,
            const std::string&                      resourcesFile,
            std::map<unsigned, IScene*>             scenes,
            std::vector<ColliderLayerMatrix::Rule>  colliderLayerRules
    );

    static std::string getResourcePath();

    static Window &window();
    static Resources &resources();
    static SceneManager &sceneManager();
    static PhysicsWorld &physicsWorld();
    static Canvas &canvas();
    static LightSystem &lightSystem();
    static TileMap &tileMap();
    static PathFinder &pathFinder();
    static const Input &input();

    static void executeDelayedFunction(
            GameObject* gameObject,
            Uint32      delay,
            void        (*function)(GameObject*)
    );

    static GameObject* createGameObject();

    static bool isDebugMode();
    static void setIsDebugMode(bool isDebugMode);

    static void quit();

private:
    Window *window_;
    Resources *resources_;
    SceneManager *sceneManager_;
    PhysicsWorld *physicsWorld_;
    Canvas* canvas_;
    LightSystem* lightSystem_;
    TileMap* tileMap_;
    PathFinder* pathFinder_;
    Input* input_;

    void mainLoop_();
    void cleanUp_();
    void removeDestroyedObjects_();
    void initGameObjects_();
    void updateBehaviours_();
    void lateUpdateBehaviours_();
    void PollSDL_Events();

    Tile2D();
    ~Tile2D();

    static Tile2D &instance_();

    static bool isLoaded_;

    std::set<GameObject*> objects_;
    std::set<GameObject*> objectsToDestroy_;
    std::list<GameObject*> objectsToInit_;

    struct DelayedFunction {
        GameObject* gameObject;
        Uint32 delay;
        void (*function)(GameObject*);
        Uint32 timestamp;
    };

    std::list<Tile2DBehaviour*> behaviours_;
    std::list<DelayedFunction> delayedFunctions_;

    void destroy_(GameObject* obj);
    void executeDelayedFunctions_();

    Pool<Particle> particlePool_;

    bool isDebugMode_;
    bool quit_;

    static const unsigned int MAX_PARTICLES = 10000;
};

#endif //__TILE2D_H
