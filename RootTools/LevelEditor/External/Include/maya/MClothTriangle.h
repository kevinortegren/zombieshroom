#ifndef _MClothTriangle
#define _MClothTriangle
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
// ****************************************************************************
//
// CLASS:    MClothTriangle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES
#include <maya/MStatus.h>


// MayaCloth Export
#ifdef _WIN32
#	ifndef MAYACLOTH_EXPORT
#		ifdef MAYACLOTH_DLL
#			define MAYACLOTH_EXPORT _declspec( dllexport )
#		else
#			define MAYACLOTH_EXPORT _declspec( dllimport )
#		endif // MAYACLOTH_DLL
#	endif // MAYACLOTH_EXPORT
#elif defined(AW_HIDE_SYMBOLS) && defined(MAYACLOTH_DLL)
#	ifndef MAYACLOTH_EXPORT
#		define MAYACLOTH_EXPORT __attribute__ ((visibility("default")))
#	endif
#else
#	ifndef MAYACLOTH_EXPORT
#		define MAYACLOTH_EXPORT
#	endif
#endif // _WIN32

// ****************************************************************************
// DECLARATIONS

class MPoint;
class MDoubleArray;
class MVector;
class MClothEdge;
class MClothParticle;
class MClothMaterial;

// ****************************************************************************
// CLASS DECLARATION (MClothTriangle)

/*!
\internal
\ingroup cloth
\brief Interface for cloth triangle.

This class provides interface for cloth triangle.
Cloth solver writers should derive their triangle class
from this class. Common convenience methods are available.
*/
class MAYACLOTH_EXPORT MClothTriangle
{
public:
	MClothTriangle();
	virtual ~MClothTriangle();

	virtual MClothEdge* getEdge( unsigned int index,
								 MStatus* ReturnStatus = NULL ) const = 0;
	virtual MClothParticle* getParticle( unsigned int index,
										 MStatus* ReturnStatus = NULL ) const = 0;
	virtual MClothMaterial* getMaterial( MStatus* ReturnStatus = NULL ) const = 0;

	virtual MStatus getUV( int vertexIndex,
						   double& u,
						   double& v ) const = 0;
	virtual MStatus getUVs( MDoubleArray& uArray,
							MDoubleArray& vArray ) const = 0;

	virtual bool contains( MClothParticle *n ) const;
	virtual bool equals( MClothTriangle *triangle ) const;

protected:
	// No protected members

private:
	// No private members
};

#endif /* __cplusplus */
#endif /* _MClothTriangle */
