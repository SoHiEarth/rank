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
  std::string name;
  EntryColor color;
};

#endif // ENTRY_H
