#include "sticky_card_deck.h"

#if defined(STICKY_MONOCHROME)

#include "display.h"
#include "interface.h"
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

static void drawHeader() {
    tft->setTextSize(_fm); // Size 3 (24px)
    tft->setTextColor(BLACK, WHITE);
    tft->drawString("STICKY MULTI-BOOT", 16, 12);

    tft->setTextSize(1);
    tft->setTextColor(BLACK, WHITE);
    tft->drawString("NAV: 3 BUTTONS ON RIGHT [^ UP] [v DOWN] [* SELECT]", 16, 38);

    int bat = getBattery();
    if (bat < 0) bat = 0;
    if (bat > 100) bat = 100;
    String batStr = String(bat) + "%";
    tft->setTextSize(_fp); // Size 2 (16px)
    tft->drawRightString(batStr, tftWidth - 52, 14, 1);

    // Battery outline
    tft->drawRoundRect(tftWidth - 46, 14, 30, 15, 2, BLACK);
    int fillW = (24 * bat) / 100;
    if (fillW > 0) {
        tft->fillRect(tftWidth - 43, 17, fillW, 9, BLACK);
    }
    tft->fillRect(tftWidth - 16, 18, 2, 7, BLACK); // Tip

    // Header divider line
    tft->drawLine(16, 52, tftWidth - 16, 52, BLACK);
    tft->drawLine(16, 53, tftWidth - 16, 53, BLACK);
}

static void drawSingleCard(int slotIdx, int itemIdx, const MenuOptions &item, bool isSelected) {
    CardInfo info = getCardInfo(item);
    int x = 16;
    int y = 60 + slotIdx * (92 + 8);
    int w = tftWidth - 32;
    int h = 92;

    if (isSelected) {
        tft->fillRoundRect(x, y, w, h, 6, BLACK);

        int iconX = x + 14;
        int iconY = y + (h - 32) / 2;
        drawStickyIcon(info.icon, iconX, iconY, WHITE);

        tft->setTextSize(_fm);
        tft->setTextColor(WHITE, BLACK);
        tft->drawString(info.title, x + 56, y + 16);

        tft->setTextSize(_fp);
        tft->setTextColor(WHITE, BLACK);
        tft->drawString(info.subtitle, x + 56, y + 46);

        // Prominent action badge on right of selected card
        int btnW = 124;
        int btnH = 32;
        int btnX = x + w - btnW - 14;
        int btnY = y + (h - btnH) / 2;
        tft->fillRoundRect(btnX, btnY, btnW, btnH, 4, WHITE);
        tft->drawRoundRect(btnX, btnY, btnW, btnH, 4, BLACK);
        tft->setTextSize(2);
        tft->setTextColor(BLACK, WHITE);
        tft->drawCentreString("[*] SELECT", btnX + btnW / 2, btnY + 8, 1);

        tft->setTextSize(1);
        tft->setTextColor(WHITE, BLACK);
        tft->drawRightString(info.badge, btnX - 10, y + 16, 1);
    } else {
        tft->fillRoundRect(x, y, w, h, 6, WHITE);
        tft->drawRoundRect(x, y, w, h, 6, BLACK);
        tft->drawRoundRect(x + 1, y + 1, w - 2, h - 2, 5, BLACK);

        int iconX = x + 14;
        int iconY = y + (h - 32) / 2;
        drawStickyIcon(info.icon, iconX, iconY, BLACK);

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

    const int maxVisible = 6;
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

        // Scroll indicators
        if (startIdx > 0) {
            tft->setTextSize(1);
            tft->setTextColor(BLACK, WHITE);
            tft->drawCentreString("^ MORE ^", tftWidth / 2, 54, 1);
        }
        if (startIdx + visibleCount < size) {
            tft->setTextSize(1);
            tft->setTextColor(BLACK, WHITE);
            tft->drawCentreString("v MORE v", tftWidth / 2, 706, 1);
        }

        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 16;
            int y = 60 + slot * (92 + 8);
            int w = tftWidth - 32;
            int h = 92;
            opt[itemIdx].setCoords(x, y, w, h);
            drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
        }

        // Dedicated Hardware Navigation Guide Card (anchored above footer)
        int legendY = 662;
        int legendH = 104;
        tft->fillRoundRect(16, legendY, tftWidth - 32, legendH, 6, WHITE);
        tft->drawRoundRect(16, legendY, tftWidth - 32, legendH, 6, BLACK);
        tft->drawRoundRect(17, legendY + 1, tftWidth - 34, legendH - 2, 5, BLACK);

        // Header strip inside box
        tft->fillRect(18, legendY + 2, tftWidth - 36, 24, BLACK);
        tft->setTextSize(1);
        tft->setTextColor(WHITE, BLACK);
        tft->drawCentreString("CONTROLS: 3 HARDWARE BUTTONS ON RIGHT EDGE", tftWidth / 2, legendY + 8, 1);

        // Button mappings (large, crisp, unmistakable)
        tft->setTextSize(2);
        tft->setTextColor(BLACK, WHITE);
        tft->drawString("[^] TOP BTN  : UP", 28, legendY + 34);
        tft->drawString("[v] MID BTN  : DOWN", 28, legendY + 56);
        tft->drawString("[*] BOT BTN  : SELECT / RUN", 28, legendY + 78);

        // Tips on right side of box
        tft->setTextSize(1);
        tft->drawRightString("HOLD [^+v] TOGETHER: BACK / ESC", tftWidth - 28, legendY + 40, 1);
        tft->drawRightString("TOUCH: TAP CARDS DIRECTLY", tftWidth - 28, legendY + 66, 1);

        // Bottom divider before touch footer
        tft->drawLine(16, tftHeight - 2, tftWidth - 16, tftHeight - 2, BLACK);
        TouchFooter();
    } else {
        // Incremental fast-update of previous and newly selected cards
        for (int slot = 0; slot < visibleCount; ++slot) {
            int itemIdx = startIdx + slot;
            int x = 16;
            int y = 60 + slot * (92 + 8);
            int w = tftWidth - 32;
            int h = 92;
            opt[itemIdx].setCoords(x, y, w, h);

            if (itemIdx == prevIndex || itemIdx == index) {
                drawSingleCard(slot, itemIdx, opt[itemIdx], (itemIdx == index));
            }
        }
    }

    tft->display(false);
}

#endif // STICKY_MONOCHROME
