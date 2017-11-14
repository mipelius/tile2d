// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#ifndef __RESOURCES_H
#define __RESOURCES_H

#include <map>
#include "AudioClip.h"
#include "Music.h"
#include "BlockMapping.h"
#include "Font.h"
#include "ResourceContainer.h"
#include "Texture.h"

class Resources {
    friend class Tile2D;

private:
    Resources() = default;
    ~Resources() = default;

    void init(
            std::vector<std::string>& resTextures,
            std::vector<std::string>& resSfx,
            std::vector<std::string>& resMusic,
            std::vector<std::string>& resFonts,
            std::vector<std::string>& resBlockMappings
    );

public:
    ResourceContainer<Texture> textures;
    ResourceContainer<Font> fonts;
    ResourceContainer<BlockMapping> blockMappings;
    ResourceContainer<AudioClip> sfx;
    ResourceContainer<Music> music;
};

#endif //__RESOURCES_H
