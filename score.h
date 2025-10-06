#pragma once

struct Score {
   using This = Score;
   static const int x0 = 535;
   static const int y = 66;
   static const int digit_width = 7;
   static const int digit_height = 14;

   SDL_Texture *digit_tex;
   int value;

   void deinit();

   void init();

   void draw(); 
};
