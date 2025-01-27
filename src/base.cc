#include <base.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <data/entry.h>

SDL_Window* main_window = nullptr;
std::pair<int, int> window_size = {0, 0};
SDL_Renderer* main_renderer = nullptr;
SDL_Event event;
bool running = false;
SDL_Point mouse_position = {0, 0};
SDL_Window* prompt_window = nullptr;
SDL_Renderer* prompt_renderer = nullptr;
std::vector<Entry*> entries;
int top_entries_render_count = 5;
TTF_Font* interface_font = nullptr;
