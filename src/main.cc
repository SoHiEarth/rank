#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <base.h>
int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  main_window = SDL_CreateWindow("こんにちは",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      1920, 1080,
      SDL_WINDOW_FULLSCREEN_DESKTOP
  );
  main_renderer = SDL_CreateRenderer(
      main_window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
      }
    }
    SDL_SetRenderDrawColor(
        main_renderer,
        0, 0, 0, SDL_ALPHA_OPAQUE
    );
    SDL_RenderClear(main_renderer);
    SDL_RenderPresent(main_renderer);
  }
  
  SDL_DestroyRenderer(prompt_renderer);
  SDL_DestroyWindow(prompt_window);
  SDL_DestroyRenderer(main_renderer);
  SDL_DestroyWindow(main_window);

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}
