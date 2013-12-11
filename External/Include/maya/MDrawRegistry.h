#ifndef _MDrawRegistry
#define _MDrawRegistry
//-
// ===========================================================================
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MDrawRegistry
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MObject.h>

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

class MPxShadingNodeOverride;
class MPxSurfaceShadingNodeOverride;
class MPxShaderOverride;
class MPxGeometryOverride;
class MPxDrawOverride;
class MPxSubSceneOverride;
class MPxVertexBufferGenerator;
class MPxVertexBufferMutator;
class MPxPrimitiveGenerator;
class MPxIndexBufferMutator;

// ****************************************************************************
// CLASS DECLARATION (MDrawRegistry)

//! \ingroup OpenMayaRender
//! \brief Access the registry associating node types with custom draw classes
/*!
This class provides a means to register custom draw overrides to be used by
Viewport 2.0 to draw dependency nodes.

The registry is based on classification strings. Each draw override must be
registered with a classification string and any node type with a matching
classification string will be drawn using the registered draw override.
*/
class OPENMAYARENDER_EXPORT MDrawRegistry
{
public:
	typedef MPxShadingNodeOverride* (*ShadingNodeOverrideCreator)(const MObject&);
	static MStatus registerShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					ShadingNodeOverrideCreator creator);
	static MStatus deregisterShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxSurfaceShadingNodeOverride* (*SurfaceShadingNodeOverrideCreator)(const MObject&);
	static MStatus registerSurfaceShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					SurfaceShadingNodeOverrideCreator creator);
	static MStatus deregisterSurfaceShadingNodeOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxShaderOverride* (*ShaderOverrideCreator)(const MObject&);
	static MStatus registerShaderOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					ShaderOverrideCreator creator);
	static MStatus deregisterShaderOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxGeometryOverride* (*GeometryOverrideCreator)(const MObject&);
	static MStatus registerGeometryOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					GeometryOverrideCreator creator);
	static MStatus deregisterGeometryOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxDrawOverride* (*DrawOverrideCreator)(const MObject&);
	static MStatus registerDrawOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					DrawOverrideCreator creator);
	static MStatus deregisterDrawOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

	typedef MPxSubSceneOverride* (*SubSceneOverrideCreator)(const MObject&);
	static MStatus registerSubSceneOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					SubSceneOverrideCreator creator);
	static MStatus deregisterSubSceneOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId);

    typedef MPxVertexBufferGenerator* (*VertexBufferGeneratorCreator)();
    static MStatus registerVertexBufferGenerator(
        const MString& bufferName, VertexBufferGeneratorCreator creator);
    static MStatus deregisterVertexBufferGenerator(
        const MString& bufferName);

    typedef MPxVertexBufferMutator* (*VertexBufferMutatorCreator)();
    static MStatus registerVertexBufferMutator(
        const MString& bufferName, VertexBufferMutatorCreator creator);
    static MStatus deregisterVertexBufferMutator(
        const MString& bufferName);

    typedef MPxPrimitiveGenerator* (*PrimitiveGeneratorCreator)();
    static MStatus registerPrimitiveGenerator(
        const MString& primitiveType, PrimitiveGeneratorCreator creator);
    static MStatus deregisterPrimitiveGenerator(
        const MString& primitiveType);

    typedef MPxIndexBufferMutator* (*IndexBufferMutatorCreator)();
    static MStatus registerIndexBufferMutator(
        const MString& primitiveType, IndexBufferMutatorCreator creator);
    static MStatus deregisterIndexBufferMutator(
        const MString& primitiveType);

	static const char* className();

private:
	MDrawRegistry();
	~MDrawRegistry();

	static MStatus deregisterOverrideCreator(
					const MString& drawClassification,
					const MString& registrantId,
					const char* methodName);
};

} // namespace MHWRender

// ****************************************************************************

#endif /* __cplusplus */
#endif /* _MDrawRegistry */

