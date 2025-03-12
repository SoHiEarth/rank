#ifndef PROMPT_H
#define PROMPT_H

#include <data/entry.h>
void PromptEntryEditor(Entry* entry);
template <typename T>
void CheckError(T* ptr, std::string message);
#endif // PROMPT_H
