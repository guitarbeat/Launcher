#include "sticky_card_deck.h"

#if defined(STICKY_MONOCHROME)

#include "display.h"
#include "interface.h"
#include "idf/launcher_platform.h"
#include <StickyUI.h>
#include <cmath>
#include <algorithm>
#include <Arduino.h>

using stickyui::Icon;

// Adapts the GxEPD2 tft to StickyUI's two-call canvas; all chrome comes from lib/StickyUI.
struct TftCanvas {
    void fillRect(int x, int y, int w, int h, bool black) { tft->fillRect(x, y, w, h, black ? BLACK : WHITE); }
    void pixel(int x, int y, bool black) { tft->drawPixel(x, y, black ? BLACK : WHITE); }
};

struct CardInfo {
    String title;
    String subtitle;
    String badge;
    Icon icon;
};

static CardInfo getCardInfo(const MenuOptions &optItem) {
    String name = optItem.name;
    String text = optItem.text;
    String textLower = text;
    textLower.toLowerCase();

    if (name == "STICK" || textLower.indexOf("sticky") >= 0 || textLower.indexOf("finger") >= 0) {
        return {"Sticky Fingers", "RF Recon & Learning Companion", "SLOT 0", Icon::Antenna};
    }
    if (name == "DRAFT" || textLower.indexOf("draft") >= 0) {
        return {"Draftling", "Distraction-Free Writerdeck", "SLOT 1", Icon::Document};
    }
    if (name == "CROSS" || textLower.indexOf("cross") >= 0) {
        return {"CrossMux", "EPUB Reader & Document Sync", "SLOT 2", Icon::Book};
    }
    if (textLower.indexOf("map") >= 0 || name == "MAP") {
        return {"Sticky Map", "Offline City Navigator", "SD CARD", Icon::Map};
    }
    if (name == "SD") {
        return {"MicroSD Library", "Browse SD Images & ROMs", "STORAGE", Icon::Sd};
    }
    if (name == "CFG") {
        return {"Settings", "Display, Wi-Fi & System", "SYSTEM", Icon::Gear};
    }
    if (name == "WUI") {
        return {"Wireless Web Manager", "Web Flash & File Server", "WIFI", Icon::Wifi};
    }
    if (name == "PMan" || name == "PM") {
        return {"Partition Manager", "Configure Boot Slots", "FLASH", Icon::Chip};
    }
    if (name == "OTA") {
        return {"Online Installer", "Over-The-Air Releases", "NETWORK", Icon::Cloud};
    }
    if (name == "OFF") {
        return {"Power Off", "Deep Sleep & Standby", "STANDBY", Icon::Power};
    }

    String title = text.length() > 0 ? text : name;
    return {title, "Application", "APP", Icon::Generic};
}

// ---------------------------------------------------------------------------
// Dynamic Notches (Exact Hardware Chassis Alignment)
// ---------------------------------------------------------------------------

static int s_activeButton = -1;
static unsigned long s_activeButtonTime = 0;

void setStickyActiveButton(int btn) {
    s_activeButton = btn;
    s_activeButtonTime = launcherMillis();
}

int getStickyActiveButton() {
    return s_activeButton;
}

unsigned long getStickyActiveButtonTime() {
    return s_activeButtonTime;
}

void resetStickyActiveButton() {
    s_activeButton = -1;
}

void stickyAudioFeedback(uint32_t freq, uint32_t ms) {
#if defined(STICKY_MONOCHROME)
    if (!soundEnabled) return;
    constexpr uint8_t BUZZER_PIN = 48;
    ledcAttach(BUZZER_PIN, 2000, 10);
    ledcWriteTone(BUZZER_PIN, freq);
    delay(ms);
    ledcWriteTone(BUZZER_PIN, 0);
    ledcWrite(BUZZER_PIN, 0);
    ledcDetach(BUZZER_PIN);
#else
    (void)freq;
    (void)ms;
#endif
}

void drawRightButtonRail(const char* confirm, const char* up, const char* down, const char* back, int pressedBtn) {
    if (pressedBtn < 0) pressedBtn = s_activeButton;
    TftCanvas c;
    stickyui::rail(c, confirm, up, down, back, pressedBtn);
}

void refreshStickyButtonRail(const char* confirm, const char* up, const char* down, const char* back) {
    drawRightButtonRail(confirm, up, down, back, s_activeButton);
    if (tft) {
        tft->display(false);
    }
}

// Header & Card Rendering
// ---------------------------------------------------------------------------

static void drawHeader() {
    TftCanvas c;
    stickyui::header(c, "STICKY LAUNCHER", "MULTI-BOOT CARD DECK", getBattery());
}

static void drawSingleCard(int slotIdx, int itemIdx, const MenuOptions &item, bool isSelected) {
    (void)itemIdx;
    CardInfo info = getCardInfo(item);
    TftCanvas c;
    stickyui::card(c, stickyui::kMarginX, stickyui::cardY(slotIdx), stickyui::kContentW, stickyui::kCardH, isSelected,
                   info.icon, info.title.c_str(), info.subtitle.c_str(), info.badge.c_str());
}

void drawStickyCardDeckMenu(std::vector<MenuOptions> &opt, int index, bool forceFullRedraw) {
    static int lastIndex = -1;
    static int lastStartIdx = -1;

    int size = static_cast<int>(opt.size());
    if (size < 1) {
        displayRedStripe("No options available");
        return;
    }

    const int maxVisible = 7;
    int visibleCount = (size < maxVisible) ? size : maxVisible;
    int startIdx = 0;
    if (size > maxVisible) {
        if (index >= maxVisible) {
            startIdx = index - maxVisible + 1;
        }
        if (startIdx + visibleCount > size) {
            startIdx = size - visibleCount;
        }
        if (startIdx < 0) startIdx = 0;
    }

    bool fullRedraw = forceFullRedraw || lastIndex < 0 || lastStartIdx != startIdx ||
                      lastIndex >= size;
    int prevIndex = lastIndex;
    lastIndex = index;
    lastStartIdx = startIdx;

    for (int i = 0; i < size; ++i) opt[i].resetCoords();

    if (fullRedraw) {
        tft->fillScreen(WHITE);
        drawHeader();
        drawRightButtonRail("RUN", "UP", "DOWN", nullptr, s_activeButton);

        // Scroll indicators centered in card content area (x = 224)
        if (startIdx > 0) {
            TftCanvas c;
            stickyui::textCentre(c, 224, 52, "^ MORE ^", stickyui::kSmallSize, true, true);
        }
        if (startIdx + visibleCount < size) {
            TftCanvas c;
            stickyui::textCentre(c, 224, 730, "v MORE v", stickyui::kSmallSize, true, true);
        }

        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 14;
            int y = 58 + slot * (90 + 6);
            int w = 420;
            int h = 90;
            opt[itemIdx].setCoords(x, y, w, h);
            drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
        }

    } else {
        // Incremental fast-update of previous and newly selected cards
        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 14;
            int y = 58 + slot * (90 + 6);
            int w = 420;
            int h = 90;
            opt[itemIdx].setCoords(x, y, w, h);

            if (itemIdx == prevIndex || itemIdx == index) {
                drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
            }
        }
        drawRightButtonRail("RUN", "UP", "DOWN", nullptr, s_activeButton);
    }
}

#endif // STICKY_MONOCHROME
