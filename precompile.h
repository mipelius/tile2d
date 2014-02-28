#ifndef __precompile_H_
#define __precompile_H_

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>

#ifdef __MACOS
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#endif

#ifdef WIN32
#include <SDL_opengl.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#endif


#endif //__precompile_H_
