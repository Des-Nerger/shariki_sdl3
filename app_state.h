#pragma once

#include <SDL3/SDL.h>
#include <board.h>
#include <score.h>

enum Mode {
   MODE_START_ANIMATION,
   MODE_PROCESSING_PLAYER_INPUT,
   MODE_QUADING_UNQUADING,
   MODE_LINE_BURNING,
   MODE_FALLING,
   MODE_GAME_OVER,
   MODE_NAME_INPUTTING,
   MODE_TOP_PLAYERS,
   MODE_WHETHER_PLAY_AGAIN,
   MODE_END_ANIMATION,
   MODE_COUNT
};

extern struct AppState {
   SDL_Window *window;
   SDL_Renderer *renderer;
   Board board;
   Score score;
   Mode mode;
   int falling_column_idx;
} as;
