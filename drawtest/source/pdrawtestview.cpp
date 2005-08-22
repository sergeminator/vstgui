//-------------------------------------------------------------------------------------------------------
//  VSTGUI Test View plugin
//  Copyright (c) 2004 Arne Scheffler. All rights reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//-------------------------------------------------------------------------------------------------------

#ifndef __pdrawtestview__
#include "pdrawtestview.h"
#endif


CDrawTestView::CDrawTestView (const CRect& size)
: CView (size)
, value (0)
{
}

static inline void testDrawRect (CDrawContext *pContext, CRect r)
{
	int offset = pContext->getLineWidth ();
	int i = 0;
	while (r.width () > 1 && r.height () > 1)
	{
		if (i++ % 2)
			pContext->setFrameColor (kBlackCColor);
		else
			pContext->setFrameColor (kRedCColor);
		r.inset (offset, offset);
		pContext->drawRect (r);
	}
}

static inline void testFillRect (CDrawContext *pContext, CRect r)
{
	int i = 0;
	while (r.width () > 1 && r.height () > 1)
	{
		if (i++ % 2)
			pContext->setFillColor (kYellowCColor);
		else
			pContext->setFillColor (kCyanCColor);
		r.inset (1,1);
		pContext->fillRect (r);
	}
}

static inline void testDrawLine (CDrawContext *pContext, CRect r)
{
	int offset = pContext->getLineWidth ();
	int i = 0;
	while (r.width () > 1 && r.height () > 1)
	{
		if (i++ % 2)
			pContext->setFrameColor (kBlueCColor);
		else
			pContext->setFrameColor (kGreenCColor);
		r.inset (offset,offset);
		pContext->moveTo (CPoint (r.left, r.top));
		pContext->lineTo (CPoint (r.right, r.top));
		pContext->lineTo (CPoint (r.right, r.bottom));
		pContext->lineTo (CPoint (r.left, r.bottom));
		pContext->lineTo (CPoint (r.left, r.top));
	}
}

static inline void clearRect (CDrawContext* pContext, const CRect& r)
{
	pContext->setFillColor (kWhiteCColor);
	pContext->fillRect (r);
}

#define kMaxValue	5

void CDrawTestView::draw (CDrawContext *pContext)
{
	CRect r (size);
	clearRect (pContext, r);

	switch (value)
	{
		case 0:
		{
			pContext->setLineWidth (1);
			testDrawRect (pContext, size);
			break;
		}
		case 1:
		{
			pContext->setLineWidth (1);
			testDrawLine (pContext, size);
			break;
		}
		case 2:
		{
			testFillRect (pContext, size);
			break;
		}
		case 3:
		{
			pContext->setLineWidth (2);
			testDrawLine (pContext, size);
			break;
		}
		case 4:
		{
			pContext->setDrawMode (kAntialias);
			pContext->setLineWidth (2);
			testDrawLine (pContext, size);
			break;
		}
		case 5:
		{
			pContext->setDrawMode (kAntialias);
			pContext->setLineWidth (1);
			testDrawLine (pContext, size);
			break;
		}
	}
	
	char str[256];
	sprintf (str, "DrawMode : %d", value);
	pContext->setFont (kSystemFont);
	pContext->setFontColor (kWhiteCColor);
	pContext->drawString (str, r);
	pContext->setFontColor (kBlackCColor);
	r.offset (-1, -1);
	pContext->drawString (str, r);
}

void CDrawTestView::mouse (CDrawContext* pContext, CPoint& where, long buttons)
{
	if (buttons & kLButton)
	{
		value++;
		if (value > kMaxValue)
			value = 0;
	}
	setDirty (true);
}

