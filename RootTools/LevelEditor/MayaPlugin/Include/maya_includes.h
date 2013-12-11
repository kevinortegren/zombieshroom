#pragma once

// Måste definiera NT_PLUGIN and REQUIRE_IOSTREAM
#ifdef WIN32
	#define NT_PLUGIN // för att kunna kompilera under WIN32
	#define REQUIRE_IOSTREAM // för att kunna skriva ut i output-fönstret med std::cout, inget måste
	#define EXPORT __declspec(dllexport) // synliggöra plugin:et för Maya
#else
	#define EXPORT
#endif

// Function Sets
#include <maya/MFnPlugin.h> // får endast inkluderas i en fil

// Wrappers
#include <maya/MGlobal.h>
#include <maya/MCallbackIdArray.h>

// Messages
#include <maya/MMessage.h>
#include <maya/MTimerMessage.h>
#include <maya/MFnMesh.h>
#include <maya/MDGMessage.h>
#include <maya/MPolyMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MSceneMessage.h>

// Libs
#pragma comment(lib, "Foundation.lib") // Länka mot Foundation.lib
#pragma comment(lib, "OpenMaya.lib") // Länka mot OpenMaya.lib

#include <maya/MFnEnumAttribute.h>
#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFnTransform.h>
#include <maya/MMatrix.h>

#include <maya/MItDependencyNodes.h>
#include <maya/MFnCamera.h>
#include <maya/MFnLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnLambertShader.h>
#include <maya/MPlugArray.h>

#ifdef _DEBUG
	#define PrintMayaError
#endif

inline void Error(int line_number, MString error_desc)
{
	#ifdef PrintMayaError
	MString output;
	output = line_number;
	MGlobal::displayInfo("# ERROR @ (" + output + ") " + error_desc);
	#endif
}

inline void Error(int line_number, MString error_desc, MStatus maya_status)
{
	#ifdef PrintMayaError
	MString output;
	output = line_number;
	MGlobal::displayInfo("# ERROR @ (" + output + ") " + error_desc);

	if (!maya_status)
		MGlobal::displayInfo(maya_status.errorString());
	#endif
}

inline void Error(int line_number, MString error_desc, MStatus maya_status1, MStatus maya_status2)
{
	#ifdef PrintMayaError
	MString output;
	output = line_number;
	MGlobal::displayInfo("# ERROR @ (" + output + ") " + error_desc);

	if (!maya_status1)
		MGlobal::displayInfo(maya_status1.errorString());
	if (!maya_status2)
		MGlobal::displayInfo(maya_status2.errorString());
	#endif
}

inline void Error(int line_number, MStatus maya_status)
{
	#ifdef PrintMayaError
	MString output;
	output = line_number;

	if (!maya_status)
		MGlobal::displayInfo("# ERROR @ (" + output + ")" + maya_status.errorString());
	#endif
}

inline void Error(int line_number, MStatus maya_status1, MStatus maya_status2)
{
	#ifdef PrintMayaError
	MString output;
	output = line_number;

	if (!maya_status1)
		MGlobal::displayInfo("# ERROR @ (" + output + ")" + maya_status1.errorString());
	if (!maya_status2)
		MGlobal::displayInfo("# ERROR @ (" + output + ")" + maya_status2.errorString());
	#endif
}

inline bool Status(int line, MString error_desc, MStatus maya_status)
{
	bool status = true;
	if (!maya_status)
	{
		Error(line, error_desc, maya_status);
		status = false;
	}
	return status;
}

inline bool Status(int line, MStatus maya_status)
{
	bool status = true;
	if (!maya_status)
	{
		Error(line, maya_status);
		status = false;
	}
	return status;
}

inline bool Status(int line, MString error_desc, bool status)
{
	if (!status)
	{
		Error(line, error_desc);
	}
	return status;
}

template <class T1>
inline void Print(T1 t1Data)
{
	MString output;
	output = t1Data;
	MGlobal::displayInfo(output);
}

template <class T1, class T2>
inline void Print(T1 t1Data, T2 t2Data)
{
	MString output;
	output = t1Data;
	output += t2Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5, class T6>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data, T6 t6Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;
	output += t6Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data, T6 t6Data, T7 t7Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;
	output += t6Data;
	output += t7Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data, T6 t6Data, T7 t7Data, T8 t8Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;
	output += t6Data;
	output += t7Data;
	output += t8Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data, T6 t6Data, T7 t7Data, T8 t8Data, T9 t9Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;
	output += t6Data;
	output += t7Data;
	output += t8Data;
	output += t9Data;

	MGlobal::displayInfo(output);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline void Print(T1 t1Data, T2 t2Data, T3 t3Data, T4 t4Data, T5 t5Data, T6 t6Data, T7 t7Data, T8 t8Data, T9 t9Data, T10 t10Data)
{
	MString output;
	output = t1Data;
	output += t2Data;
	output += t3Data;
	output += t4Data;
	output += t5Data;
	output += t6Data;
	output += t7Data;
	output += t8Data;
	output += t9Data;
	output += t10Data;

	MGlobal::displayInfo(output);
}