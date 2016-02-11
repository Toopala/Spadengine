#pragma once

#ifdef _WIN32
#  include "SDL/SDL_assert.h"
#else
#  include "SDL2/SDL_assert.h"
#endif

#define SGE_ASSERT SDL_assert
