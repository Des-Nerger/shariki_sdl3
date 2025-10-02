#pragma once

#include <array>

struct Sharik {
   int x, y;
   // bool is_selected;
   struct Color {
      Uint8 r, g, b;

      using This = Color; 
      bool operator ==(const This& rhs) {
         return this->r == rhs.r &&
                this->g == rhs.g &&
                this->b == rhs.b;
      }
   } color;
   static const int diameter = 40;
};

struct Board {
   using This = Board;
   static const int x0 = 160;
   static const int y0 = 55;
   static const int cols = 8; // ---umn-
   static const int rows = 8;
   using Rgba = std::array<Uint8, 4>;
   static constexpr This::Rgba bg_color = {0xAA, 0xAA, 0xAA, 0xFF};
   Sharik board[This::rows][This::cols];
   std::array<int, 2> selected_ij;
   SDL_Texture *circle;

   Board();
   void draw();
   void init();
   void deinit();
   ~Board() = default;

   void handleIfMatch(std::array<int, 2> ij);
};
