#ifndef _MMaterial
#define _MMaterial
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
// CLASS:    MMaterial
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/M3dView.h>

// ****************************************************************************
// DECLARATIONS

class MDagPath;
class MVector;
class MDrawData;
class MPxHwShaderNode;
class MImage;

// ****************************************************************************
// CLASS DECLARATION (MMaterial)

//! \ingroup OpenMayaUI
//! \brief Hardware shading material class used in MPxSurfaceShapeUI 
/*!
This class is used in the draw functions of user defined shapes
(see MPxSurfaceShapeUI) for setting up and querying materials
used in shaded mode drawing.
*/
class OPENMAYAUI_EXPORT MMaterial
{
public:
	MMaterial();
	MMaterial( const MMaterial& in );
	MMaterial( const MObject&, MStatus* ReturnStatus=NULL );
	~MMaterial();

public:
	// Evaluation methods.
	MStatus		evaluateMaterial(M3dView&, const MDagPath& );
	MStatus		evaluateShininess();
	MStatus		evaluateDiffuse();
	MStatus		evaluateEmission();
	MStatus		evaluateSpecular();
	MStatus		evaluateTexture( MDrawData & data );

	bool		materialIsTextured() const;

	// Draw methods.
    // Can call these after calling correct evaluate method above.
	MStatus		setMaterial(const MDagPath&, bool hasTransparency);
	MStatus		getShininess( float & );
	MStatus		getDiffuse( MColor & );
	MStatus		getEmission( MColor & );
	MStatus		getSpecular( MColor & );
    MStatus		getHasTransparency( bool & );
	MStatus		getTextureTransformation(const MDrawData& data,
                                         MMatrix& texXform);
	MStatus		getTextureTransformation(const MDrawData& data,
                                         float& rotateUV,
                                         float& scaleU,
										 float& scaleV,
										 float& translateU,
										 float& translateV,
										 float& rotateFrame);

	// This method sets the texture so that it can be used in
	// the specified view.
	void		applyTexture( M3dView&, const MDrawData& );

	//! Channels which can be queried. Only Lambert, Phong,
	//! PhongE, Blinn, and Anisotropic are supported
	typedef enum
	{
		kColor = 0,			//!< nop
		kTransparency,		//!< \nop
		kAmbientColor,		//!< \nop
		kIncandescence,		//!< \nop
		kBumpMap,			//!< \nop
		kDiffuse,			//!< \nop
		kTransluscence,		//!< \nop
		kRoughness,			//!< PhongE only
		kHighlightSize,		//!< PhongE only
		kWhiteness,			//!< PhongE only
		kCosinePower,		//!< Phong only
		kEccentricity,		//!< Blinn only
		kSpecularRollOff,	//!< Blinn only
		kSpecularColor,		//!< Blinn and Phong(E) only
		kReflectivity,		//!< Blinn and Phong(E) only
		kReflectedColor		//!< Blinn and Phong(E) only

	} MtextureChannel;
	MStatus textureImage( MImage &image, MColor &color,
						MtextureChannel chan, bool &mapped,
						MDagPath &dagPath,
						int xRes = -1, int yRes = -1);

	MPxHwShaderNode *	getHwShaderNode( MStatus * ReturnStatus = NULL );

	static MMaterial	defaultMaterial();

	MObject				shadingEngine(); 

	static const char*	className();

	//	Obsolete: This method was retained to provide backwards
	//	compatability.
	MStatus		evaluateTextureTransformation();

	//	Obsolete: This method was retained to provide backwards
	//	compatability.
	MStatus		getTextureTransformation(float& scaleU,
										 float& scaleV,
										 float& translateU,
										 float& translateV,
										 float& rotate);
protected:
// No protected members

private:
    MMaterial( void* in );

	void*	 fMaterial;
};

#endif /* __cplusplus */
#endif /* _MMaterial */
