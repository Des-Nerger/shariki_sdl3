#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <math.h>
#include <stdio.h>
#include <utility>

#include <app_state.h>
#include <board.h>

#define SDL_WINDOW_WIDTH         640
#define SDL_WINDOW_HEIGHT        480

SDL_AppResult SDL_AppIterate(void *unused_appstate)
{
   (void)unused_appstate;
   SDL_SetRenderDrawColor(as.renderer, 200, 111, 111, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(as.renderer);
   as.board.draw();
   SDL_RenderPresent(as.renderer);
   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(
   void **unused_appstate,
   int argc,
   char *argv[])
{
   (void)unused_appstate;

   if (!SDL_Init(SDL_INIT_VIDEO)) {
      SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   if (!SDL_CreateWindowAndRenderer(
      "shariki_sdl3",
      SDL_WINDOW_WIDTH,
      SDL_WINDOW_HEIGHT,
      0,
      &as.window,
      &as.renderer)
   ) return SDL_APP_FAILURE;

   as.board.init();
   as.score.init();

   return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *unused_appstate, SDL_Event *event)
{
   (void)unused_appstate;
   switch (event->type) {
   case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;
   case SDL_EVENT_KEY_DOWN:
      switch (event->key.scancode) {
      /* Quit. */
      case SDL_SCANCODE_ESCAPE:
      case SDL_SCANCODE_Q:
         return SDL_APP_SUCCESS;
      }
      return SDL_APP_CONTINUE;
   case SDL_EVENT_MOUSE_BUTTON_DOWN:
      {
         auto &mouse_button = event->button; // mouse
         std::array<int, 2> selected_ij = {
            ((int) mouse_button.y - Board::y0) / Sharik::diameter,
            ((int) mouse_button.x - Board::x0) / Sharik::diameter
         };
         if (as.board.selected_ij == selected_ij /* ||
             as.board.board
               [selected_ij[0]]
               [selected_ij[1]].color == Sharik::Color{
                  Board::bg_color[0],
                  Board::bg_color[1],
                  Board::bg_color[2] } */ ) {
            as.board.selected_ij = std::array<int, 2>{-1, -1};
         } else {
            if (abs(as.board.selected_ij[0] - selected_ij[0]) +
               abs(as.board.selected_ij[1] - selected_ij[1]) == 1
            ) {
               std::swap(
                  as.board.board[selected_ij[0]][selected_ij[1]].color,
                  as.board.board
                     [as.board.selected_ij[0]]
                     [as.board.selected_ij[1]].color);
               as.board.handleIfMatch({selected_ij});
               as.board.handleIfMatch({as.board.selected_ij});
               as.board.selected_ij = {-1, -1};
            } else as.board.selected_ij = selected_ij;
         }

         // auto &sh /*arik */ = as.board.board
         //    [selected_ij[0]]
         //    [selected_ij[1]];

         // printf("x=%7.2f  y=%7.2f\n", mouse_button.x, mouse_button.y);
         // std::swap(sh.color.r, sh.color.g);
         // sh.is_selected ^= true;
      }
      break;   
   default:
      break;
   }
   return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *unused_appstate, SDL_AppResult result)
{
   (void)unused_appstate;
   as.score.deinit();
   as.board.deinit();
   if (as.renderer != NULL) {
      SDL_DestroyRenderer(as.renderer);
      as.renderer = NULL;
   }
   if (as.window != NULL) {
      SDL_DestroyWindow(as.window);
      as.window = NULL;
   }
}

