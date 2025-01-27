#ifndef BASE_H
#define BASE_H

#include <SDL.h>
#include <vector>
#include <data/entry.h>
extern SDL_Window* main_window;
extern SDL_Renderer* main_renderer;
extern SDL_Event event;
extern bool running;
extern SDL_Window* prompt_window;
extern SDL_Renderer* prompt_renderer;
extern std::vector<Entry> entries;
extern int top_entries_render_count;
#endif // BASE_H
