#pragma once

// Rotated 5x7 ASCII font table for the right-rail button labels.
//
// The glyph data now lives in the shared StickyType library (lib/StickyType) so
// sticky-ruby and the launcher render identical letterforms from one source.
// This header keeps the historical `font5x7_ascii32_126` name in scope for
// sticky_card_deck.cpp's local rotated-draw helpers.
#include <StickyType.h>

using stickytype::font5x7_ascii32_126;
