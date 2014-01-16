#include "stdafx.h"
#include "maya_includes.h"

#include "SharedMemory.h"
#include <algorithm>
#include "Constants.h"
SharedMemory SM;

// Globala variabler.
MCallbackIdArray g_callback_ids;
MObject g_selectedObject;

MObject g_objectList[g_maxSceneObjects], g_mayaMeshList[g_maxMeshes], g_mayaCameraList[g_maxCameras], g_mayaLightList[g_maxLights], g_mayaMaterialList[g_maxMeshes];
int currNrSceneObjects=0, currNrMeshes=0, currNrLights=0, currNrCameras=0, currNrMaterials = 0, currNrMaterialObjects = 0;

void ConnectionCB(MPlug& srcPlug, MPlug& destPlug, bool made, void *clientData);
void dirtyTextureNode(MObject &node, MPlug&, void *clientData);
void dirtyMeshNodeCB(MObject &node, MPlug &plug, void *clientData);
void dirtyLightNodeCB(MObject &node, MPlug &plug, void *clientData);
void dirtyTransformNodeCB(MObject &node, MPlug &plug, void *clientData);
void NodeAddedCB(MObject &node, void *clientData);
void NodeRemovedCB(MObject &node, void *clientData);
void viewCB(const MString &str, void *clientData);
void dirtyMaterialCB(MObject &node, MPlug &plug, void *clientData);

int cameraExists(MString name);

void checkForNewMeshes(bool made, MObject source, MObject destination);
void checkForNewCameras(MObject &node, void *clientData);
void loadScene();
void sortObjectList();
int nodeExists(MObject node);
void printLists();
void checkForNewLights(MObject &node, void *clientData);
void GetMaterial(MObject node);
void MayaMeshToList(MObject node, int meshIndex, bool doTrans, bool doMaterial, bool doMesh);
void MayaLightToList(MObject node, int id);
void MayaCameraToList(MObject node, int id);
void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth, MFnDependencyNode &material_node, MObject &out_materialObjectNode);
MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle );
void Export();
int exportMaya;

// Lägger till ett callback-id i callback-arrayen.
void AddCallbackID(MStatus status, MCallbackId id)
{
	if (Status(__LINE__, status))
		g_callback_ids.append(id);
}

// Initiera pluginet.
EXPORT MStatus initializePlugin(MObject obj)
{	
	//SM.InitalizeSharedMemory();
	sortObjectList();
	//printLists();
	loadScene();

	SM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(SM.IdMutexHandle, SM.milliseconds);

	*SM.export = 0;
	exportMaya = 0;

	ReleaseMutex(SM.IdMutexHandle);

	myInitMemoryCheck(); // Har koll på minnet och minnesläckor
	MStatus status = MS::kSuccess;

	//MItDependencyNodes it(MFn::kFileTexture);			/////////////////////////////////////////FUUUUUGLY///////////////////////////////////////////////////

	//while(!it.isDone())
	//{
	//	Print("Herroooooow");
	//	MObject obj = it.item();

	//	it.next();
	//	MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(obj, dirtyTextureNode, nullptr, &status);
	//	AddCallbackID(status, id);
	//}

	// Registrera ett nytt plug-in till Maya.
	MFnPlugin plugin(obj, "UD1414-Labb02", "1.0", "Any", &status);

	if (Status(__LINE__, status))
	{
		MCallbackId id = MDGMessage::addConnectionCallback(ConnectionCB, nullptr, &status);				
		AddCallbackID(status, id);

		id = MDGMessage::addNodeAddedCallback(NodeAddedCB, "dependNode", nullptr, &status);
		AddCallbackID(status, id);

		id = MDGMessage::addNodeRemovedCallback(NodeRemovedCB, "dependNode", nullptr, &status);
		AddCallbackID(status, id);
	}

	return status;
}

void Export()
{	
	for(int i = 0; i < currNrMeshes; i++)
	{
		Print("Index ", i, " ", SM.meshList[i].transformation.name);
		int count = 0;
		int countVertex = 0;
		bool exists = true;
		int saveJ = 0;
		if(i == 0)
		{
			exists = false;
		}
		else
		{
			for(int j = 0; j < i; j++)
			{
				if(SM.meshList[i].nrOfVertices == SM.meshList[j].nrOfVertices)
				{
					for(int v = 0; v < SM.meshList[i].nrOfVertices; v++)
					{
						if(SM.meshList[i].vertex[v] != SM.meshList[j].vertex[v])	//Ska hoppa ut när den hittar en identisk.
						{
							exists = false;
						}
						else
						{
							countVertex++;
							if(countVertex == SM.meshList[i].nrOfVertices)
							{
								exists = true;
								saveJ = j;
								break;
							}
						}
						
					}
					if(exists)
						break;
				}
				else
				{
					count++;
					if(count == i)
						exists = false;
				}
				
			}
		
		}

		if(exists)
		{
			MFnMesh mesh = g_mayaMeshList[saveJ];
			Print("Overwriting: ", SM.meshList[i].modelName, " index: ", i, " with ", SM.meshList[saveJ].modelName, " index: ", saveJ);
			memset(SM.meshList[i].modelName, NULL, sizeof(SM.meshList[i].modelName));
			memcpy(SM.meshList[i].modelName, SM.meshList[saveJ].modelName, mesh.name().numChars());
			SM.UpdateSharedMesh(i, true, false, currNrMeshes);
			//Herp
		}
		else
		{
			MString name, realName, ExportFunction;		//CANNOT SAVE FULLPATHNAME FILES with char | //Fixed in cleanfullpathname() function but not needed anymore
			name = SM.meshList[i].modelName;

			MFnMesh mesh = g_mayaMeshList[i];
			realName = mesh.fullPathName();

			MGlobal::executeCommand("select -r " + realName);

			string outputDirectory = g_savepath + "Models/";
			MString savepath = outputDirectory.c_str();
			//ExportFunction = "file -force -options \"\" -typ \"OpenCOLLADA exporter\" -pr -es \"" + savepath + name + ".dae\"";
			ExportFunction = "file -force -options \"bakeTransforms=0;relativePaths=0;copyTextures=1;exportTriangles=1;cgfxFileReferences=1;isSampling=0;curveConstrainSampling=0;removeStaticCurves=1;exportPolygonMeshes=1;exportLights=0;exportCameras=0;exportJointsAndSkin=0;exportAnimations=0;exportInvisibleNodes=0;exportDefaultCameras=0;exportTexCoords=1;exportNormals=1;exportNormalsPerVertex=1;exportVertexColors=0;exportVertexColorsPerVertex=0;exportTexTangents=1;exportTangents=0;exportReferencedMaterials=0;exportMaterialsOnly=0;exportXRefs=0;dereferenceXRefs=1;exportCameraAsLookat=0;cameraXFov=0;cameraYFov=1;doublePrecision=0;\" -typ \"OpenCOLLADA exporter\" -pr -es \"" + savepath + name + ".dae\"";
			Print(name);
			MGlobal::executeCommand(ExportFunction);
			Print(ExportFunction);
		}
	}

	SM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(SM.IdMutexHandle, SM.milliseconds);

	*SM.export = 2;
	exportMaya = 0;

	ReleaseMutex(SM.IdMutexHandle);
}

void sortObjectList()
{
	MItDependencyNodes it(MFn::kInvalid);
	MStatus status;

	currNrSceneObjects = 0;

	//add all objects to a array.
	while(!it.isDone())
	{
		g_objectList[currNrSceneObjects] = it.item();
		currNrSceneObjects++;
		it.next();
	}
	//Sort the list mesh to meshlist and so on.
	for(int i= 0; i < currNrSceneObjects; i ++)
	{
		if(g_objectList[i].hasFn(MFn::kMesh))
		{
			MFnMesh tempMesh = g_objectList[i];
			g_mayaMeshList[currNrMeshes] = g_objectList[i];
			MayaMeshToList(g_mayaMeshList[currNrMeshes],currNrMeshes, true, true, true);
			currNrMeshes ++;
		}
		else if(g_objectList[i].hasFn(MFn::kCamera))
		{
			MFnCamera tempCam = g_objectList[i];
			g_mayaCameraList[currNrCameras] = g_objectList[i];
			MayaCameraToList(g_mayaCameraList[currNrCameras], currNrCameras);	
			currNrCameras++;
		}
		else if(g_objectList[i].hasFn(MFn::kLight))
		{
			MFnLight tempLight = g_objectList[i];
			g_mayaLightList[currNrLights] = g_objectList[i];
			MayaLightToList(g_mayaLightList[currNrLights], currNrLights);		

			currNrLights++;
		}
		else if(g_objectList[i].hasFn(MFn::kLambert))
		{
			MFnLambertShader Lambert = g_objectList[i];
			g_mayaMaterialList[currNrMaterialObjects] = g_objectList[i];

			Print("Adding CB to ", Lambert.name());
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_objectList[i], dirtyMaterialCB, nullptr, &status);
			AddCallbackID(status, id);

			currNrMaterialObjects++;
		}
	}
}

void printLists()
{
	//Print the lists, cuz it´s how cool kidz does it.
	Print("Meshes");
	for(int i = 0; i < currNrMeshes; i++)
	{
		MFnMesh tempMesh = g_mayaMeshList[i];
		Print(tempMesh.fullPathName());
		
	}

	Print("Cameras");
	for(int i = 0; i < currNrCameras; i++)
	{
		MFnCamera tempCamera = g_mayaCameraList[i];
		Print(tempCamera.fullPathName());
		
	}

	Print("Lights");
	for(int i = 0; i < currNrLights; i++)
	{
		MFnLight tempLight = g_mayaLightList[i];
		Print(tempLight.fullPathName());
		
	}


}

void loadScene()
{
	MStatus status;
	//Goes through all the objects in sorted list and adds callbacks to them.

	//mesh transformation CB
	for(int i = 0; i < currNrMeshes; i++)
	{
		MFnMesh mesh = g_mayaMeshList[i];

		if(mesh.parent(0, &status).hasFn(MFn::kTransform))
		{
			MObject transform = mesh.parent(0, &status);
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}
		if(g_mayaMeshList[i].hasFn(MFn::kMesh))
		{
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaMeshList[i], dirtyMeshNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}

		MayaMeshToList(g_mayaMeshList[i], i, true, true, true);
		SM.UpdateSharedMesh(i, true, true, currNrMeshes);
	}
	//Light transformation CB
	for(int i = 0; i < currNrLights; i++)
	{
		MFnLight light = g_mayaLightList[i];
		if(g_mayaLightList[i].hasFn(MFn::kLight))
		{
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaLightList[i], dirtyLightNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}

		if(light.parent(0,&status).hasFn(MFn::kTransform))
		{
			MObject transform = light.parent(0, &status);
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}

		MayaLightToList(g_mayaLightList[i], i);
		SM.UpdateSharedLight(i, currNrLights);
	}
	//Camera transformation CB
	for(int i = 0; i < currNrCameras; i++)
	{
		MFnCamera camera = g_mayaCameraList[i];

		const MString temp = "modelPanel4";

		MCallbackId id = MUiMessage::add3dViewPostRenderMsgCallback(temp, viewCB, nullptr, &status);
		
		AddCallbackID(status, id);

		MayaCameraToList(g_mayaCameraList[i], i);
		SM.UpdateSharedCamera(i);
	}

}

void viewCB(const MString &str, void *clientData)
{
	//Print("viewCB");

	//Callback when moving camera
	int tempInt = cameraExists(str);

	if(tempInt!= -1)
	{
		MayaCameraToList(g_mayaCameraList[tempInt], tempInt);
		SM.UpdateSharedCamera(tempInt);		
	}

	SM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(SM.IdMutexHandle, SM.milliseconds);
	exportMaya = *SM.export;
	ReleaseMutex(SM.IdMutexHandle);

	if(exportMaya == 1)
	{
		Export();
		exportMaya = 0;
	}	
}

////////////////////////////	LOOK IF A MESH NODE IS DIRTY  //////////////////////////////////////////
void dirtyMeshNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	Print("dirtyMeshNodeCB");

	int index = nodeExists(node);

	if(index != -1)
	{
		MayaMeshToList(node, index, true, true, true);
		SM.UpdateSharedMesh(index, true, true, currNrMeshes);
	}
}

 void dirtyTextureNode(MObject &node, MPlug&, void *clientData)					/////////////////////////////////////DONT DO SHIT//////////////////////////////////
 {
	Print("DEEPSHIEEEEET IT WOOOORKS");
 }

void dirtyLightNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	Print("dirtyLightNode");
	MStatus status;
	MFnLight Light = node;

	int index = nodeExists(node);
	if(index != -1)
	{
		MayaLightToList(node, index);
		SM.UpdateSharedLight(index, currNrLights);
	}
}

////////////////////////////	LOOK IF A TRANSFORMATION NODE IS DIRTY  //////////////////////////////////////////
void getFlags(MObject object, int index)
{
	if(object.hasFn(MFn::kTransform))
	{
		MFnTransform trans = object;
		Print("Getting flags for: ", SM.meshList[index].modelName);

		for(int i = 0; i < trans.parentCount(); i++)
		{
			if(trans.parent(i).hasFn(MFn::kTransform))
			{
				MFnTransform temp = trans.parent(i);
				Print("Found parent: ", temp.name());

				bool alreadyExists = false;
				string name = temp.name().asChar();

				for(int j = 0; j < g_maxNrOfFlags; j ++)
				{
					if(alreadyExists == false)
					{					
						if(name.compare(SM.meshList[index].transformation.flags[j]) == 0)
						{
							alreadyExists = true;
							Print("Flag exists skipping...");
						}
						else
						{
							if(SM.meshList[index].transformation.flags[j][0] == NULL)
							{
								memcpy(SM.meshList[index].transformation.flags[j], temp.name().asChar(), g_shortMaxNameLength);
								Print("Added flag ", temp.name().asChar(), " to ", SM.meshList[index].modelName, " at index ", j);
								alreadyExists = true;
							}
						}
					}
				}
				getFlags(trans.parent(i), index);
			}
		}
	}
}

void dirtyTransformNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	Print("dirtyTransformNodeCB");
	
	MStatus status = MS::kSuccess;
	MFnTransform trans = node;

	int index = nodeExists(trans.child(0, &status));

	if(index != -1)
	{

		if(trans.child(0, &status).hasFn(MFn::kMesh))
		{
			MayaMeshToList(trans.child(0, &status), index, true, true, true);
		}

		if(trans.child(0, &status).hasFn(MFn::kLight))
		{
			MayaLightToList(trans.child(0, &status), index);
			SM.UpdateSharedLight(index, currNrLights);
		}

	}
}



void dirtyMaterialCB(MObject &node, MPlug &plug, void *clientData)
{// Reagerar precis INNAN man bytt textur!
	Print("dirtyMaterialCB");

	string materialName = "";
	if(node.hasFn(MFn::kLambert))
	{
		MFnLambertShader Lambert = node;
		materialName = Lambert.name().asChar();
	}

	if(materialName != "")
	{
		for(int i = 0; i < currNrMeshes; i++)
		{
			string temp = SM.meshList[i].materialName;
			Print("Comparing", temp.c_str(), " and ", materialName.c_str());
			if(SM.meshList[i].materialName == materialName)
			{
				MFnMesh mesh = g_mayaMeshList[i];

				Print("Updating ", materialName.c_str(), " and ", mesh.fullPathName(), " at index ", i);
				MayaMeshToList(g_mayaMeshList[i], i, true, true, true);
				SM.UpdateSharedMaterials(currNrMaterials, i);
				SM.UpdateSharedMesh(i, true, true, currNrMeshes);
			}
		}
	}

	MStatus status = MS::kSuccess;	
}

////////////////////////////	SORT OUT WHAT TYPE THE NEW NODE ARE //////////////////////////////////////////
void NodeAddedCB(MObject &node, void *clientData)
{
	Print("NodeAddedCB");

	//Callback when node is added.
	checkForNewCameras(node, clientData);
	checkForNewLights(node, clientData);
	//printLists();
}

void NodeRemovedCB(MObject &node, void *clientData)
{
	Print("NodeRemovedCB");

	//Callback when node is removed
	int removeID = 0;

	if(node.hasFn(MFn::kMesh))
	{
		Print("Removed a mesh!");

		MFnMesh mesh = node;

		for(int i = 0; i < currNrMeshes; i++)
		{
			MFnMesh listMesh = g_mayaMeshList[i];
			if(listMesh.name() == mesh.name())
			{
				Print("Mesh ", mesh.name(), " at index ", i);
				removeID = i;
				for(int j = i; j < currNrMeshes; j++)
				{
					g_mayaMeshList[j] = g_mayaMeshList[j+1];
					MayaMeshToList(g_mayaMeshList[j], j, true, true, true);
					SM.UpdateSharedMesh(j, true, true, currNrMeshes-1);
				}
																					//Will crash if it reaches g_maxMeshes
				currNrMeshes--;
				SM.RemoveMesh(removeID, currNrMeshes);
			}
		}	
	}
	if(node.hasFn(MFn::kLight))
	{
		MFnLight light = node;

		for(int i = 0; i < currNrLights; i ++)
		{
			MFnLight listLight = g_mayaLightList[i];
			if(listLight.name() == light.name())
			{
				removeID = i;
				for(int j = i; j < currNrLights; j++)
				{
					g_mayaLightList[j] = g_mayaLightList[j+1];
					MayaLightToList(g_mayaLightList[j], j);
					SM.UpdateSharedLight(j,currNrLights-1);
				}
				currNrLights--;
				SM.RemoveLight(removeID, currNrMeshes);
			}
		}
	}
}

void checkForNewMaterials(bool made, MObject source, MObject destination)
{

}

// MESHES
// CONNECTION CB FOR MESHES
void ConnectionCB(MPlug& srcPlug, MPlug& destPlug, bool made, void *clientData)
{

	MStatus status1 = MS::kSuccess;
	MStatus status2 = MS::kSuccess;

	MObject source = srcPlug.node(&status1);
	MObject destination = destPlug.node(&status2);
	
	Print("-_-_-_-_-_-_-_ ConnectionCB _-_-_-_-_-_-_-_-_-_-_");
	Print("source is ", source.apiTypeStr());
	Print("destination is ", destination.apiTypeStr());
	Print("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");

	if (Status(__LINE__, status1) && Status(__LINE__, status2))
	{
		checkForNewMeshes(made, source, destination);
		//Print("Updating all materials");
		//for(int i = 0; i < currNrMeshes; i++)
		//{
		//	for(int j = 0; j < currNrMaterials; j++)
		//	{
		//		SM.UpdateSharedMaterials(currNrMaterials, j, i);
		//	}
		//}

		//if (source.hasFn(MFn::kMesh))
		//{
		//	MFnMesh mesh = source;
		//	Print("Updating material in ", mesh.fullPathName());
		//	int updateID = nodeExists(source);
		//	SM.UpdateSharedMesh(updateID, false, false, currNrMeshes);
		//	SM.UpdateSharedMaterials(currNrMaterials, SM.meshList[updateID].MaterialID, updateID);
		//}

		//if(source.hasFn(MFn::kFileTexture))
		//{
		//	//string* materialName = new string();
		//	string materialName = "";
		//	if(destination.hasFn(MFn::kLambert))
		//	{
		//		MFnLambertShader Lambert = destination;
		//		//*materialName = Lambert.name().asChar();
		//		materialName = Lambert.name().asChar();
		//	}

		//	if(materialName != "")
		//	{
		//		MStatus status;
		//		Print("Adding CB to ", materialName.c_str());
		//		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(destination, dirtyMaterialCB, nullptr, &status);
		//		AddCallbackID(status, id);
		//	}

		//}

		if(source.hasFn(MFn::kMesh) && destination.hasFn(MFn::kShadingEngine))
		{
			int temp = nodeExists(source);

			if(temp != -1)
			{
				MayaMeshToList(source, temp, true, true, true);
				//SM.UpdateSharedMesh(temp, true, true, currNrMeshes);
				//SM.UpdateSharedMaterials(currNrMaterials, -1, temp);
			}
		}

		if(destination.hasFn(MFn::kLambert))
		{
			MStatus status;
			MFnLambertShader lambert = destination;

			int id = nodeExists(destination);
			if(id == -1)
			{
				Print("Adding CB to ", lambert.name());
				MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(destination, dirtyMaterialCB, nullptr, &status);
				AddCallbackID(status, id);
				g_mayaMaterialList[currNrMaterialObjects] = destination;
				currNrMaterialObjects++;
			}
		}
	}
}

void checkForNewMeshes(bool made, MObject source, MObject destination)
{
	MCallbackId id;
	MStatus status;

	if (made && source.hasFn(MFn::kPolyCreator) && destination.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = destination;
		g_mayaMeshList[currNrMeshes] = destination;

		id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaMeshList[currNrMeshes], dirtyMeshNodeCB, nullptr, &status);
		AddCallbackID(status, id);


		if(mesh.parent(0, &status).hasFn(MFn::kTransform))
		{
			MObject transform = mesh.parent(0, &status);

			id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);

			MayaMeshToList(destination, currNrMeshes, true, true, true);
			currNrMeshes++;		

		}
	}
}

// Cameras
void checkForNewCameras(MObject &node, void *clientData)
{
	MFnCamera camera = node;

	MStatus status;

	if(node.hasFn(MFn::kCamera))
	{
		Print("New Camera Added!");
		g_mayaCameraList[currNrCameras] = node;
		MayaCameraToList(node, currNrCameras);
		currNrCameras++;
	}

	if(camera.parent(0,&status).hasFn(MFn::kTransform))
	{
		MObject transform = camera.parent(0, &status);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}	
}

// Lights
void checkForNewLights(MObject &node, void *clientData)
{
	MFnLight light = node;

	MStatus status;

	if(node.hasFn(MFn::kLight))
	{
		Print("New Light Added");
		g_mayaLightList[currNrLights] = node;
		MayaLightToList(node, currNrLights);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(node, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
		currNrLights++;
	}
	
	if(light.parent(0,&status).hasFn(MFn::kTransform))
	{
		MObject transform = light.parent(0, &status);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
}

int cameraExists(MString name)
{
	//Return a index so we know what camera to update.
	int answer = -1;

	if(name == "modelPanel1")
	{
		answer = 1;
	}
	if(name == "modelPanel4")
	{
		answer = 0;
	}
	if(name == "modelPanel3")
	{
		answer = 3;
	}
	if(name == "modelPanel2")
	{
		answer = 2;
	}

	return answer;
}

int nodeExists(MObject node)
{
	//Takes out the index of the node in our arrays. Else return -1.
	int answer = -1;
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = node;
		for(int i = 0; i < currNrMeshes; i++)
		{
			MFnMesh temp = g_mayaMeshList[i];

			if(temp.fullPathName() == mesh.fullPathName())
			{
				answer = i;
				break;
			} 
		}	
	}

	if (node.hasFn(MFn::kCamera))
	{
		MFnCamera cam = node;
		for(int i = 0; i < currNrCameras; i++)
		{
			MFnCamera temp = g_mayaCameraList[i];

			if(temp.fullPathName() == cam.fullPathName())
			{
				answer = i;
				break;
			}
		}	
	}

	if (node.hasFn(MFn::kLight))
	{
		MFnLight light = node;
		for(int i = 0; i < currNrLights; i++)
		{
			MFnLight temp = g_mayaLightList[i];

			if(temp.fullPathName() == light.fullPathName())
			{
				answer = i;
				break;
			}
		}	
	}

	if (node.hasFn(MFn::kLambert))
	{
		MFnLambertShader lambert = node;
		for(int i = 0; i < currNrMaterialObjects; i++)
		{
			MFnLambertShader temp = g_mayaMaterialList[i];
			Print("nodeExists() Comparing ", temp.name(), " and ", lambert.name());
			if(temp.name() == lambert.name())
			{
				answer = i;
				break;
			}
		}	
	}

	return answer;
}

MString cleanFullPathName(const char * str)
{
	//Are being used to remove retarded symbols from fullpathname.
	string tmpString = str;
	char chars[] = "| _";
	for (unsigned int i = 0; i < strlen(chars); ++i)
	{
		tmpString.erase(std::remove(tmpString.begin(), tmpString.end(), chars[i]), tmpString.end());
	}

	MString TEMP = tmpString.c_str();
	return TEMP;
}
////////////////////////////	ADD THE NODES TO THE SHARED LIST	 //////////////////////////////////////////

void MayaMeshToList(MObject node, int meshIndex, bool doTrans, bool doMaterial, bool doMesh)
{
	//Get mesh and transformation information from the nodes. Used when editing a node / adding a new one.
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MVector rotPivot, scalePivot;
	MSpace::Space space_transform = MSpace::kTransform;
	MSpace::Space space_local = MSpace::kObject;

	MIntArray triangleCounts, triangleVertices;

	///////////////////////	MESH
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = node;

		int numVertices = mesh.numVertices();
		int numPolygons = mesh.numPolygons();
		MPointArray points_;
		float floatPoints[g_maxVerticesPerMesh][4];
		float U, V;

		//Get the material information.
		//Get, set texturepaths
		MString texturepath = "";
		MString normalpath = "";
		MString materialName = "";
		float bumpdepth;
		MFnDependencyNode material_node;
		MObject material_objectNode;

		//Get the name of the transformation and the model name for the exporter.
		MString transName = cleanFullPathName(mesh.fullPathName().asChar());
		//Get and set mesh name
		memcpy(SM.meshList[meshIndex].transformation.name, transName.asChar(), transName.numChars());

		MString name = cleanFullPathName(mesh.name().asChar());
		memcpy(SM.meshList[meshIndex].modelName, name.asChar(), name.numChars());


		if(doMaterial)
		{/////////////////////  GET MATERIALS AND ADD TO LIST /////////////////////////////////////////////

		ExtractMaterialData(mesh, texturepath, normalpath, bumpdepth, material_node, material_objectNode);
		materialName = material_node.name();

		//Check if the material already exists.
		int materialID = -1;
		for(int i = 0; i < currNrMaterials; i++)
		{
			std::string tempMaterialName = SM.materialList[i].materialName;
			std::string tempMaterialName2 = materialName.asChar();
			if(tempMaterialName == tempMaterialName2)
			{
				materialID = i;
			}
		}

		//If it´s not existing then add it to the material array.
		if(materialID == -1)
		{						
			memset(SM.materialList[currNrMaterials].materialName, NULL, sizeof(SM.materialList[currNrMaterials].materialName));
			memcpy(SM.materialList[currNrMaterials].materialName, materialName.asChar(), materialName.numChars());
			memset(SM.materialList[currNrMaterials].texturePath, NULL, sizeof(SM.materialList[currNrMaterials].texturePath));
			memcpy(SM.materialList[currNrMaterials].texturePath, texturepath.asChar(), texturepath.numChars());
			memset(SM.materialList[currNrMaterials].normalPath, NULL, sizeof(SM.materialList[currNrMaterials].normalPath));
			memcpy(SM.materialList[currNrMaterials].normalPath, normalpath.asChar(),normalpath.numChars());
			SM.meshList[meshIndex].MaterialID = currNrMaterials;

			currNrMaterials++;
			
		}
		else
		{
			SM.meshList[meshIndex].MaterialID = materialID;
			memset(SM.materialList[materialID].texturePath, NULL, sizeof(SM.materialList[materialID].texturePath));
			memcpy(SM.materialList[materialID].texturePath, texturepath.asChar(), texturepath.numChars());
			memset(SM.materialList[materialID].normalPath, NULL, sizeof(SM.materialList[materialID].normalPath));
			memcpy(SM.materialList[materialID].normalPath, normalpath.asChar(),normalpath.numChars());
		}

		memset(SM.meshList[meshIndex].materialName, NULL, sizeof(SM.meshList[meshIndex].materialName));
		memcpy(SM.meshList[meshIndex].materialName, materialName.asChar(), materialName.numChars());
		SM.UpdateSharedMaterials(currNrMaterials, meshIndex);
		//Print("Updated material ", materialName.asChar());

		}

		if(doMesh)
		{///////////////////////////////// GET VERTEX AND UV CONVERTED TO TRIANGLES //////////////////////////////////

		//Get points from mesh
		mesh.getPoints(points_, space_local);

		//Convert points to float array
		points_.get(floatPoints);

		MIntArray polygonVertices;
		MPoint UVpoint;
		float2 UV;
		int triangleVertices[3];
		MVector normal;
		int count = 0;
		MFloatVectorArray normals;

		SM.meshList[meshIndex].nrOfVertices = 0;

		int uvID;
		mesh.getNormals(normals);

		//goes through the polygons extracting information. 
		//Quads -> triangles
		for(int i = 0; i < numPolygons; i++)
		{
			MIntArray polygonVertices, localIndex;
			int polygonVertexCount = mesh.polygonVertexCount(i);
			MIntArray normalIDs;
			int nrOfTriangles;

			//decides if the face have 1 or 2 triangles.
			if(polygonVertexCount >= 4)
				nrOfTriangles = 2;
			else
				nrOfTriangles = 1;
			
			mesh.getPolygonVertices(i, polygonVertices);

			mesh.getFaceNormalIds(i, normalIDs);

			//Two triangles (t) in polygon
			for(int t = 0; t < nrOfTriangles; t++)
			{
				mesh.getPolygonTriangleVertices(i, t, triangleVertices);
				MIntArray triangleVertices2;

				for(int k = 0; k < 3; k++)
				{
					triangleVertices2.append(triangleVertices[k]);
				}

				localIndex = GetLocalIndex(polygonVertices, triangleVertices2);

				//Three vertexes (j) in triangle
				
				for(int j = 0; j < 3; j++)
				{
					int temp = i;
					
					
					SM.meshList[meshIndex].vertex[count].x = floatPoints[triangleVertices[j]][0];
					SM.meshList[meshIndex].vertex[count].y = floatPoints[triangleVertices[j]][1];
					SM.meshList[meshIndex].vertex[count].z = floatPoints[triangleVertices[j]][2];

					//mesh.getVertexNormal(localIndex[j], normal, space_local);
					//OLD

					normal = normals[normalIDs[localIndex[j]]];					

					SM.meshList[meshIndex].normal[count].x = normal.x;
					SM.meshList[meshIndex].normal[count].y = normal.y;
					SM.meshList[meshIndex].normal[count].z = normal.z;

					mesh.getPolygonUVid(i, localIndex[j], uvID, 0);
					mesh.getUV(uvID, U, V, 0);

					SM.meshList[meshIndex].UV[count].x = U;
					SM.meshList[meshIndex].UV[count].y = 1-V;

					count++;
					SM.meshList[meshIndex].nrOfVertices ++;
				}
			}
		}

		}

		if(doTrans)
		{///////////////////////	MESH TRANSFORM ////////////////////////////////////////
		if(mesh.parent(0,&status).hasFn(MFn::kTransform))
		{
			glm::vec3 position;
			MFnTransform transform = mesh.parent(0, &status);
			rotPivot = transform.rotatePivot(MSpace::kObject);
			scalePivot = transform.scalePivot(MSpace::kObject);

			transform.getScale(scale);
			transform.getRotationQuaternion(rotX, rotY, rotZ, rotW, MSpace::kPreTransform);
			position.x = transform.getTranslation(space_transform).x;
			position.y = transform.getTranslation(space_transform).y;
			position.z = transform.getTranslation(space_transform).z;

			SM.meshList[meshIndex].transformation.position.x = position.x;
			SM.meshList[meshIndex].transformation.position.y = position.y;
			SM.meshList[meshIndex].transformation.position.z = position.z;

			SM.meshList[meshIndex].transformation.scale.x = scale[0];
			SM.meshList[meshIndex].transformation.scale.y = scale[1];
			SM.meshList[meshIndex].transformation.scale.z = scale[2];

			SM.meshList[meshIndex].transformation.rotation.x = rotX;
			SM.meshList[meshIndex].transformation.rotation.y = rotY;
			SM.meshList[meshIndex].transformation.rotation.z = rotZ;
			SM.meshList[meshIndex].transformation.rotation.w = rotW;

			SM.meshList[meshIndex].transformation.rotPivot.x = rotPivot.x;
			SM.meshList[meshIndex].transformation.rotPivot.y = rotPivot.y;
			SM.meshList[meshIndex].transformation.rotPivot.z = rotPivot.z;

			SM.meshList[meshIndex].transformation.scalePivot.x = scalePivot.x;
			SM.meshList[meshIndex].transformation.scalePivot.y = scalePivot.y;
			SM.meshList[meshIndex].transformation.scalePivot.z = scalePivot.z;

			getFlags(mesh.parent(0,&status), meshIndex);
		}
		}
	}

	SM.UpdateSharedMesh(meshIndex, doTrans, doMesh, currNrMeshes);
}

void MayaLightToList(MObject node, int lightIndex)
{
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MSpace::Space space_transform = MSpace::kTransform;
	MSpace::Space space_local = MSpace::kObject;

	///////////////////////	LIGHT
	if(node.hasFn(MFn::kLight))
	{
		MFnLight light = node;
		SM.lightList[lightIndex].color.r = light.color(&status).r;
		SM.lightList[lightIndex].color.g = light.color(&status).g;
		SM.lightList[lightIndex].color.b = light.color(&status).b;
		SM.lightList[lightIndex].color.a = light.color(&status).a;
		SM.lightList[lightIndex].Intensity = light.intensity(&status);

		memcpy(SM.lightList[lightIndex].transformation.name, light.fullPathName().asChar(), light.fullPathName().numChars());
		//SM.lightList[lightIndex].transformation.name = light.fullPathName().asChar();

		///////////////////////	LIGHT TRANSFORM
		if(light.parent(0, &status).hasFn(MFn::kTransform))
		{
			MFnTransform transform = light.parent(0, &status);

			transform.getScale(scale);
			transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

			SM.lightList[lightIndex].transformation.position.x = transform.getTranslation(space_transform).x;
			SM.lightList[lightIndex].transformation.position.y = transform.getTranslation(space_transform).y;
			SM.lightList[lightIndex].transformation.position.z = transform.getTranslation(space_transform).z;

			SM.lightList[lightIndex].transformation.scale.x = scale[0];
			SM.lightList[lightIndex].transformation.scale.y = scale[1];
			SM.lightList[lightIndex].transformation.scale.z = scale[2];

			SM.lightList[lightIndex].transformation.rotation.x = rotX;
			SM.lightList[lightIndex].transformation.rotation.y = rotY;
			SM.lightList[lightIndex].transformation.rotation.z = rotZ;
			SM.lightList[lightIndex].transformation.rotation.w = rotW;
		}
	}
}

void MayaCameraToList(MObject node, int cameraIndex)
{
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MSpace::Space space_transform = MSpace::kTransform;
	MSpace::Space space_local = MSpace::kObject;

	///////////////////////	CAMERA
	if(node.hasFn(MFn::kCamera))
	{
		MFnCamera camera = node;

		memcpy(SM.cameraList[cameraIndex].transformation.name, camera.fullPathName().asChar(), camera.fullPathName().numChars());
		//SM.cameraList[cameraIndex].transformation.name = camera.fullPathName().asChar();
		SM.cameraList[cameraIndex].aspectRatio = camera.aspectRatio();

		SM.cameraList[cameraIndex].eyePoint.x = camera.eyePoint(space_transform).x;
		SM.cameraList[cameraIndex].eyePoint.y = camera.eyePoint(space_transform).y;
		SM.cameraList[cameraIndex].eyePoint.z = camera.eyePoint(space_transform).z;

		SM.cameraList[cameraIndex].farClippingPlane = camera.farClippingPlane();
		SM.cameraList[cameraIndex].horizontalFieldOfView = camera.horizontalFieldOfView();
		SM.cameraList[cameraIndex].nearClippingPlane = camera.nearClippingPlane();

		SM.cameraList[cameraIndex].rightDirection.x = camera.rightDirection().x;
		SM.cameraList[cameraIndex].rightDirection.y = camera.rightDirection().y;
		SM.cameraList[cameraIndex].rightDirection.z = camera.rightDirection().z;

		SM.cameraList[cameraIndex].upDirection.x = camera.upDirection().x;
		SM.cameraList[cameraIndex].upDirection.y = camera.upDirection().y;
		SM.cameraList[cameraIndex].upDirection.z = camera.upDirection().z;

		SM.cameraList[cameraIndex].verticalFieldOfView = camera.verticalFieldOfView();

		SM.cameraList[cameraIndex].viewDirection.x = camera.viewDirection().x;
		SM.cameraList[cameraIndex].viewDirection.y = camera.viewDirection().y;
		SM.cameraList[cameraIndex].viewDirection.z = camera.viewDirection().z;

		///////////////////////	CAMERA TRANSFORM
		if(camera.parent(0, &status).hasFn(MFn::kTransform))
		{
			MFnTransform transform = camera.parent(0, &status);

			transform.getScale(scale);
			transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

			SM.cameraList[cameraIndex].transformation.position.x = transform.getTranslation(space_transform).x;
			SM.cameraList[cameraIndex].transformation.position.y = transform.getTranslation(space_transform).y;
			SM.cameraList[cameraIndex].transformation.position.z = transform.getTranslation(space_transform).z;

			SM.cameraList[cameraIndex].transformation.scale.x = scale[0];
			SM.cameraList[cameraIndex].transformation.scale.y = scale[1];
			SM.cameraList[cameraIndex].transformation.scale.z = scale[2];

			SM.cameraList[cameraIndex].transformation.rotation.x = rotX;
			SM.cameraList[cameraIndex].transformation.rotation.y = rotY;
			SM.cameraList[cameraIndex].transformation.rotation.z = rotZ;
			SM.cameraList[cameraIndex].transformation.rotation.w = rotW;
		}
	}
}

void GetMaterialNode(MObject &shading_engine, MFnDependencyNode &out_material_node)
{
	MStatus status = MS::kSuccess;

	MFnDependencyNode shading_engine_node(shading_engine);
	MPlug surface_shader_plug = shading_engine_node.findPlug("surfaceShader", &status);

	if (!surface_shader_plug.isNull(&status))
	{   
		MPlugArray materials;

		bool as_dest = true;
		bool as_source = false;
		surface_shader_plug.connectedTo(materials, as_dest, as_source, &status);

		if (materials.length() <= 0)
		{
			
		}
		else
		{
			MObject material_object(materials[0].node(&status));
			out_material_node.setObject(material_object);
		}
	}
}

void ExtractColor(MFnDependencyNode &material_node, MString &out_color_path, MObject &out_materialObject)
{
	MStatus status = MS::kSuccess;;
	//Get the texture paths.

	MPlug color_plug;
	MPlugArray connections;
	color_plug = material_node.findPlug("color", true, &status);
	color_plug.connectedTo(connections, true, false, &status);

	bool found_color = false;

	for (unsigned int n = 0; n < connections.length(); ++n)
	{
		if (connections[n].node(&status).hasFn(MFn::kFileTexture))
		{
			found_color = true;
			MFnDependencyNode texture_node(connections[n].node(&status));

			//MObject FileNode;
			//texture_node.setObject(FileNode);
			

			MPlug ftn = texture_node.findPlug("ftn", &status);
			out_color_path = ftn.asString(MDGContext::fsNormal);
			//out_materialObject = FileNode;

			break;
		}
	}

	if (!found_color)
	{
		out_color_path = "NONE";
	}
}

void ExtractBump(MFnDependencyNode &material_node, MString &out_bump_path, float &out_bump_depth)
{
	MStatus status = MS::kSuccess;;
	
	MPlug normal_plug;
	MPlugArray connections;
	normal_plug = material_node.findPlug("normalCamera", true, &status);
	normal_plug.connectedTo(connections, true, false, &status);


	bool found_bump = false;

	for(unsigned int i = 0; i < connections.length(); ++i){
		if(connections[i].node(&status).hasFn(MFn::kBump)){
			found_bump = true;

			
			MFnDependencyNode test_node(connections[i].node(&status));
			
			// Bump Depth value
			float bump_depth;
			MPlug bumpDepth = test_node.findPlug("bumpDepth", &status);
			bumpDepth.getValue(bump_depth);
			out_bump_depth = bump_depth;


			// Bump path
			MPlug bump_value = test_node.findPlug("bumpValue", &status);
			MPlugArray bv_connections;

			bump_value.connectedTo(bv_connections, true, false, &status);
			for(unsigned int j=0; j<bv_connections.length(); ++j){
				if(bv_connections[j].node(&status).hasFn(MFn::kFileTexture)){
					
					MFnDependencyNode test1(bv_connections[i].node(&status));

					MPlug ftn = test1.findPlug("ftn", &status);
					out_bump_path = ftn.asString(MDGContext::fsNormal);
				}

			}
			break;
		}

	}


	if (!found_bump)
	{
		out_bump_depth = 0;
		out_bump_path = "NONE";
	}
}

void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth, MFnDependencyNode &material_node, MObject &out_materialNode)
{
	//get the shaders and goes through the functions extracting textures.
	MStatus status = MS::kSuccess;;

	MObjectArray shaders;
	MIntArray face_indices;

	int parentCount = mesh.parentCount(&status);

	for(int i = 0; i < parentCount; i++)
	{
		mesh.getConnectedShaders(i, shaders, face_indices);
		
		for(int j = 0; j < shaders.length(); j++)
		{
			GetMaterialNode(shaders[j], material_node);
			ExtractColor(material_node, out_color_path, out_materialNode);
			ExtractBump(material_node, out_bump_path, out_bump_depth);			
		}
	}
}

MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle )
{
	//Get the local indexes for the triangle
  MIntArray   localIndex;
  unsigned    gv, gt;
  assert ( getTriangle.length() == 3 );    // Should always deal with a triangle

  for ( gt = 0; gt < getTriangle.length(); gt++ )
  {
    for ( gv = 0; gv < getVertices.length(); gv++ )
    {
      if ( getTriangle[gt] == getVertices[gv] )
      {
        localIndex.append( gv );
        break;
      }
    }

    // if nothing was added, add default "no match"
    if ( localIndex.length() == gt )
      localIndex.append( -1 );
  }

  return localIndex;
}



////////////////////////////	UNINITALIZE PLUGIN //////////////////////////////////////////
// Avinitiera pluginet.
EXPORT MStatus uninitializePlugin(MObject obj)
{
	// Avregistrera plugin:et.
	MFnPlugin plugin(obj);

	// Ta bort alla registrerade callbacks innan plug-in:et stängs!
	// Maya kraschar förmodligen annars!
	MMessage::removeCallbacks(g_callback_ids);


	return MS::kSuccess;
}