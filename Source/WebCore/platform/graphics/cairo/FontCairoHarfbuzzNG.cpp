/*
 * Copyright (C) 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 * Copyright (C) 2012 Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "FontCascade.h"

#if USE(CAIRO)

#include "Font.h"
#include "GraphicsContext.h"
#include "HarfBuzzShaper.h"
#include "LayoutRect.h"
#include "Logging.h"
#include "NotImplemented.h"
#include "PlatformContextCairo.h"
#include <cairo.h>

namespace WebCore {

float FontCascade::getGlyphsAndAdvancesForComplexText(const TextRun& run, unsigned from, unsigned to, GlyphBuffer& glyphBuffer, ForTextEmphasisOrNot /* forTextEmphasis */) const
{
    HarfBuzzShaper shaper(this, run);
    if (!shaper.shape(&glyphBuffer, from, to)) {
        LOG_ERROR("Shaper couldn't shape glyphBuffer.");
        return 0;
    }

    if (glyphBuffer.isEmpty())
        return 0;

    return shaper.selectionRect({ }, 0, from, to).x();
}

bool FontCascade::canReturnFallbackFontsForComplexText()
{
    return false;
}

bool FontCascade::canExpandAroundIdeographsInComplexText()
{
    return false;
}

float FontCascade::floatWidthForComplexText(const TextRun& run, HashSet<const Font*>*, GlyphOverflow* glyphOverflow) const
{
    if (glyphOverflow) {
        // FIXME: Calculate the actual values rather than just the font's ascent and descent
        glyphOverflow->top = glyphOverflow->computeBounds ? fontMetrics().ascent() : 0;
        glyphOverflow->bottom = glyphOverflow->computeBounds ? fontMetrics().descent() : 0;
        glyphOverflow->left = 0;
        glyphOverflow->right = 0;
    }
    HarfBuzzShaper shaper(this, run);
    if (shaper.shape())
        return shaper.totalWidth();
    LOG_ERROR("Shaper couldn't shape text run.");
    return 0;
}

int FontCascade::offsetForPositionForComplexText(const TextRun& run, float x, bool includePartialGlyphs) const
{
    HarfBuzzShaper shaper(this, run);
    if (shaper.shape())
        return shaper.offsetForPosition(x, includePartialGlyphs);
    LOG_ERROR("Shaper couldn't shape text run.");
    return 0;
}

void FontCascade::adjustSelectionRectForComplexText(const TextRun& run, LayoutRect& selectionRect, unsigned from, unsigned to) const
{
    HarfBuzzShaper shaper(this, run);
    GlyphBuffer glyphBuffer;
    if (!shaper.shape(&glyphBuffer, from, to)) {
        LOG_ERROR("Shaper couldn't shape text run.");
        return;
    }

    // FIXME: This should mimic Mac port.
    selectionRect = LayoutRect(shaper.selectionRect(selectionRect.location(), selectionRect.height().toInt(), from, to));
}

} // namespace WebCore

#endif // USE(CAIRO)
