
#ifndef _MPxSurfaceShapeUI
#define _MPxSurfaceShapeUI
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MPxSurfaceShapeUI
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/M3dView.h>
#include <maya/MBoundingBox.h>
#include <maya/M3dView.h>
#include <maya/MSelectInfo.h>
#include <maya/MDrawRequest.h>
#include <maya/MDrawRequestQueue.h>

// ****************************************************************************
// DECLARATIONS

class MSelectionList;
class MPointArray;
class MPxSurfaceShape;
class MDrawData;
class MMaterial;
class MMaterialArray;
class MTextureEditorDrawInfo;
class MObjectArray;

// ****************************************************************************
// CLASS DECLARATION (MPxNode)

//! \ingroup OpenMayaUI MPx
//! \brief drawing and selection for user defined shapes
/*!
The base class for the UI portion of all user defined shapes.
*/
class OPENMAYAUI_EXPORT MPxSurfaceShapeUI
{
public:
	MPxSurfaceShapeUI();
	virtual ~MPxSurfaceShapeUI();

	//! Selection modes for UVs
    enum UVSelectionType {
		kSelectMeshUVs,		//!< The UV selection type is UVs.
		kSelectMeshVerts,	//!< The UV selection type is vertices.
		kSelectMeshFaces,	//!< The UV selection type is faces.
		kSelectMeshEdges	//!< The UV selection type is edges.
    };

	void					getDrawData( void * geom, MDrawData & );

	// ************************************************************************
	// Methods to overload

	virtual void			getDrawRequests( const MDrawInfo &,
											 bool objectAndActiveOnly,
											 MDrawRequestQueue & requests );
	virtual void		    draw( const MDrawRequest &, M3dView & view ) const;

	virtual bool		    select( MSelectInfo &selectInfo,
							    	MSelectionList &selectionList,
							    	MPointArray &worldSpaceSelectPts ) const;
	virtual bool			snap( MSelectInfo & snapInfo ) const;

	virtual bool			canDrawUV() const;
	virtual void			drawUV(M3dView &view, const MTextureEditorDrawInfo &info) const;

	virtual bool			selectUV( M3dView &view,
									  UVSelectionType selType,
									  int xmin, int ymin,
									  int xmax, int ymax,
									  bool singleSelect,
									  MSelectionList &selList ) const;

	// ************************************************************************

	MPxSurfaceShape*		surfaceShape() const;
	MMaterial 				material( MDagPath & path ) const;
	void					materials( MDagPath & path, MObjectArray & componentFilter, MMaterialArray & materials, MObjectArray * componentSet = NULL) const;

	static MPxSurfaceShapeUI *	surfaceShapeUI( const MDagPath & path, MStatus *ReturnStatus = NULL ); 

	static const char*	    className();

private:
	friend class MDrawRequest;
	friend class MDrawInfo;
	void * instance;
};

#endif /* __cplusplus */
#endif /* _MPxSurfaceShapeUI */
