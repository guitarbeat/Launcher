#pragma once

#include <globals.h>
#include <vector>

#if defined(STICKY_MONOCHROME)
void drawStickyCardDeckMenu(std::vector<MenuOptions> &opt, int index, bool forceFullRedraw);
void drawRightButtonRail(const char* confirm = "RUN", const char* up = "UP", const char* down = "DOWN", const char* back = nullptr, int pressedBtn = -1);
void setStickyActiveButton(int btn);
int getStickyActiveButton();
unsigned long getStickyActiveButtonTime();
void resetStickyActiveButton();
void refreshStickyButtonRail(const char* confirm = "RUN", const char* up = "UP", const char* down = "DOWN", const char* back = nullptr);
void stickyAudioFeedback(uint32_t freq, uint32_t ms);
#endif
