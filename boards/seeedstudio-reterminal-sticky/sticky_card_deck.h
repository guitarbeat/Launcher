#pragma once

#include <globals.h>
#include <vector>

#if defined(STICKY_MONOCHROME)
void drawStickyCardDeckMenu(std::vector<MenuOptions> &opt, int index, bool forceFullRedraw);
#endif
