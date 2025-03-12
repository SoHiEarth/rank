#include <SDL_keyboard.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <prompt.h>
#include <data/entry.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <base.h>
#include <map>
#include <string>
#include <algorithm>
bool prompt_close_requested = false;
std::string* current_text = nullptr;
SDL_Rect current_input_rect;
template <typename T>
void CheckError(T* ptr, std::string message) {
  if (ptr == nullptr) {
    SDL_Log("CheckError: %s: %s", message.c_str(), SDL_GetError());
    exit(1);
  }
}

void TextInput(std::string* text, SDL_Rect input_rect) {
  SDL_StartTextInput();
  current_text = text;
  current_input_rect = input_rect;
}

void RenderTable(SDL_Renderer* renderer, std::string table_title, std::map<std::string, std::string*> table) {
  TTF_SetFontStyle(interface_font, 1);
  SDL_Surface* title_surface = TTF_RenderUTF8_Blended(interface_font, table_title.c_str(), {0, 0, 100, 255});
  CheckError(title_surface, "Failed to render title surface");
  SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
  CheckError(title_texture, "Failed to create title texture");
  SDL_Rect title_rect = {50, 50, title_surface->w, title_surface->h};
  SDL_RenderCopy(renderer, title_texture, nullptr, &title_rect);
  SDL_FreeSurface(title_surface);
  SDL_DestroyTexture(title_texture);

  int y_offset = title_rect.y + title_rect.h + 50;
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
  SDL_RenderDrawLine(renderer,
      50, title_rect.y + title_rect.h + (y_offset - (title_rect.y + title_rect.h)) / 2,
      750, title_rect.y + title_rect.h + (y_offset - (title_rect.y + title_rect.h)) / 2);

  for (auto& [label, value] : table) {
    TTF_SetFontStyle(interface_font, 1);
    SDL_Surface *label_surface = TTF_RenderUTF8_Blended(interface_font, label.c_str(), {0, 0, 0, 200});
    CheckError(label_surface, "Failed to render label surface");
    SDL_Texture *label_texture = SDL_CreateTextureFromSurface(renderer, label_surface);
    CheckError(label_texture, "Failed to create label texture");
    SDL_Rect label_rect = {80, y_offset, label_surface->w, label_surface->h};
    SDL_RenderCopy(renderer, label_texture, nullptr, &label_rect);
    SDL_FreeSurface(label_surface);
    SDL_DestroyTexture(label_texture);

    if (*value == "") {
      *value = "未入力";
    }
    TTF_SetFontStyle(interface_font, 0);
    SDL_Surface *value_surface = TTF_RenderUTF8_Blended(interface_font, value->c_str(), {0, 0, 0, 255});
    CheckError(value_surface, "Failed to render value surface");
    SDL_Texture *value_texture = SDL_CreateTextureFromSurface(renderer, value_surface);
    CheckError(value_texture, "Failed to create value texture");
    SDL_Rect value_rect = {std::clamp(label_rect.x + label_rect.w + 10, label_rect.x + 100, 300), y_offset, value_surface->w, value_surface->h},
      value_rect_bg = {value_rect.x - 5, value_rect.y - 5, value_rect.w + 10, value_rect.h + 10};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &value_rect_bg);
    SDL_RenderCopy(renderer, value_texture, nullptr, &value_rect);
    SDL_FreeSurface(value_surface);
    SDL_DestroyTexture(value_texture);
    if (SDL_PointInRect(&mouse_position, &value_rect)) {
      SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
      SDL_RenderFillRect(renderer, &value_rect_bg);
      if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        TextInput(value, value_rect);
      }
    }
    y_offset += 50;
  }
}

void CreatePromptWindow() {
  prompt_window = SDL_CreateWindow("入力",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800, 600,
      SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP
  );
  prompt_renderer = SDL_CreateRenderer(
      prompt_window,
      -1,
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
}

void PromptEntryEditor(Entry* entry) {
  if (prompt_window == nullptr && prompt_renderer == nullptr)
    CreatePromptWindow();
  std::map<std::string, std::string*> table = {
    {"名前", &entry->name},
    {"スコア", &entry->score}
  };
  prompt_close_requested = false;
  while (!prompt_close_requested) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          prompt_close_requested = true;
          break;
        case SDL_WINDOWEVENT:
          switch (event.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
              prompt_close_requested = false;
              break;
          }
          break;
        case SDL_MOUSEMOTION:
          mouse_position = {event.motion.x, event.motion.y};
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (current_text != nullptr) {
            current_text = nullptr;
            SDL_StopTextInput();
          }
        case SDL_TEXTINPUT:
          if (current_text == nullptr) break;
          if (*current_text == "~") {
            *current_text = "";
          }
          *current_text += event.text.text;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_BACKSPACE && current_text->size() > 0) {
            current_text->pop_back();
            if (current_text->empty()) {
              *current_text = "~";
            }
          }
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            if (current_text != nullptr) {
              current_text = nullptr;
              SDL_StopTextInput();
            }
            else prompt_close_requested = true;
          }
          break;
      }
    }
    SDL_SetRenderDrawColor(
        prompt_renderer,
        255, 255, 255, SDL_ALPHA_OPAQUE
    );
    SDL_RenderClear(prompt_renderer);
    RenderTable(prompt_renderer, "データを入力", table);

    SDL_Surface* submit_surface = TTF_RenderUTF8_Blended(interface_font, "送信", {0, 0, 0, 255});
    CheckError(submit_surface, "Failed to render submit surface");
    SDL_Texture* submit_texture = SDL_CreateTextureFromSurface(prompt_renderer, submit_surface);
    CheckError(submit_texture, "Failed to create submit texture");
    SDL_Rect submit_rect = {
      800 - 50 - submit_surface->w,
      600 - 50 - submit_surface->h,
      submit_surface->w,
      submit_surface->h
    };
    SDL_RenderCopy(prompt_renderer, submit_texture, nullptr, &submit_rect);
    SDL_FreeSurface(submit_surface);
    SDL_DestroyTexture(submit_texture);
    if (SDL_PointInRect(&mouse_position, &submit_rect)) {
      SDL_SetRenderDrawBlendMode(prompt_renderer, SDL_BLENDMODE_BLEND);
      SDL_SetRenderDrawColor(prompt_renderer, 0, 0, 0, 100);
      SDL_RenderFillRect(prompt_renderer, &submit_rect);
      if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
        prompt_close_requested = true;
      }
    }
    SDL_RenderPresent(prompt_renderer);
  }
  SDL_DestroyRenderer(prompt_renderer);
  SDL_DestroyWindow(prompt_window);
  prompt_renderer = nullptr;
  prompt_window = nullptr;
}
