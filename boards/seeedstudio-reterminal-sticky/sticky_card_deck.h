#pragma once

#include <globals.h>
#include <vector>

#if defined(STICKY_MONOCHROME)
void drawStickyCardDeckMenu(std::vector<MenuOptions> &opt, int index, bool forceFullRedraw);
void drawRightButtonRail(const char* confirm = "RUN", const char* up = "UP", const char* down = "DOWN", const char* back = "BACK");
#endif
