#ifndef PROMPT_H
#define PROMPT_H

#include <data/entry.h>
Entry* PromptNewEntry();
template <typename T>
void CheckError(T* ptr, std::string message);
#endif // PROMPT_H
