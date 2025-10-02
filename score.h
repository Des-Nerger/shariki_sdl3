#pragma once

struct Score {
   using This = Score;
   static const int x0 = 535;
   static const int y0 = 66;

   SDL_Texture *digit_tex;

   void deinit();

   void init();

   void draw(); 
};
