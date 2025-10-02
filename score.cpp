#include <SDL3/SDL.h>
#include <app_state.h>
#include <score.h>

void Score::deinit() {
   SDL_DestroyTexture(this->digit_tex);
}

void Score::init() {
   SDL_Surface *digit_surface = SDL_LoadBMP("digits.bmp");
   this->digit_tex = SDL_CreateTextureFromSurface(
      as.renderer,
      digit_surface);
   SDL_DestroySurface(digit_surface);
}


void Score::draw() {
/*
   const auto numOfDigits(this.value);
   auto r = this.value;
   for (auto i = num_digits - 1; r != 0; i -= 1, r /= 10) {
      auto d = r % 10;
      this.drawDigit(d);
   }
*/
}
