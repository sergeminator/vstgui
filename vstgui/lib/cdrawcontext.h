//-----------------------------------------------------------------------------
// VST Plug-Ins SDK
// VSTGUI: Graphical User Interface Framework for VST plugins : 
//
// Version 4.0
//
//-----------------------------------------------------------------------------
// VSTGUI LICENSE
// (c) 2010, Steinberg Media Technologies, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation 
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this 
//     software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A  PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#ifndef __cdrawcontext__
#define __cdrawcontext__

#include "vstguibase.h"
#include "cpoint.h"
#include "crect.h"
#include "cfont.h"
#include "ccolor.h"

namespace VSTGUI {

//-----------
// @brief Line Style
//-----------
enum CLineStyle
{
	kLineSolid = 0,
	kLineOnOffDash
};

//-----------
// @brief Draw Mode
//-----------
enum CDrawMode
{
	kCopyMode = 0,					///< non antialiased drawing
	kAntialias						///< antialised drawing
};

//----------------------------
// @brief Text Alignment (Horizontal)
//----------------------------
enum CHoriTxtAlign
{
	kLeftText = 0,
	kCenterText,
	kRightText
};

//----------------------------
// @brief Draw Style
//----------------------------
enum CDrawStyle
{
	kDrawStroked = 0,
	kDrawFilled,
	kDrawFilledAndStroked
};

} // namespace

#include <stack>

namespace VSTGUI {
class CBitmap;

//-----------------------------------------------------------------------------
// CDrawContext Declaration
//! @brief A drawing context encapsulates the drawing context of the underlying OS
//-----------------------------------------------------------------------------
class CDrawContext : public CBaseObject
{
public:
	//-----------------------------------------------------------------------------
	/// @name Draw primitives
	//-----------------------------------------------------------------------------
	//@{
	virtual void moveTo (const CPoint &point);	///< move line position to point
	virtual void lineTo (const CPoint &point) = 0;	///< draw a line from current position to point
	void getLoc (CPoint &where) const { where = currentState.penLoc; }
	virtual void drawLines (const CPoint* points, const long& numberOfLines) = 0;	///< draw multiple lines at once
	virtual void drawPolygon (const CPoint *pPoints, long numberOfPoints, const CDrawStyle drawStyle = kDrawStroked) = 0; ///< draw a polygon
	virtual void drawRect (const CRect &rect, const CDrawStyle drawStyle = kDrawStroked) = 0;	///< draw a rect
	virtual void drawArc (const CRect &rect, const float startAngle1, const float endAngle2, const CDrawStyle drawStyle = kDrawStroked) = 0;	///< draw an arc, angles are in degree
	virtual void drawEllipse (const CRect &rect, const CDrawStyle drawStyle = kDrawStroked) = 0;	///< draw an ellipse
	virtual void drawPoint (const CPoint &point, CColor color) = 0;	///< draw a point
	virtual void drawBitmap (CBitmap* bitmap, const CRect& dest, const CPoint& offset = CPoint (0, 0), float alpha = 1.f) = 0; ///< don't call directly, please use CBitmap::draw instead
	//@}

	//-----------------------------------------------------------------------------
	/// @name Line Mode
	//-----------------------------------------------------------------------------
	//@{
	virtual void setLineStyle (CLineStyle style);	///< set the current line style
	CLineStyle getLineStyle () const { return currentState.lineStyle; }	///< get the current line style

	virtual void setLineWidth (CCoord width);	///< set the current line width
	CCoord getLineWidth () const { return currentState.frameWidth; }	///< get the current line width
	//@}

	//-----------------------------------------------------------------------------
	/// @name Draw Mode
	//-----------------------------------------------------------------------------
	//@{
	virtual void setDrawMode (CDrawMode mode);	///< set the current draw mode, see CDrawMode
	CDrawMode getDrawMode () const { return currentState.drawMode; }	///< get the current draw mode, see CDrawMode
	//@}

	//-----------------------------------------------------------------------------
	/// @name Clipping
	//-----------------------------------------------------------------------------
	//@{
	virtual void setClipRect (const CRect &clip);	///< set the current clip
	CRect& getClipRect (CRect &clip) const;///< get the current clip
	virtual void resetClipRect ();	///< reset the clip to the default state
	//@}

	//-----------------------------------------------------------------------------
	/// @name Color
	//-----------------------------------------------------------------------------
	//@{
	virtual void setFillColor  (const CColor color);	///< set current fill color
	CColor getFillColor () const { return currentState.fillColor; }	///< get current fill color
	virtual void setFrameColor (const CColor color);	///< set current stroke color
	CColor getFrameColor () const { return currentState.frameColor; }///< get current stroke color
	//@}

	//-----------------------------------------------------------------------------
	/// @name Font
	//-----------------------------------------------------------------------------
	//@{
	virtual void setFontColor (const CColor color);	///< set current font color
	CColor getFontColor () const { return currentState.fontColor; }	///< get current font color
	virtual void setFont (const CFontRef font, const long& size = 0, const long& style = -1); ///< set current font
	const CFontRef&  getFont () const { return currentState.font; }	///< get current font
	//@}
	
	//-----------------------------------------------------------------------------
	/// @name Text
	//-----------------------------------------------------------------------------
	//@{
	CCoord getStringWidth (const char* pStr);	///< get the width of an UTF-8 encoded string
	void drawString (const char* string, const CRect& _rect, const CHoriTxtAlign hAlign = kCenterText, bool antialias = true);	///< draw an UTF-8 encoded string
	void drawString (const char* string, const CPoint& _point, bool antialias = true);	///< draw an UTF-8 encoded string
	//@}
	
	//-----------------------------------------------------------------------------
	/// @name Global Alpha State
	//-----------------------------------------------------------------------------
	//@{
	virtual void setGlobalAlpha (float newAlpha);	///< sets the global alpha value[0..1]
	float getGlobalAlpha () const { return currentState.globalAlpha; }	///< get current global alpha value
	//@}
	
	//-----------------------------------------------------------------------------
	/// @name Global State Stack
	//-----------------------------------------------------------------------------
	//@{
	virtual void saveGlobalState ();
	virtual void restoreGlobalState ();
	//@}

	//-----------------------------------------------------------------------------
	/// @name Offset Transformation
	//-----------------------------------------------------------------------------
	//@{
	virtual void setOffset (const CPoint& offset);
	const CPoint& getOffset () const { return currentState.offset; }
	//@}

	CLASS_METHODS_NOCOPY(CDrawContext, CBaseObject)
protected:
	CDrawContext (const CRect& surfaceRect);
	~CDrawContext ();

	virtual void init ();

	/// @cond ignore
	struct CDrawContextState
	{
		CFontRef font;
		CColor frameColor;
		CColor fillColor;
		CColor fontColor;
		CCoord frameWidth;
		CPoint offset;
		CPoint penLoc;
		CRect clipRect;
		CLineStyle lineStyle;
		CDrawMode drawMode;
		float globalAlpha;
	};
	/// @endcond

	CRect surfaceRect;

	CDrawContextState currentState;
	std::stack<CDrawContextState*> globalStatesStack;
};

} // namespace

#endif
