#include "data/entry.h"
#include <data/entry_render.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <base.h>
#include <prompt.h>
void RenderEntry(Entry* entry, SDL_Rect anchor) {
  SDL_Surface* entry_surface = TTF_RenderUTF8_Blended(interface_font, entry->name.c_str(), {255, 255, 255, 255});
  CheckError(entry_surface, "Failed to render text surface");
  SDL_Texture* entry_texture = SDL_CreateTextureFromSurface(main_renderer, entry_surface);
  CheckError(entry_texture, "Failed to create text texture");
  SDL_Rect entry_rect = {
    anchor.y + anchor.h - entry_surface->h,
    entry_surface->w,
    entry_surface->h
  };
  entry_rect.x = anchor.x + ((anchor.w - entry_rect.w) / 2);
  SDL_RenderCopy(main_renderer, entry_texture, nullptr, &entry_rect);
  SDL_FreeSurface(entry_surface);
  SDL_DestroyTexture(entry_texture);

  switch (entry->color) {
    case kEntryColorRed:
      SDL_SetRenderDrawColor(main_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorYellow:
      SDL_SetRenderDrawColor(main_renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorOrange:
      SDL_SetRenderDrawColor(main_renderer, 255, 165, 0, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorGreen:
      SDL_SetRenderDrawColor(main_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorBlue:
      SDL_SetRenderDrawColor(main_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorPurple:
      SDL_SetRenderDrawColor(main_renderer, 128, 0, 128, SDL_ALPHA_OPAQUE);
      break;
    case kEntryColorWhite:
      SDL_SetRenderDrawColor(main_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      break;
  }
  SDL_Rect score_bar_rect = {
    .w = 50,
    .h = 500 * std::stoi(entry->score) / 100,
  };
  score_bar_rect.x = anchor.x + (anchor.w - score_bar_rect.w) / 2;
  score_bar_rect.y = anchor.y + anchor.h - score_bar_rect.h;
  SDL_RenderFillRect(main_renderer, &score_bar_rect);
}
