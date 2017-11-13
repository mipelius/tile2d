// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#include "ResourcesTMP.h"

/* ----------- SAMPLES ----------- */

ResourcesTMP::Samples::Samples() :
explosion               (   new Sample("soundfx/explosion.wav",               127)  ),
missileCollision        (   new Sample("soundfx/missile_collision.wav",       30)   ),
spaceshipAcceleration   (   new Sample("soundfx/spaceship_acceleration.wav",  127)  ),
spaceshipCollision      (   new Sample("soundfx/spaceship_collision.wav",     127)  ),
spaceshipExplosion      (   new Sample("soundfx/spaceship_explosion.wav",     100)  ),
spaceshipShoot          (   new Sample("soundfx/spaceship_shoot.wav",         30)   )
{

}

ResourcesTMP::Samples::~Samples() {
    delete explosion;
    delete missileCollision;
    delete spaceshipAcceleration;
    delete spaceshipCollision;
    delete spaceshipExplosion;
    delete spaceshipShoot;
}

/* ----------- TUNES ----------- */

ResourcesTMP::Tunes::Tunes() :
spacegame   (   new Music("music/spacegame.mp3")    )
{

}

ResourcesTMP::Tunes::~Tunes() {
    delete spacegame;
}

/* ----------- TEXTURES ----------- */

ResourcesTMP::Textures::Textures() :
animExplosion           ( new Texture("images/anim_explosion.png"           )   ),
animRocketFire          ( new Texture("images/anim_rocket_fire.png"         )   ),
bg1                     ( new Texture("images/bg1.jpg"                      )   ),
bg2                     ( new Texture("images/bg2.jpg"                      )   ),
bomb                    ( new Texture("images/bomb.png"                     )   ),
light                   ( new Texture("images/light.png"                    )   ),
missile                 ( new Texture("images/missile.png"                  )   ),
spaceship               ( new Texture("images/spaceship.png"                )   ),
inventoryEquipableSlot  ( new Texture("images/inventory_equipable_slot.png" )   ),
inventorySelectedSlot   ( new Texture("images/inventory_selected_slot.png"  )   ),
inventorySlot           ( new Texture("images/inventory_slot.png"           )   ),
laserCannon             ( new Texture("images/laser_cannon.png"             )   ),
plasmaCannon            ( new Texture("images/plasma_cannon.png"            )   )
{

}

ResourcesTMP::Textures::~Textures() {
    delete animExplosion;
    delete animRocketFire;
    delete bg1;
    delete bg2;
    delete bomb;
    delete light;
    delete missile;
    delete spaceship;
}

/* ----------- OTHER ----------- */

ResourcesTMP::Other::Other() :
blockMapping(   new BlockMapping("json/blockMappings.json") ),
smallFont(      new Font("json/smallfont.json"))
{

}

ResourcesTMP::Other::~Other() {
    delete blockMapping;
}

/* ----------- RESOURCES COMMON ----------- */

ResourcesTMP::ResourcesTMP() :
samples(    new Samples()   ),
tunes(      new Tunes()     ),
textures(   new Textures()  ),
other(      new Other()     )
{

}

ResourcesTMP::~ResourcesTMP() {
    delete samples;
    delete tunes;
    delete textures;
    delete other;
}