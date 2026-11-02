#include "sticky_card_deck.h"

#if defined(STICKY_MONOCHROME)

#include "display.h"
#include "interface.h"
#include "font5x7_rot.h"
#include <Arduino.h>

enum StickyIconType {
    ICON_ANTENNA,
    ICON_DOCUMENT,
    ICON_BOOK,
    ICON_MAP,
    ICON_SD,
    ICON_GEAR,
    ICON_WIFI,
    ICON_CHIP,
    ICON_CLOUD,
    ICON_POWER,
    ICON_GENERIC
};

struct CardInfo {
    String title;
    String subtitle;
    String badge;
    StickyIconType icon;
};

static CardInfo getCardInfo(const MenuOptions &optItem) {
    String name = optItem.name;
    String text = optItem.text;
    String textLower = text;
    textLower.toLowerCase();

    if (name == "STICK" || textLower.indexOf("sticky") >= 0 || textLower.indexOf("finger") >= 0) {
        return {"Sticky Fingers", "Radio Recon, Vault & Mood", "[FLASH SLOT 0]", ICON_ANTENNA};
    }
    if (name == "DRAFT" || textLower.indexOf("draft") >= 0) {
        return {"Draftling", "Distraction-Free Writerdeck", "[FLASH SLOT 1]", ICON_DOCUMENT};
    }
    if (name == "CROSS" || textLower.indexOf("cross") >= 0) {
        return {"CrossMux", "EPUB Reader & KOReader Sync", "[FLASH SLOT 2]", ICON_BOOK};
    }
    if (textLower.indexOf("map") >= 0 || name == "MAP") {
        return {"Sticky Map", "Offline City Map Navigator", "[MICROSD CARD]", ICON_MAP};
    }
    if (name == "SD") {
        return {"MicroSD Library", "Browse Firmwares & Maps on SD", "[STORAGE]", ICON_SD};
    }
    if (name == "CFG") {
        return {"Settings", "Display, Wi-Fi & System Config", "[SYSTEM]", ICON_GEAR};
    }
    if (name == "WUI") {
        return {"Wireless Web Manager", "Upload & Manage via Browser", "[WIFI]", ICON_WIFI};
    }
    if (name == "PMan" || name == "PM") {
        return {"Partition Manager", "Manage Multi-Boot Flash Slots", "[FLASH]", ICON_CHIP};
    }
    if (name == "OTA") {
        return {"Online Installer", "Check & Install OTA Updates", "[NETWORK]", ICON_CLOUD};
    }
    if (name == "OFF") {
        return {"Power Off", "Deep Sleep & Standby Mode", "[POWER]", ICON_POWER};
    }

    String title = text.length() > 0 ? text : name;
    return {title, "Application", "[APP]", ICON_GENERIC};
}

static void drawStickyIcon(StickyIconType type, int x, int y, uint16_t color) {
    uint16_t bg = (color == BLACK) ? WHITE : BLACK;
    int cx = x + 16;
    int cy = y + 16;

    switch (type) {
        case ICON_ANTENNA: {
            tft->drawLine(cx, y + 4, cx, y + 26, color);
            tft->drawLine(cx - 1, y + 4, cx - 1, y + 26, color);
            tft->fillCircle(cx, y + 4, 2, color);
            tft->drawCircle(cx, cy, 6, color);
            tft->drawCircle(cx, cy, 11, color);
            tft->drawCircle(cx, cy, 15, color);
            tft->fillRect(cx - 16, y, 32, 5, bg);
            tft->fillRect(cx - 16, y + 21, 32, 12, bg);
            tft->drawLine(cx - 6, y + 26, cx + 6, y + 26, color);
            break;
        }
        case ICON_DOCUMENT: {
            int px = x + 6, py = y + 4, pw = 20, ph = 24;
            tft->drawRect(px, py, pw, ph, color);
            tft->drawRect(px + 1, py + 1, pw - 2, ph - 2, color);
            tft->drawLine(px + pw - 6, py, px + pw, py + 6, color);
            tft->drawLine(px + 4, py + 7, px + pw - 7, py + 7, color);
            tft->drawLine(px + 4, py + 12, px + pw - 4, py + 12, color);
            tft->drawLine(px + 4, py + 17, px + pw - 5, py + 17, color);
            break;
        }
        case ICON_BOOK: {
            int bx = x + 16, by = y + 6;
            tft->drawLine(bx, by + 2, bx, by + 20, color);
            tft->drawLine(bx - 1, by + 2, bx - 1, by + 20, color);
            tft->drawLine(bx - 1, by + 2, bx - 13, by + 4, color);
            tft->drawLine(bx - 13, by + 4, bx - 13, by + 22, color);
            tft->drawLine(bx - 13, by + 22, bx - 1, by + 20, color);
            tft->drawLine(bx, by + 2, bx + 12, by + 4, color);
            tft->drawLine(bx + 12, by + 4, bx + 12, by + 22, color);
            tft->drawLine(bx + 12, by + 22, bx, by + 20, color);
            tft->drawLine(bx - 10, by + 8, bx - 4, by + 7, color);
            tft->drawLine(bx - 10, by + 12, bx - 4, by + 11, color);
            tft->drawLine(bx - 10, by + 16, bx - 4, by + 15, color);
            tft->drawLine(bx + 4, by + 7, bx + 10, by + 8, color);
            tft->drawLine(bx + 4, by + 11, bx + 10, by + 12, color);
            tft->drawLine(bx + 4, by + 15, bx + 10, by + 16, color);
            break;
        }
        case ICON_MAP: {
            int mx = x + 4, my = y + 5;
            tft->drawLine(mx, my + 4, mx + 8, my, color);
            tft->drawLine(mx + 8, my, mx + 16, my + 4, color);
            tft->drawLine(mx + 16, my + 4, mx + 24, my, color);
            tft->drawLine(mx, my + 22, mx + 8, my + 18, color);
            tft->drawLine(mx + 8, my + 18, mx + 16, my + 22, color);
            tft->drawLine(mx + 16, my + 22, mx + 24, my + 18, color);
            tft->drawLine(mx, my + 4, mx, my + 22, color);
            tft->drawLine(mx + 8, my, mx + 8, my + 18, color);
            tft->drawLine(mx + 16, my + 4, mx + 16, my + 22, color);
            tft->drawLine(mx + 24, my, mx + 24, my + 18, color);
            tft->fillCircle(mx + 12, my + 9, 3, color);
            tft->drawLine(mx + 12, my + 9, mx + 12, my + 14, color);
            break;
        }
        case ICON_SD: {
            int sx = x + 7, sy = y + 4, sw = 18, sh = 24;
            tft->drawRect(sx, sy + 4, sw, sh - 4, color);
            tft->drawLine(sx + 4, sy, sx + sw - 1, sy, color);
            tft->drawLine(sx, sy + 4, sx + 4, sy, color);
            tft->drawLine(sx + sw - 1, sy, sx + sw - 1, sy + sh - 1, color);
            for (int p = 0; p < 4; ++p) {
                tft->fillRect(sx + 3 + p * 3, sy + 3, 2, 4, color);
            }
            break;
        }
        case ICON_GEAR: {
            tft->drawCircle(cx, cy, 9, color);
            tft->drawCircle(cx, cy, 8, color);
            tft->drawCircle(cx, cy, 4, color);
            tft->fillRect(cx - 2, cy - 12, 4, 3, color);
            tft->fillRect(cx - 2, cy + 9, 4, 3, color);
            tft->fillRect(cx - 12, cy - 2, 3, 4, color);
            tft->fillRect(cx + 9, cy - 2, 3, 4, color);
            break;
        }
        case ICON_WIFI: {
            tft->fillCircle(cx, y + 24, 2, color);
            tft->drawCircle(cx, y + 24, 6, color);
            tft->drawCircle(cx, y + 24, 11, color);
            tft->drawCircle(cx, y + 24, 16, color);
            tft->fillRect(cx - 18, y + 25, 36, 10, bg);
            break;
        }
        case ICON_CHIP: {
            int kx = x + 6, ky = y + 6, ksz = 20;
            tft->drawRect(kx, ky, ksz, ksz, color);
            tft->drawRect(kx + 1, ky + 1, ksz - 2, ksz - 2, color);
            for (int p = 0; p < 3; ++p) {
                tft->drawLine(kx - 3, ky + 4 + p * 5, kx, ky + 4 + p * 5, color);
                tft->drawLine(kx + ksz, ky + 4 + p * 5, kx + ksz + 3, ky + 4 + p * 5, color);
            }
            tft->fillCircle(kx + 4, ky + 4, 2, color);
            break;
        }
        case ICON_CLOUD: {
            tft->drawCircle(cx - 5, cy - 1, 6, color);
            tft->drawCircle(cx + 5, cy, 5, color);
            tft->drawCircle(cx + 1, cy - 5, 6, color);
            tft->drawLine(cx - 9, cy + 5, cx + 10, cy + 5, color);
            tft->drawLine(cx, cy + 1, cx, cy + 11, color);
            tft->drawLine(cx - 3, cy + 8, cx, cy + 11, color);
            tft->drawLine(cx + 3, cy + 8, cx, cy + 11, color);
            break;
        }
        case ICON_POWER: {
            tft->drawCircle(cx, cy, 10, color);
            tft->drawCircle(cx, cy, 9, color);
            tft->fillRect(cx - 3, cy - 11, 6, 6, bg);
            tft->drawLine(cx, cy - 12, cx, cy, color);
            tft->drawLine(cx - 1, cy - 12, cx - 1, cy, color);
            break;
        }
        default: {
            int gx = x + 6, gy = y + 6;
            tft->drawRoundRect(gx, gy, 20, 20, 4, color);
            tft->drawRoundRect(gx + 1, gy + 1, 18, 18, 3, color);
            tft->drawLine(gx + 10, gy + 4, gx + 10, gy + 16, color);
            tft->drawLine(gx + 4, gy + 10, gx + 16, gy + 10, color);
            break;
        }
    }
}

// ---------------------------------------------------------------------------
// Rotated 90 Deg Clockwise Text Rendering (Replicating Sticky Fingers Chrome)
// ---------------------------------------------------------------------------

static void drawCharRotated90CW(int x, int y, char c, uint8_t size, uint16_t color) {
    if (c < 32 || c > 126) c = ' ';
    const uint8_t *glyph = font5x7_ascii32_126[c - 32];
    for (int col = 0; col < 5; ++col) {
        uint8_t line = pgm_read_byte(&glyph[col]);
        for (int row = 0; row < 7; ++row) {
            if (line & (1 << row)) {
                if (size == 1) {
                    tft->drawPixel(x + row, y - col, color);
                } else {
                    tft->fillRect(x + row * size, y - (col + 1) * size + 1, size, size, color);
                }
            }
        }
    }
}

static int drawTextRotated90CW(int x, int y, const char *str, uint8_t size, uint16_t color) {
    if (!str) return 0;
    int curY = y;
    int stepY = 6 * size;
    while (*str) {
        drawCharRotated90CW(x, curY, *str, size, color);
        curY -= stepY;
        str++;
    }
    return y - curY;
}

static int getTextWidthRotated(const char *str, uint8_t size) {
    if (!str) return 0;
    int len = strlen(str);
    if (len == 0) return 0;
    return (len - 1) * 6 * size + 5 * size;
}

// ---------------------------------------------------------------------------
// Right Edge Button Rail (Exact Hardware Chassis Alignment)
// ---------------------------------------------------------------------------

void drawRightButtonRail(const char* confirm, const char* up, const char* down, const char* back) {
    constexpr int kSidebarWidth = 54;
    const int railX = tftWidth - kSidebarWidth; // 480 - 54 = 426
    constexpr int pillW = 24;
    const int pillX = railX + (kSidebarWidth - pillW) / 2; // 441
    constexpr int minPillH = 52;
    constexpr int maxPillH = 78;

    // Full vertical divider line at x = 426
    tft->drawFastVLine(railX, 0, 800, BLACK);

    // Physical order on the portrait chassis (top to bottom on right edge):
    // Button 0 (Top, GPIO 4): AI / Power / Confirm (centerY = 35)
    // Button 1 (Middle, GPIO 5): Up / Prev (centerY = 105)
    // Button 2 (Bottom, GPIO 6): Down / Next (centerY = 175)
    const char* labels[3] = {confirm, up, down};
    constexpr int centers[3] = {35, 105, 175};

    for (int i = 0; i < 3; ++i) {
        if (!labels[i] || !labels[i][0]) continue;
        int textH = getTextWidthRotated(labels[i], 2);
        int pillH = textH + 16;
        if (pillH < minPillH) pillH = minPillH;
        if (pillH > maxPillH) pillH = maxPillH;
        int pillY = centers[i] - pillH / 2;

        // Rounded tactile pill matching chassis button
        tft->fillRoundRect(pillX, pillY, pillW, pillH, 4, WHITE);
        tft->drawRoundRect(pillX, pillY, pillW, pillH, 4, BLACK);

        // Center rotated text inside pill
        int charX = pillX + (pillW - 14) / 2; // 14px glyph height in X
        int startY = centers[i] + textH / 2;
        drawTextRotated90CW(charX, startY, labels[i], 2, BLACK);
    }

    // Button 0 (Confirm) is also the physical Power key.
    // Show rotated "HOLD SLEEP" beside it (railX + 2 = 428)
    if (labels[0] && labels[0][0]) {
        constexpr const char* holdHint = "HOLD SLEEP";
        int hintH = getTextWidthRotated(holdHint, 1);
        drawTextRotated90CW(railX + 2, 78 + hintH / 2, holdHint, 1, BLACK);
    }

    // Chord bracket connecting Button 1 (105) and Button 2 (175)
    // Indicates the 2-button chord (Page Up + Page Down together = Back)
    if (back && back[0]) {
        tft->drawFastVLine(railX + 5, 105, 70, BLACK);
        tft->drawFastHLine(railX + 5, 105, 5, BLACK);
        tft->drawFastHLine(railX + 5, 175, 5, BLACK);

        // [ 2 ] badge centered at Y = 140
        tft->fillRoundRect(railX + 1, 129, 13, 23, 4, WHITE);
        tft->drawRoundRect(railX + 1, 129, 13, 23, 4, BLACK);
        tft->setTextSize(1);
        tft->setTextColor(BLACK, WHITE);
        tft->drawString("2", railX + 4, 137);

        // Rotated chord action label below Button 2
        int labelH = getTextWidthRotated(back, 1);
        drawTextRotated90CW(railX + 18, 250 + labelH / 2, back, 1, BLACK);
    }
}

// ---------------------------------------------------------------------------
// Header & Card Rendering
// ---------------------------------------------------------------------------

static void drawHeader() {
    tft->setTextSize(_fm); // Size 3 (24px)
    tft->setTextColor(BLACK, WHITE);
    tft->drawString("STICKY LAUNCHER", 14, 12);

    tft->setTextSize(1);
    tft->setTextColor(BLACK, WHITE);
    tft->drawString("CARD DECK / MULTI-BOOT", 14, 38);

    int bat = getBattery();
    if (bat < 0) bat = 0;
    if (bat > 100) bat = 100;
    String batStr = String(bat) + "%";
    tft->setTextSize(_fp); // Size 2 (16px)
    tft->drawRightString(batStr, 426 - 46, 14, 1);

    // Battery outline
    tft->drawRoundRect(426 - 42, 14, 30, 15, 2, BLACK);
    int fillW = (24 * bat) / 100;
    if (fillW > 0) {
        tft->fillRect(426 - 39, 17, fillW, 9, BLACK);
    }
    tft->fillRect(426 - 12, 18, 2, 7, BLACK); // Tip

    // Header divider line ending at rail (x = 426)
    tft->drawFastHLine(12, 52, 426 - 12, BLACK);
    tft->drawFastHLine(12, 53, 426 - 12, BLACK);
}

static void drawSingleCard(int slotIdx, int itemIdx, const MenuOptions &item, bool isSelected) {
    CardInfo info = getCardInfo(item);
    int x = 12;
    int y = 60 + slotIdx * (92 + 8);
    int w = 402;
    int h = 92;

    if (isSelected) {
        tft->fillRoundRect(x, y, w, h, 6, BLACK);

        // Inverted card contents
        drawStickyIcon(info.icon, x + 12, y + (h - 32) / 2, WHITE);

        tft->setTextSize(_fm);
        tft->setTextColor(WHITE, BLACK);
        tft->drawString(info.title, x + 56, y + 16);

        tft->setTextSize(_fp);
        tft->setTextColor(WHITE, BLACK);
        tft->drawString(info.subtitle, x + 56, y + 46);

        tft->setTextSize(1);
        tft->setTextColor(WHITE, BLACK);
        tft->drawRightString(info.badge, x + w - 16, y + 16, 1);

        // Tactile indicator pointing toward right button rail
        tft->fillTriangle(x + w - 24, y + h / 2 - 8, x + w - 24, y + h / 2 + 8, x + w - 14, y + h / 2, WHITE);
    } else {
        tft->fillRoundRect(x, y, w, h, 6, WHITE);
        tft->drawRoundRect(x, y, w, h, 6, BLACK);

        drawStickyIcon(info.icon, x + 12, y + (h - 32) / 2, BLACK);

        tft->setTextSize(_fm);
        tft->setTextColor(BLACK, WHITE);
        tft->drawString(info.title, x + 56, y + 16);

        tft->setTextSize(_fp);
        tft->setTextColor(BLACK, WHITE);
        tft->drawString(info.subtitle, x + 56, y + 46);

        tft->setTextSize(1);
        tft->setTextColor(BLACK, WHITE);
        tft->drawRightString(info.badge, x + w - 16, y + 16, 1);
    }
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
        drawRightButtonRail("RUN", "UP", "DOWN", "BACK");

        // Scroll indicators centered in card content area (x = 213)
        if (startIdx > 0) {
            tft->setTextSize(1);
            tft->setTextColor(BLACK, WHITE);
            tft->drawCentreString("^ MORE ^", 213, 54, 1);
        }
        if (startIdx + visibleCount < size) {
            tft->setTextSize(1);
            tft->setTextColor(BLACK, WHITE);
            tft->drawCentreString("v MORE v", 213, 756, 1);
        }

        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 12;
            int y = 60 + slot * (92 + 8);
            int w = 402;
            int h = 92;
            opt[itemIdx].setCoords(x, y, w, h);
            drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
        }


    } else {
        // Incremental fast-update of previous and newly selected cards
        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 12;
            int y = 60 + slot * (92 + 8);
            int w = 402;
            int h = 92;
            opt[itemIdx].setCoords(x, y, w, h);

            if (itemIdx == prevIndex || itemIdx == index) {
                drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
            }
        }
    }

}

#endif // STICKY_MONOCHROME
