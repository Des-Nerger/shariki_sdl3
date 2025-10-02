#include <SDL3/SDL.h>
#include <array>
#include <tuple>
#include <app_state.h>
#include <board.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum Color {
   COLOR_CYAN,
   COLOR_WHITE,
   COLOR_BLUE,
   COLOR_DARK_BLUE,
   COLOR_GREEN,
   COLOR_YELLOW,
   COLOR_GRAY,
   COLOR_RED,
   COLOR_VIOLET,
   COLOR_COUNT
};

Uint8 all_colors[COLOR_COUNT][3] = {
   {0x55, 0xFF, 0xFF},
   {0xFF, 0xFF, 0xFF},
   {0x55, 0x55, 0xFF},
   {0x00, 0x00, 0xAA},
   {0x00, 0xAA, 0x00},
   {0xFF, 0xFF, 0x55},
   {0x55, 0x55, 0x55},
   {0xAA, 0x00, 0x00},
   {0xAA, 0x00, 0xAA},
};

using This = Board;

void This::deinit() {
   SDL_DestroyTexture(this->circle);
   this->circle = NULL;
}

void This::init() {
   const auto W = Sharik::diameter + 1;
   const auto H = Sharik::diameter + 1;
   this->circle = SDL_CreateTexture(
      as.renderer,
      SDL_PIXELFORMAT_RGBA32,
      SDL_TEXTUREACCESS_STREAMING,
      W,
      H);
   if (this->circle == NULL) {
      fprintf(stderr, "%s\n", SDL_GetError());
      exit(EXIT_FAILURE);
   }
   This::Rgba pix[H][W];
   std::fill(
      &pix[0][0],
      &pix[0][0] + (H * W),
      This::bg_color);
   const auto r = (Sharik::diameter / 2);
   const auto r2 = r * r;
   auto y = 0;
   const auto X = W / 2 + (int)(r / sqrt(2));
   for (
      int f = r2, x = W / 2;
      x <= X;
      f += 2 * (x - W / 2) + 1, x += 1)
   {
      const auto f_up = f - 2 * (H / 2 - y) + 1;
      if (std::abs(r2 - f_up) < std::abs(r2 - f)) {
         f = f_up, y += 1;
 
         #define PUT_ROW_YX1X2(Y, X1, X2) do { \
            auto y_ = (Y); \
            auto x2 = (X2); \
            for (auto x_ = (X1); x_ <= x2; x_++) { \
               auto *p = &pix[y_][x_][0]; \
               *(p++) = 0x00; \
               *(p++) = 0x00; \
               *(p++) = 0x00; \
               *p = 0x00; \
            } \
         } while (false) 
 
         if (y != 0) {
            PUT_ROW_YX1X2(y, W / 2 - (x - W / 2) + 1, x - 1);
            PUT_ROW_YX1X2(
               H / 2 - (y - H / 2),
               W / 2 - (x - W / 2) + 1,
               x - 1);
         }
      }

/*
      PUT_PIX_YX(H / 2 - (y - H / 2), x);
      PUT_PIX_YX(H / 2 - (y - H / 2), W / 2 - (x - W / 2));

      PUT_PIX_YX(x, y);
      PUT_PIX_YX(x, H / 2 - (y - H / 2));
*/

      {
         const auto row = W / 2 - (x - W / 2);
         PUT_ROW_YX1X2(
            row,
            y + 1,
            H / 2 - (y - H / 2) - 1 );
      }
      PUT_ROW_YX1X2(x, y + 1, H / 2 - (y - H / 2) - 1);

      #define PUT_PIX_YX(Y, X) do { \
         auto *p = &pix[(Y)][(X)][0]; \
         *(p++) = 0x00; \
         *(p++) = 0x00; \
         *(p++) = 0x00; \
         *p = 0xFF; \
      } while (false)

      PUT_PIX_YX(y, x);
      PUT_PIX_YX(y, W / 2 - (x - W / 2));

      PUT_PIX_YX(H / 2 - (y - H / 2), x);
      PUT_PIX_YX(H / 2 - (y - H / 2), W / 2 - (x - W / 2));

      PUT_PIX_YX(x, y);
      PUT_PIX_YX(x, H / 2 - (y - H / 2));

      PUT_PIX_YX(W / 2 - (x - W / 2), y);
      PUT_PIX_YX(W / 2 - (x - W / 2), H / 2 - (y - H / 2));

      /* fprintf(
         stderr,
         "x = %d, y = %d, f = %d, f_up = %d, r2 = %d\n",
         x,
         y,
         f,
         f_up,
         r2); */
   }
   SDL_UpdateTexture(
      this->circle,
      NULL,
      pix,
      sizeof(pix[0]));
}

This::Board() {
   this->selected_ij = {-1, -1};
   srand(time(NULL));
   for (
      auto [i, y] = std::tuple{0, This::y0};
      i < This::rows;
      i += 1, y += Sharik::diameter
   ) {
      for (
         auto [j, x] = std::tuple{0, This::x0};
         j < This::cols;
         j += 1, x += Sharik::diameter
      ) {
         const auto &rand_color = all_colors[rand() % COLOR_COUNT];
         board[i][j] = {
            .x = x,
            .y = y,
            // .is_selected = false,
            .color = {
               .r = rand_color[0],
               .g = rand_color[1],
               .b = rand_color[2]
            },
         };
      }
   }
}

void This::draw() {
   SDL_FRect rect =
      { .w = Sharik::diameter, .h = Sharik::diameter };
   for (auto i = 0; i < This::rows; i += 1) {
      for (auto j = 0; j < This::cols; j += 1) {
         auto &sh /*arik */ = board[i][j]; 
         rect.x = sh.x;
         rect.y = sh.y;
         auto color = sh.color;
         // if (sh.is_selected) memset(&color, 0x00, sizeof(color));
         if (this->selected_ij == std::array<int, 2>{i, j})
            memset(&color, 0x00, sizeof(color));
         SDL_SetRenderDrawColor(
            as.renderer,
            color.r,
            color.g,
            color.b,
            SDL_ALPHA_OPAQUE);
         SDL_RenderFillRect(as.renderer, &rect);
      }
   }

   rect = {
      .w = (float) (this->circle->w),
      .h = (float) (this->circle->h) };
   for (auto i = 0; i < This::rows; i += 1) {
      for (auto j = 0; j < This::cols; j += 1) {
         auto &sh /*arik */ = board[i][j]; 
         rect.x = sh.x;
         rect.y = sh.y;
         SDL_RenderTexture(as.renderer, this->circle, NULL, &rect);
      }
   }
}

void This::handleIfMatch(std::array<int, 2> ij) {
   const auto color = as.board.board[ij[0]][ij[1]].color;

   auto upmost_i = ij[0];
   while (upmost_i >= 1 &&
          as.board.board[upmost_i - 1][ij[1]].color == color)
      upmost_i -= 1;
   auto downmost_i = ij[0];
   while (downmost_i <= This::rows &&
          as.board.board[downmost_i + 1][ij[1]].color == color)
      downmost_i += 1;

   auto leftmost_j = ij[1];
   while (leftmost_j >= 1 &&
          as.board.board[ij[0]][leftmost_j - 1].color == color)
      leftmost_j -= 1;
   auto rightmost_j = ij[1];
   while (rightmost_j <= This::cols &&
          as.board.board[ij[0]][rightmost_j + 1].color == color)
      rightmost_j += 1;

   if (downmost_i - upmost_i + 1 >= 3)
      for (auto i = upmost_i; i <= downmost_i; i++)
         as.board.board[i][ij[1]].color = {
            .r = This::bg_color[0],
            .g = This::bg_color[1],
            .b = This::bg_color[2]
         };

   if (rightmost_j - leftmost_j + 1 >= 3)
      for (auto j = leftmost_j; j <= rightmost_j; j++)
         as.board.board[ij[0]][j].color = {
            .r = This::bg_color[0],
            .g = This::bg_color[1],
            .b = This::bg_color[2]
         };

   // fprintf(stderr, "(%d, %d): %d %d %d %d\n",
   //   ij[0], ij[1], leftmost_j, downmost_i, upmost_i, rightmost_j);
}
