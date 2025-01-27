#ifndef ENTRY_H
#define ENTRY_H

#include <string>

enum EntryColor {
  kEntryColorRed,
  kEntryColorOrange,
  kEntryColorYellow,
  kEntryColorGreen,
  kEntryColorBlue,
  kEntryColorPurple,
  kEntryColorWhite
};

struct Entry {
  std::string name,
    score;
  EntryColor color;
};

#endif // ENTRY_H
