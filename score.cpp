#include <SDL3/SDL.h>
#include <app_state.h>
#include <math.h>
#include <score.h>
#include <stdio.h>

void Score::deinit() {
   SDL_DestroyTexture(this->digit_tex);
   this->value = -1;
}

void Score::init() {
   SDL_Surface *digit_surface = SDL_LoadBMP("digits.bmp");
   this->digit_tex = SDL_CreateTextureFromSurface(
      as.renderer,
      digit_surface);
   SDL_DestroySurface(digit_surface);
   this->value = 1;
}

inline int numOfDigits(int a) {
   return log10(a) + 1;
}

void Score::draw() {
   auto r = this->value;

   SDL_FRect dstrect[] = {{
      .x =
         This::x0 + This::digit_width * (numOfDigits(this->value) - 1),
      .y = This::y,
      .w = This::digit_width,
      .h = This::digit_height }};

   for (
      ;
      dstrect->x >= This::x0;
      r /= 10, dstrect->x -= This::digit_width)
   {
      SDL_FRect srcrect[] = {{
         .x = This::digit_width * (r % 10),
         .y = 0,
         .w = This::digit_width,
         .h = This::digit_height,
      }};
      fprintf(stderr,
              "src(.x=%5.1f) -> dst(.x=%5.1f)\n",
              srcrect->x, dstrect->x);

      // draw a digit
      SDL_RenderTexture(
         as.renderer,
         this->digit_tex,
         srcrect,
         dstrect);
   }
   fprintf(stderr, "\n");

/*   
   for (auto i = num_digits - 1; r != 0; i -= 1, r /= 10) {
      auto d = r % 10;
      this.drawDigit(d);
   }
*/

}
