#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <base.h>
#include <prompt.h>
#include <data/entry_render.h>
int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  main_window = SDL_CreateWindow("ランキング一覧",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      1920, 1080,
      SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN
  );
  SDL_GetWindowSize(main_window, &window_size.first, &window_size.second);
  main_renderer = SDL_CreateRenderer(
      main_window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  interface_font = TTF_OpenFont("NotoSansJP-Regular.ttf", 24);
  TTF_SetFontStyle(interface_font, TTF_STYLE_BOLD);
  running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
          }
          break;
        case SDL_MOUSEMOTION:
          mouse_position = {event.motion.x, event.motion.y};
          break;
        case SDL_WINDOWEVENT_RESIZED:
          SDL_GetWindowSize(main_window, &window_size.first, &window_size.second);
          break;
      }
    }
    SDL_SetRenderDrawColor(
        main_renderer,
        0, 0, 0, SDL_ALPHA_OPAQUE
    );
    SDL_RenderClear(main_renderer);

    SDL_Surface* add_data_surface = TTF_RenderUTF8_Blended(interface_font, "データを追加", {255, 255, 255, 255});
    CheckError(add_data_surface, "Failed to render text surface");
    SDL_Texture* add_data_texture = SDL_CreateTextureFromSurface(main_renderer, add_data_surface);
    CheckError(add_data_texture, "Failed to create text texture");
    SDL_Rect add_data_rect = {10, 10, add_data_surface->w, add_data_surface->h};
    SDL_RenderCopy(main_renderer, add_data_texture, nullptr, &add_data_rect);
    SDL_FreeSurface(add_data_surface);
    SDL_DestroyTexture(add_data_texture);
    if (SDL_PointInRect(&mouse_position, &add_data_rect)) {
      SDL_SetRenderDrawBlendMode(main_renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 100);
      SDL_RenderFillRect(main_renderer, &add_data_rect);
      if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        entries.push_back(PromptNewEntry());
      }
    }

    if (!entries.empty()) {
      for (int i = 0; i < std::min(top_entries_render_count, (int)entries.size()); i++) {
        entries[i]->name = (entries[i]->name.empty()) ? "名前未設定" : entries[i]->name;
        SDL_Rect anchor = {
          50 + ((window_size.first - 100)/top_entries_render_count) * i,
          150,
          (window_size.first - 100)/top_entries_render_count,
          window_size.second - 50 - 150
        };
        RenderEntry(entries[i], anchor);
      }
    }
    SDL_RenderPresent(main_renderer);
  }
  
  TTF_CloseFont(interface_font);
  SDL_DestroyRenderer(prompt_renderer);
  SDL_DestroyWindow(prompt_window);
  SDL_DestroyRenderer(main_renderer);
  SDL_DestroyWindow(main_window);

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}
