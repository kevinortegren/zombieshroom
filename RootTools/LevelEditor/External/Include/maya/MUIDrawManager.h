#ifndef _MUIDrawManager
#define _MUIDrawManager
//-
// ==========================================================================
// Copyright 2012 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MStatus.h>

// ****************************************************************************
// CLASS FORWARDS
class MPoint;
class MPointArray;
class MColor;
class MColorArray;
class MString;
class THmanipContainer;
class THmanip;
class MVector;
class MVectorArray;
class THstandardContext;
class MUintArray;
class THdrRedraw;

// ****************************************************************************
// NAMESPACE
namespace MHWRender
{
	// ****************************************************************************
	// DECLARATIONS
	//
	//! \ingroup OpenMayaRender
	//! \brief Main interface for drawing simple geometry in Viewport 2.0 and Maya Hardware Renderer 2.0
	/*!
		Simple geometry includes things like lines and text.

		All drawing operations, including setting state like color and font size, must occur between calls to beginDrawable() and endDrawable(). For example, to draw a pair of red lines you would do the following:

		\code
		MColor red(1.0f, 0.0f, 0.0f);

		painter.beginDrawable();
		painter.setColor( red );
		painter.line( MPoint(0, 0, 0), MPoint(1, 1, 1) );
		painter.line( MPoint(0, 0, 0), MPoint(-1, -1, 5) );
		painter.endDrawable();
		\endcode

		The call to endDrawable() resets the draw state, so if you wanted to continue drawing in red later on you would have to repeat the call to setColor(). E.g:

		\code
		painter.beginDrawable();
		painter.setColor( red );
		painter.text( MPoint(0, 0, 4), "Hello, world!" );
		painter.endDrawable();
		\endcode

		Note that draw operations may not take place immediately but instead be queued up for later execution.
	*/

	// ****************************************************************************

	class OPENMAYARENDER_EXPORT MUIDrawManager
	{
	public:

		/*!
			Font size for drawing the text.
		*/
		enum FontSize{
			// Font 'helvetica', size 12, bold, iso8859-1
			kDefaultFontSize, //!< \nop
			// Font 'helvetica', size 9, bold, iso8859-1
			kSmallFontSize //!< \nop
		};

		/*!
			Text alignment.
		*/
		enum TextAlignment {
			// Text aligned to the left of the background box
			kLeft, //!< \nop
			// Text aligned at the middle of the background box
			kCenter, //!< \nop
			// Text aligned to the right of the background box
			kRight //!< \nop
		};

		/*!
			Line style.
		*/
		enum LineStyle {
			// Solid line
			kSolid, //!< \nop
			// Short Dotted line
			kShortDotted,  //!< \nop
			// Short dashed line
			kShortDashed,  //!< \nop
			// Dashed line
			kDashed, //!< \nop
			// Dotted line
			kDotted  //!< \nop
		};

		/*!
			Paint style.
		*/
		enum PaintStyle {
			// Solid
			kFlat, //!< \nop
			// Stippled 
			kStippled,  //!< \nop
			// Shaded with lighting
			kShaded
		};

		/*!
			Primitive.
		*/
		enum Primitive {
			// Point list
			kPoints, //!< \nop
			// Line list
			kLines, //!< \nop
			// Line strip
			kLineStrip, //!< \nop
			// Closed line
			kClosedLine, //!< \nop
			// Triangle list
			kTriangles, //!< \nop
			// Triangle strip
			kTriStrip //!< \nop
		};


	public:

		void beginDrawable();
		void endDrawable();

		void setColor( const MColor& color );
		void setColorIndex( const short index );

		void setPointSize(float value);
		void setLineWidth(float value);
		void setLineStyle(LineStyle style);
		void setLineStyle(unsigned int factor, unsigned short pattern);

		void setPaintStyle(PaintStyle style);

		// Basic primitive drawing methods
		void line( const MPoint& startPoint, const MPoint& endPoint );

		void line2d( const MPoint& startPoint, const MPoint& endPoint );
		
		void point( const MPoint& point );

		void point2d( const MPoint& point );

		void rect( const MPoint& center, const MVector& up, const MVector& normal,
			double scaleX, double scaleY, bool filled = false );
		void rect2d( const MPoint& center, const MVector& up,
			double scaleX, double scaleY, bool filled = false );

		void sphere( const MPoint& center, double radius, bool filled = false );

		void circle( const MPoint& center, const MVector& normal, double radius, 
			bool filled = false );
		void circle2d( const MPoint& center, double radius, 
			bool filled = false );

		void arc( const MPoint& center, const MVector& start, const MVector& end,
			const MVector& normal, double radius, bool filled = false );
		void arc2d( const MPoint& center, const MVector& start, const MVector& end,
			double radius, bool filled = false );

		void mesh( Primitive mode, const MPointArray& position, 
			const MVectorArray* normal = NULL, 
			const MColorArray* color = NULL,
			const MUintArray* index = NULL );

		void mesh2d( Primitive mode, const MPointArray& position, 
			const MColorArray* color = NULL,
			const MUintArray* index = NULL );

		void cone(const MPoint& base, const MVector& direction, double radius, double height, 
			bool filled = false);

		void box (const MPoint& center, const MVector& up, const MVector & right, double scaleX = 1.0, double scaleY = 1.0,
			double scaleZ = 1.0, bool filled = false);

		// Text drawing
		void setFontSize( FontSize fontSize );
		void text(
			const MPoint& position,
			const MString& text,
			TextAlignment alignment = kLeft,
			const int *backgroundSize = NULL,
			const MColor* backgroundColor = NULL,
			bool dynamic = false);
		void text2d(
			const MPoint& position,
			const MString& text,
			TextAlignment alignment = kLeft,
			const int *backgroundSize = NULL,
			const MColor* backgroundColor = NULL,
			bool dynamic = false);

	private:
		MUIDrawManager( void* );
		~MUIDrawManager();

		void* fData;

		friend class ::THmanipContainer;
		friend class ::THmanip;
		friend class ::THstandardContext;
		friend class ::THdrRedraw;
	};

} // namespace MHWRender

#endif /* __cplusplus */
#endif /* _MUIDrawManager */

