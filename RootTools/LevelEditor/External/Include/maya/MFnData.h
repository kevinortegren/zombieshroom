#ifndef _MFnData
#define _MFnData
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
// CLASS:    MFnData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>

// ****************************************************************************
// CLASS DECLARATION (MFnData)

//! \ingroup MFn OpenMaya
//! Parent class for dependency graph data function sets.
/*!
   MFnData is the parent class for all dependency graph data function
   sets.  Conceptually, data objects are what flow through the connections
   in the dependency graph.

   Each node in the dependency graph has a data block associated with it.
   The data block holds the data objects for all of the node's attributes
   (see MDataBlock).  The data block is only available during the compute
   method of a node.  A data handle (MDataHandle) can be created to access
   a particular attribute's data inside of the data block.

   Once a data handle is obtained, it is possible to get the data object
   from it.  This is only necessary for heavier data structures such as
   geometry or strings.  Simple numeric data can be manipulated straight
   through the data handle.

   Note: using kDynSweptGeometry data requires that a plug-in or stand
   alone API application be linked with the OpenMayaFX library.
*/
class OPENMAYA_EXPORT MFnData : public MFnBase
{
	declareMFnNoVoidConstructor( MFnData, MFnBase );

public:
	MFnData();

	//! Types of dependency graph data
	enum Type {
        //! Invalid value
		kInvalid,
        //! Numeric, use MFnNumericData extract the node data.
		kNumeric,
		//! Plugin Blind Data, use MFnPluginData to extract the node data.
		kPlugin,
        //! Plugin Geometry, use MFnGeometryData to extract the node data.
		kPluginGeometry,
        //! String, use MFnStringData to extract the node data.
		kString,
        //! Matrix, use MFnMatrixData to extract the node data.
		kMatrix,
        //! String Array, use MFnStringArrayData to extract the node data.
		kStringArray,
        //! Double Array, use MFnDoubleArrayData to extract the node data.
		kDoubleArray,
        //! Float Array, use MFnFloatArrayData to extract the node data.
		kFloatArray,
        //! Int Array, use MFnIntArrayData to extract the node data.
		kIntArray,
        //! Point Array, use MFnPointArrayData to extract the node data.
		kPointArray,
        //! Vector Array, use MFnVectorArrayData to extract the node data.
		kVectorArray,
        //! Component List, use MFnComponentListData to extract the node data.
		kComponentList,
        //! Mesh, use MFnMeshData to extract the node data.
		kMesh,
        //! Lattice, use MFnLatticeData to extract the node data.
		kLattice,
        //! Nurbs Curve, use MFnNurbsCurveData to extract the node data.
		kNurbsCurve,
        //! Nurbs Surface, use MFnNurbsSurfaceData to extract the node data.
		kNurbsSurface,
        //! Sphere, use MFnSphereData to extract the node data.
		kSphere,
        //! ArrayAttrs, use MFnArrayAttrsData to extract the node data.
		kDynArrayAttrs,
        /*! SweptGeometry, use MFnDynSweptGeometryData to extract the
		  node data. This data node is in OpenMayaFX which must be
		  linked to.
		*/
	        kDynSweptGeometry,
        //! Subdivision Surface, use MFnSubdData to extract the node data.
		kSubdSurface,
	//! nObject data, use MFnNObjectData to extract node data
		kNObject,
	//! nId data, use MFnNIdData to extract node data
		kNId,
        //! Typically used when the data can be one of several types.
        kAny,
		/*! Last value. It does not represent real data, but can be
		  used to loop on all possible types
		*/
		kLast
	};

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnNoVoidConstructorConstConstructor( MFnData, MFnBase );

END_NO_SCRIPT_SUPPORT:
};

#endif /* __cplusplus */
#endif /* _MFnData */
