#pragma once

#include <SDL3/SDL.h>
#include <board.h>
#include <score.h>

extern struct AppState {
   SDL_Window *window;
   SDL_Renderer *renderer;
   Board board;
   Score score;
} as;
