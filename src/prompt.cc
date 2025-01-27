#include <SDL_render.h>
#include <SDL_video.h>
#include <prompt.h>
#include <data/entry.h>
#include <SDL.h>
#include <base.h>
#include <map>
#include <string>
void RenderTable(SDL_Renderer* renderer, std::string table_title, std::map<std::string, std::string*> table) {
  for (auto& [label, value] : table) {
    // Render label and value
  }
}

void CreatePromptWindow() {
  prompt_window = SDL_CreateWindow("入力",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800, 600,
      SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_ALWAYS_ON_TOP
  );
  prompt_renderer = SDL_CreateRenderer(
      prompt_window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
}

bool prompt_close_requested = false;
Entry PromptNewEntry() {
  if (prompt_window == nullptr || prompt_renderer == nullptr) {
    CreatePromptWindow();
  }

  Entry entry;
  std::map<std::string, std::string*> table = {
    {"Name", &entry.name},
  };

  while (!prompt_close_requested) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          prompt_close_requested = true;
          break;
      }
    }
    SDL_SetRenderDrawColor(
        prompt_renderer,
        0, 0, 0, SDL_ALPHA_OPAQUE
    );
    SDL_RenderClear(prompt_renderer);
    RenderTable(prompt_renderer, "データを入力", table);
    SDL_RenderPresent(prompt_renderer);
  }
  SDL_DestroyRenderer(prompt_renderer);
  SDL_DestroyWindow(prompt_window);

  return entry;
}
