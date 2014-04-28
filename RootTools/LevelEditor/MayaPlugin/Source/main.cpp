#include "stdafx.h"
#include "maya_includes.h"

#include "SharedMemory.h"
#include <algorithm>
#include "Constants.h"
#include <sstream>;

// Threshold for determining if two vertices are identical
#define THRESHOLD 0.001f

SharedMemory SM;

// Globala variabler.
MCallbackIdArray g_callback_ids;
MObject g_selectedObject;
MSelectionList selected;

MObject g_objectList[g_maxSceneObjects], g_mayaMeshList[g_maxMeshes], g_mayaCameraList[g_maxCameras], 
		g_mayaLightList[g_maxLights], g_mayaMaterialList[g_maxMeshes], g_mayaLocatorList[g_maxLocators],
		g_mayaAmbientLight, g_mayaDirectionalLight, g_mayaMegaMeshList[g_maxMegaMeshes];

int currNrSceneObjects=0, currNrMeshes=0, currNrLights=0, currNrCameras=0, currNrMaterials = 0, currNrMaterialObjects = 0, currNrLocators = 0, currNrMegaMeshes = 0;

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
void MayaLocatorToList(MObject object);
void DuplicationCb(void *clientData);
void checkForDuplicatedMeshes(MObject node);
void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth, MFnDependencyNode &material_node, MString &out_specular_path, MFnDependencyNode &out_textureNode, MString &out_glow_path, MString &out_translucence_path);
MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle );
void Export();
int exportMaya;

void PaintModel(int index, MImage& texture, MString Red, MString Green, MString Blue, int _tileFactor, bool painting);
int paintCount = 0;
void ExtractRGBTextures(MFnDependencyNode &material_node, MString &Red, MString &Green, MString &Blue, int &tileFactor);
MString cleanFullPathName(const char * str);
std::string GetNameFromPath( std::string p_path );
void UpdateTransformation(Transform& destination, MObject transObject);
void ExtractTranslucence(MFnDependencyNode &material_node, MString &out_translucence_path);

PaintTexture myTextures[g_maxPaintTextures];
MSpace::Space g_space_world = MSpace::kPostTransform;
MSpace::Space g_space_local = MSpace::kPreTransform;

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

		id = MModelMessage::addAfterDuplicateCallback(DuplicationCb, nullptr, &status);
		AddCallbackID(status, id);	//Unresolved external symbol
	}

	return status;
}

void GivePaintId(int index, string filePath)
{
	bool painted = false;
	bool painting = false;

	if(SM.meshList[index].transformation.flags._PaintStatus == 0)
		painting = true;
	else if(SM.meshList[index].transformation.flags._PaintStatus == 1)
		painted = true;

	if (filePath == "PaintTexture" || painted || painting)
	{
		if(SM.meshList[index].paintIndex == -1)
		{
			//Print(SM.meshList[index].modelName, " given paintID ", paintCount);
			SM.meshList[index].paintIndex = paintCount;

			paintCount++;
		}
		else
		{
			//Print("Already has paintID!");
		}
	}
}

void GivePaintIdToMegaMesh(int index, string filePath)
{
	bool painted = false;
	bool painting = false;

	if(SM.PmegaMeshes[index]->transformation.flags._PaintStatus == 0)
		painting = true;
	else if(SM.PmegaMeshes[index]->transformation.flags._PaintStatus == 1)
		painted = true;

	if (filePath == "PaintTexture" || painted || painting)
	{
		if(SM.PmegaMeshes[index]->paintIndex == -1)
		{
			//Print(SM.meshList[index].modelName, " given paintID ", paintCount);
			SM.PmegaMeshes[index]->paintIndex = paintCount;

			//paintCount++;
		}
		else
		{
			//Print("Already has paintID!");
		}
	}
}

void PaintModel(int paintIndex, MImage& texture, MString Red, MString Green, MString Blue, int _tileFactor, bool painting)
{
	HANDLE TextureMutexHandle;
	DWORD milliseconds;
	//Print(paintCount);
	//SM.materialList[SM.meshList[index].MaterialID]
	//Hämtar ut vilken texturnod som gäller och kopplar
		//paintTexture.readFromTextureNode(
		unsigned int x,y;
		texture.getSize(x,y);
		//Print("Painting");
		//Print("SIZE X Y ", x, " ", y);

		TextureMutexHandle = CreateMutex(nullptr, false, L"TextureMutex");
		WaitForSingleObject(TextureMutexHandle, SM.milliseconds);
		//Print("Done");

		SM.PpaintList[paintIndex]->heigth = y;
		SM.PpaintList[paintIndex]->width = x;

		if(painting)
		{
			for(int i = 0; i < x*y*4; i++)
			{
				myTextures[paintIndex].Pixels[i] = texture.pixels()[i];
			}
			memcpy(SM.PpaintList[paintIndex]->Pixels, myTextures[paintIndex].Pixels, x*y*4);

		}

		if(Red != "")
			memcpy(SM.PpaintList[paintIndex]->textureRed, Red.asChar(), g_maxNameLength);
		else
			memcpy(SM.PpaintList[paintIndex]->textureRed, g_textureR.c_str(), g_maxNameLength);

		if(Green != "")
			memcpy(SM.PpaintList[paintIndex]->textureGreen, Green.asChar(), g_maxNameLength);
		else
			memcpy(SM.PpaintList[paintIndex]->textureGreen, g_textureG.c_str(), g_maxNameLength);

		if(Blue != "")
			memcpy(SM.PpaintList[paintIndex]->textureBlue, Blue.asChar(), g_maxNameLength);
		else
			memcpy(SM.PpaintList[paintIndex]->textureBlue, g_textureB.c_str(), g_maxNameLength);

		if(_tileFactor != 0)
			SM.PpaintList[paintIndex]->tileFactor = _tileFactor;
		else
			SM.PpaintList[paintIndex]->tileFactor = g_tileFactor;
	
		*SM.NumberOfPaintTextures = paintCount;
		ReleaseMutex(TextureMutexHandle);

		SM.AddUpdateMessage("Texture", paintIndex, false, false, false);
		/*När man skapar en textur så ska man titta på om nament på materialet är paint (som avgör om vi ska måla den eller inte),
		och i så fall ska vi lägga den som painttexture, sen tar vi en dirty node cb på den. och när det händer får vi skicka över pixel datan till
		andra sidan*/

		//texture.release();
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void copyTexture(string saveToDir, string texturePath)
{
	string textureName = GetNameFromPath(texturePath) + ".dds";			
	string newTexturePath = saveToDir + textureName;
	//FALSE = overwrites
	Print("Copying", s2ws(texturePath).c_str(), " to ", s2ws(newTexturePath).c_str());
	CopyFile(s2ws(texturePath).c_str(), s2ws(newTexturePath).c_str(), FALSE);
}

float getLengthOfVector(glm::vec3 myVector1, glm::vec3 myVector2)
{
	return sqrt((myVector1.x - myVector2.x)*(myVector1.x - myVector2.x) + (myVector1.y - myVector2.y)*(myVector1.y - myVector2.y) + (myVector1.z - myVector2.z)*(myVector1.z - myVector2.z));
}

void Export()
{	
	for(int i = 0; i < currNrMeshes; i++)
	{
		//Print("Index ", i, " ", SM.meshList[i].transformation.name);
		int count = 0;
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
				int countVertex = 0;
				if(SM.meshList[i].nrOfVertices == SM.meshList[j].nrOfVertices)
				{
					for(int v = 0; v < SM.meshList[i].nrOfVertices; v++)
					{
						//if(SM.meshList[i].vertex[v] != SM.meshList[j].vertex[v])	//Ska hoppa ut när den hittar en identisk.
						// (SM.meshList[i].vertex[v] - SM.meshList[j].vertex[v]).length();
						float dist = getLengthOfVector(SM.meshList[i].vertex[v], SM.meshList[j].vertex[v]);
						if (dist >= THRESHOLD)
						{
							//Om den är falsk hela vägen
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
			//Print("Overwriting: ", SM.meshList[i].modelName, " index: ", i, " with ", SM.meshList[saveJ].modelName, " index: ", saveJ);
			memset(SM.meshList[i].modelName, NULL, sizeof(SM.meshList[i].modelName));
			memcpy(SM.meshList[i].modelName, SM.meshList[saveJ].modelName, mesh.name().numChars());
			SM.UpdateSharedMesh(i, true, false, currNrMeshes);
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

			ExportFunction = "file -force -options \"bakeTransforms=0;relativePaths=0;copyTextures=0;exportTriangles=1;cgfxFileReferences=1;isSampling=0;curveConstrainSampling=0;removeStaticCurves=1;exportPolygonMeshes=1;exportLights=0;exportCameras=0;exportJointsAndSkin=0;exportAnimations=0;exportInvisibleNodes=0;exportDefaultCameras=0;exportTexCoords=1;exportNormals=1;exportNormalsPerVertex=1;exportVertexColors=0;exportVertexColorsPerVertex=0;exportTexTangents=1;exportTangents=0;exportReferencedMaterials=0;exportMaterialsOnly=0;exportXRefs=0;dereferenceXRefs=1;exportCameraAsLookat=0;cameraXFov=0;cameraYFov=1;doublePrecision=0;\" -typ \"OpenCOLLADA exporter\" -pr -es \"" + savepath + name + ".dae\"";
			
			//Copy textures
			string saveToDir = g_savepath + "Textures/";
			//DIFFUSE
			string texturePath = SM.materialList[SM.meshList[i].MaterialID].texturePath;

			copyTexture(saveToDir, SM.materialList[SM.meshList[i].MaterialID].texturePath);
			copyTexture(saveToDir, SM.materialList[SM.meshList[i].MaterialID].normalPath);
			copyTexture(saveToDir, SM.materialList[SM.meshList[i].MaterialID].specularPath);
			copyTexture(saveToDir, SM.materialList[SM.meshList[i].MaterialID].glowPath);
			copyTexture(saveToDir, SM.materialList[SM.meshList[i].MaterialID].translucencePath);

			//MString textureName = cleanFullPathName(texturePath.c_str());			
			//string newTexturePath = savToDir + textureName.asChar();
			////FALSE = overwrites
			//CopyFile(s2ws(texturePath).c_str(), s2ws(newTexturePath).c_str(), FALSE);


			MGlobal::executeCommand(ExportFunction);
			Print(ExportFunction);
		}


	}

	//EXPORT ALL MEGAMESHES WITHOUT CHECKING FOR DUPLICATES
	for(int i = 0; i < currNrMegaMeshes; i++)
	{
		SM.LockMeshMutex();
		MString name, realName, ExportFunction;
		name = SM.PmegaMeshes[i]->modelName;

		MFnMesh mesh = g_mayaMegaMeshList[i];
		realName = mesh.fullPathName();

		MGlobal::executeCommand("select -r " + realName);

		string outputDirectory = g_savepath + "Models/";
		MString savepath = outputDirectory.c_str();

		ExportFunction = "file -force -options \"bakeTransforms=0;relativePaths=0;copyTextures=0;exportTriangles=1;cgfxFileReferences=1;isSampling=0;curveConstrainSampling=0;removeStaticCurves=1;exportPolygonMeshes=1;exportLights=0;exportCameras=0;exportJointsAndSkin=0;exportAnimations=0;exportInvisibleNodes=0;exportDefaultCameras=0;exportTexCoords=1;exportNormals=1;exportNormalsPerVertex=1;exportVertexColors=0;exportVertexColorsPerVertex=0;exportTexTangents=1;exportTangents=0;exportReferencedMaterials=0;exportMaterialsOnly=0;exportXRefs=0;dereferenceXRefs=1;exportCameraAsLookat=0;cameraXFov=0;cameraYFov=1;doublePrecision=0;\" -typ \"OpenCOLLADA exporter\" -pr -es \"" + savepath + name + ".dae\"";

		//Copy textures
		string saveToDir = g_savepath + "Textures/";
		//DIFFUSE
		string texturePath = SM.materialList[SM.PmegaMeshes[i]->MaterialID].texturePath;

		copyTexture(saveToDir, SM.materialList[SM.PmegaMeshes[i]->MaterialID].texturePath);
		copyTexture(saveToDir, SM.materialList[SM.PmegaMeshes[i]->MaterialID].normalPath);
		copyTexture(saveToDir, SM.materialList[SM.PmegaMeshes[i]->MaterialID].specularPath);
		copyTexture(saveToDir, SM.materialList[SM.PmegaMeshes[i]->MaterialID].glowPath);

		MGlobal::executeCommand(ExportFunction);
		Print(ExportFunction);
		SM.UnLockMeshMutex();
	}

	SM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(SM.IdMutexHandle, SM.milliseconds);

	*SM.export = 2;
	exportMaya = 0;

	ReleaseMutex(SM.IdMutexHandle);

	//GET BACCK OLD NAMES?
	//loadScene();
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
			if(tempMesh.numPolygons() * 3 < g_maxVerticesPerMesh)
			{
				g_mayaMeshList[currNrMeshes] = g_objectList[i];
				MayaMeshToList(g_mayaMeshList[currNrMeshes],currNrMeshes, true, true, true);
				currNrMeshes ++;
			}
			else if(tempMesh.numPolygons() * 3 < g_maxVerticesPerMegaMesh)
			{
				g_mayaMegaMeshList[currNrMegaMeshes] = g_objectList[i];
				currNrMegaMeshes++;
			}
		}
		else if(g_objectList[i].hasFn(MFn::kCamera))
		{
			MFnCamera tempCam = g_objectList[i];
			g_mayaCameraList[currNrCameras] = g_objectList[i];
			MayaCameraToList(g_mayaCameraList[currNrCameras], currNrCameras);	
			currNrCameras++;
		}
		else if(g_objectList[i].hasFn(MFn::kPointLight))
		{
			MFnLight tempLight = g_objectList[i];
			g_mayaLightList[currNrLights] = g_objectList[i];
			MayaLightToList(g_mayaLightList[currNrLights], currNrLights);		

			currNrLights++;
		}
		else if(g_objectList[i].hasFn(MFn::kAmbientLight))
		{
			g_mayaAmbientLight = g_objectList[i];
			MayaLightToList(g_mayaAmbientLight, currNrLights);
		}
		else if(g_objectList[i].hasFn(MFn::kDirectionalLight))
		{
			g_mayaDirectionalLight = g_objectList[i];
			MayaLightToList(g_mayaDirectionalLight, currNrLights);
		}
		else if(g_objectList[i].hasFn(MFn::kLambert))
		{
			MFnLambertShader Lambert = g_objectList[i];
			g_mayaMaterialList[currNrMaterialObjects] = g_objectList[i];

			//Print("Adding CB to ", Lambert.name());
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_objectList[i], dirtyMaterialCB, nullptr, &status);
			AddCallbackID(status, id);

			currNrMaterialObjects++;
		}
		else if(g_objectList[i].hasFn(MFn::kLocator))
		{
			g_mayaLocatorList[currNrLocators] = g_objectList[i];
			currNrLocators++;
			MayaLocatorToList(g_objectList[i]);

			MFnDagNode locator = g_objectList[i];

			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(locator.parent(0), dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
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
		//SM.UpdateSharedMesh(i, true, true, currNrMeshes);
		//DONE correct in ^^
	}

	for(int i = 0; i < currNrMegaMeshes; i++)
	{
		MFnMesh mesh = g_mayaMegaMeshList[i];

		if(mesh.parent(0, &status).hasFn(MFn::kTransform))
		{
			MObject transform = mesh.parent(0, &status);
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}
		if(g_mayaMeshList[i].hasFn(MFn::kMesh))
		{
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaMegaMeshList[i], dirtyMeshNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}

		SM.LockMeshMutex();
		SM.PmegaMeshes[i]->MaterialID = -1;
		SM.PmegaMeshes[i]->paintIndex = -1;
		SM.UnLockMeshMutex();
		MayaMeshToList(g_mayaMegaMeshList[i], i, true, true, true);
		//SM.UpdateSharedMesh(i, true, true, currNrMeshes);
		//DONE correct in ^^
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
		//SM.UpdateSharedLight(i, currNrLights);
		//Done correct in ^^
	}

	MFnLight ambient = g_mayaAmbientLight;
	if(g_mayaAmbientLight.hasFn(MFn::kAmbientLight))
	{
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaAmbientLight, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}

	if(ambient.parent(0,&status).hasFn(MFn::kTransform))
	{
		MObject transform = ambient.parent(0, &status);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
	MayaLightToList(g_mayaAmbientLight, -1);

	MFnLight directional = g_mayaDirectionalLight;
	if(g_mayaAmbientLight.hasFn(MFn::kDirectionalLight))
	{
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaDirectionalLight, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}

	if(directional.parent(0,&status).hasFn(MFn::kTransform))
	{
		MObject transform = directional.parent(0, &status);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
	MayaLightToList(g_mayaDirectionalLight, -1);

	//Camera transformation CB
	for(int i = 0; i < currNrCameras; i++)
	{
		MFnCamera camera = g_mayaCameraList[i];
		
		std::ostringstream oss;
		oss << "modelPanel" << i+1;
		
		const MString temp = oss.str().c_str();
			
		MCallbackId id = MUiMessage::add3dViewPostRenderMsgCallback(temp, viewCB, nullptr, &status);		
		AddCallbackID(status, id);

		MayaCameraToList(g_mayaCameraList[i], i);
		//SM.UpdateSharedCamera(i);
		//Done correct in ^^
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

void DuplicationCb(void* clientdata)
{
	MGlobal::getActiveSelectionList(selected);

	MObject temp;

	for(int i = 0; i < selected.length()+1; i++)
	{
		selected.getDependNode(i,temp);

		if(temp.hasFn(MFn::kTransform))
		{
		MFnTransform tempTrans = temp; 
		//Print (tempTrans.name());
		checkForDuplicatedMeshes(tempTrans.child(0));
		checkForNewCameras(tempTrans.child(0), nullptr);
		//checkForNewLights(tempTrans.child(0), nullptr);		// NO NEEEAAAAAAD FOOOUUUUL
		}

	}
}

////////////////////////////	LOOK IF A MESH NODE IS DIRTY  //////////////////////////////////////////
void dirtyMeshNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	//Print("dirtyMeshNodeCB");

	int index = nodeExists(node);

	if(index != -1)
	{
		MayaMeshToList(node, index, false, true, true);
		//SM.UpdateSharedMesh(index, true, true, currNrMeshes);
	}
}

void dirtyLightNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	//Print("dirtyLightNode");
	MStatus status;
	MFnLight Light = node;

	int index = nodeExists(node);
	if(index != -1)
	{
		MayaLightToList(node, index);
		//SM.UpdateSharedLight(index, currNrLights);
	}

	if(node.hasFn(MFn::kAmbientLight) || node.hasFn(MFn::kDirectionalLight))
		MayaLightToList(node, index);
}

////////////////////////////	LOOK IF A TRANSFORMATION NODE IS DIRTY  //////////////////////////////////////////
void getMeshFlags(MObject object, Transform& transformation)
{
	if(object.hasFn(MFn::kTransform))
	{
		MFnTransform trans = object;
		//Print("Getting flags for: ", SM.meshList[index].modelName);

		for(int i = 0; i < trans.parentCount(); i++)
		{
			if(trans.parent(i).hasFn(MFn::kTransform))
			{
				MFnTransform temp = trans.parent(i);
				//Print("Found parent: ", temp.name());

				bool alreadyExists = false;
				string name = temp.name().asChar();

				if(name == "Hazard")
					transformation.flags._Hazard = true;
				if(name == "Painting")
					transformation.flags._PaintStatus = 0;
				if(name == "Painted")
					transformation.flags._PaintStatus = 1;
				if(name == "Static")
					transformation.flags._Static = true;
				if(name == "Transparent")
					transformation.flags._Transparent = true;
				if(name == "Water")
					transformation.flags._Water = true;
				if(name == "NoRender")
					transformation.flags._NoRender = true;
				if(name == "Grass")
					transformation.flags._Grass = true;

				getMeshFlags(trans.parent(i), transformation);
			}
		}
	}
}

void getLocatorFlags(MObject object, int index)
{
	if(object.hasFn(MFn::kTransform))
	{
		MFnTransform trans = object;
		//Print("Getting flags for: ", SM.locatorList[index].transformation.name);

		for(int i = 0; i < trans.parentCount(); i++)
		{
			if(trans.parent(i).hasFn(MFn::kTransform))
			{
				MFnTransform temp = trans.parent(i);
				//Print("Found parent: ", temp.name());

				bool alreadyExists = false;
				string name = temp.name().asChar();

				if(name == "Particle")
					SM.locatorList[index].transformation.flags._Particle = true;
				if(name == "SpawnPoint")
					SM.locatorList[index].transformation.flags._SpawnPoint = true;

				if(name == "AbilitySpawnPoint")
					SM.locatorList[index].transformation.flags._AbilitySpawnPoint = true;

				if(name == "Water")
					SM.locatorList[index].transformation.flags._Water = true;

				getLocatorFlags(trans.parent(i), index);
			}
		}
	}
}

void dirtyTransformNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	//Print("dirtyTransformNodeCB");
	
	MStatus status = MS::kSuccess;
	MFnTransform trans = node;

	int index = nodeExists(trans.child(0, &status));

	if(index != -1)
	{

		if(trans.child(0, &status).hasFn(MFn::kMesh))
		{
			MayaMeshToList(trans.child(0, &status), index, true, true, false);
		}

		if(trans.child(0, &status).hasFn(MFn::kPointLight))
		{
			MayaLightToList(trans.child(0, &status), index);
			//SM.UpdateSharedLight(index, currNrLights);
		}

	}

	if(trans.child(0, &status).hasFn(MFn::kAmbientLight) || trans.child(0, &status).hasFn(MFn::kDirectionalLight))
	{
		MayaLightToList(trans.child(0, &status), 0);
		//SM.UpdateSharedLight(index, currNrLights);
	}

	if(trans.child(0, &status).hasFn(MFn::kLocator))
	{		
		//Print("U moved a locator! Index ", nodeExists(trans.child(0)));
		MayaLocatorToList(trans.child(0));
	}
}



void dirtyMaterialCB(MObject &node, MPlug &plug, void *clientData)
{// Reagerar precis INNAN man bytt textur!
	//Print("dirtyMaterialCB");

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
		//	Print("Comparing", temp.c_str(), " and ", materialName.c_str());
			if(temp == materialName)
			{
				MFnMesh mesh = g_mayaMeshList[i];

			//	Print("Updating ", materialName.c_str(), " and ", mesh.fullPathName(), " at index ", i);
				MayaMeshToList(g_mayaMeshList[i], i, true, true, false);
				SM.UpdateSharedMaterials(currNrMaterials, i, false);

				//SM.UpdateSharedMesh(i, true, false, currNrMeshes);
			}
		}

		SM.LockMeshMutex();
		for(int i = 0; i < currNrMegaMeshes; i++)
		{

			string temp = SM.PmegaMeshes[i]->materialName;
			//	Print("Comparing", temp.c_str(), " and ", materialName.c_str());
			if(temp == materialName)
			{
				MFnMesh mesh = g_mayaMeshList[i];

				//	Print("Updating ", materialName.c_str(), " and ", mesh.fullPathName(), " at index ", i);
				MayaMeshToList(g_mayaMegaMeshList[i], i, true, true, false);
				SM.UpdateSharedMaterials(currNrMaterials, i, false);

				//SM.UpdateSharedMesh(i, true, false, currNrMeshes);
			}
		}
		SM.UnLockMeshMutex();
	}

	MStatus status = MS::kSuccess;	
}

////////////////////////////	SORT OUT WHAT TYPE THE NEW NODE ARE //////////////////////////////////////////
void NodeAddedCB(MObject &node, void *clientData)
{
//	Print("NodeAddedCB");

	//Callback when node is added.
	checkForNewCameras(node, clientData);
	checkForNewLights(node, clientData);

	if(node.hasFn(MFn::kLocator))
	{
		MStatus status;
		g_mayaLocatorList[currNrLocators] = node;
		MayaLocatorToList(node);

		MFnDagNode locator = node;

		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(locator.parent(0), dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
	//printLists();
}

void NodeRemovedCB(MObject &node, void *clientData)
{
//	Print("NodeRemovedCB");

	//Callback when node is removed
	int removeID = 0;

	if(node.hasFn(MFn::kMesh))
	{
	//	Print("Removed a mesh!");

		MFnMesh mesh = node;

		for(int i = 0; i < currNrMeshes; i++)
		{
			MFnMesh listMesh = g_mayaMeshList[i];
			if(listMesh.name() == mesh.name())
			{
			//	Print("Mesh ", mesh.name(), " at index ", i);
				removeID = i;
				currNrMeshes--;				

				for(int j = i; j < currNrMeshes; j++)
				{
					g_mayaMeshList[j] = g_mayaMeshList[j+1];
					MayaMeshToList(g_mayaMeshList[j], j, true, true, true);
					//SM.UpdateSharedMesh(j, true, true, currNrMeshes);					
				}
					
				SM.RemoveMesh(removeID, currNrMeshes);
				//g_mayaMeshList[currNrMeshes].~MObject();					//Will crash if it reaches g_maxMeshes

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
				currNrLights--;

				for(int j = i; j < currNrLights; j++)
				{
					g_mayaLightList[j] = g_mayaLightList[j+1];
					MayaLightToList(g_mayaLightList[j], j);					
				}
				
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
	
	//Print("-_-_-_-_-_-_-_ ConnectionCB _-_-_-_-_-_-_-_-_-_-_");
	//Print("source is ", source.apiTypeStr());
	//Print("destination is ", destination.apiTypeStr());
	//Print("-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");

	if (Status(__LINE__, status1) && Status(__LINE__, status2))
	{
		checkForNewMeshes(made, source, destination);

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
				//Print("Adding CB to ", lambert.name());
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

void checkForDuplicatedMeshes(MObject node)
{
	MCallbackId id;
	MStatus status;

	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = node;
		g_mayaMeshList[currNrMeshes] = node;

		id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaMeshList[currNrMeshes], dirtyMeshNodeCB, nullptr, &status);
		AddCallbackID(status, id);


		if(mesh.parent(0, &status).hasFn(MFn::kTransform))
		{
			MObject transform = mesh.parent(0, &status);

			id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);

			MayaMeshToList(node, currNrMeshes, true, true, true);
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
		//Print("New Camera Added!");
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

	if(node.hasFn(MFn::kPointLight))
	{
		//Print("New Light Added");
		g_mayaLightList[currNrLights] = node;
		MayaLightToList(node, currNrLights);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(node, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
		currNrLights++;
	}

	if(node.hasFn(MFn::kDirectionalLight))
	{
		g_mayaDirectionalLight = node;
		MayaLightToList(node, currNrLights);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(node, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
	
	if(node.hasFn(MFn::kAmbientLight))
	{
		g_mayaAmbientLight = node;
		MayaLightToList(node, currNrLights);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(node, dirtyLightNodeCB, nullptr, &status);
		AddCallbackID(status, id);
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

		for(int i = 0; i < currNrMegaMeshes; i++)
		{
			MFnMesh temp = g_mayaMegaMeshList[i];

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
			//Print("nodeExists() Comparing ", temp.name(), " and ", lambert.name());
			if(temp.name() == lambert.name())
			{
				answer = i;
				break;
			}
		}	
	}

	if (node.hasFn(MFn::kLocator))
	{
		MFnDagNode locator = node;

		MFnTransform locTrans = locator.parent(0);
		for(int i = 0; i < currNrLocators; i++)
		{
			MFnDagNode temp = g_mayaLocatorList[i];
			MFnTransform tempTrans = temp.parent(0);

			if(tempTrans.name() == locTrans.name())
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
	char chars[] = "| _:";
	for (unsigned int i = 0; i < strlen(chars); ++i)
	{
		tmpString.erase(std::remove(tmpString.begin(), tmpString.end(), chars[i]), tmpString.end());
	}

	MString TEMP = tmpString.c_str();
	return TEMP;
}

std::string GetNameFromPath( std::string p_path )
{
	std::string cutPath;
	std::string::size_type slashIndex, dotIndex;

	// Extract the file name
	cutPath  = p_path;
	slashIndex = cutPath.find_last_of("/")+1;
	if(slashIndex == 0)
		slashIndex = cutPath.find_last_of("\\")+1;
	cutPath  = cutPath.substr(slashIndex, cutPath.size());
	dotIndex = cutPath.find_last_of(".");
	cutPath  = cutPath.substr(0, dotIndex);
	return cutPath;
} 

////////////////////////////	ADD THE NODES TO THE SHARED LIST	 //////////////////////////////////////////
void MayaLocatorToList(MObject object)
{
	MFnDagNode locator = object;
	int index = nodeExists(object);
//	Print("LOCATOR INDEX: ", nodeExists(object));

	if(index != -1 && locator.parent(0).hasFn(MFn::kTransform))
	{
		double scale[3];
		double rotX, rotY, rotZ, rotW;
		glm::vec3 position;

		MFnTransform trans = locator.parent(0);
		
		MObject transObj = trans.object();
		UpdateTransformation(SM.locatorList[index].transformation, transObj);
		memcpy(SM.locatorList[index].transformation.name, locator.name().asChar(), locator.name().numChars());

		getLocatorFlags(locator.parent(0), index);

		SM.UpdateSharedLocator(index, currNrLocators);
	}

}

void MayaMeshToList(MObject node, int meshIndex, bool doTrans, bool doMaterial, bool doMesh)
{
	//Get mesh and transformation information from the nodes. Used when editing a node / adding a new one.
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MVector rotPivot, scalePivot;
	MIntArray triangleCounts, triangleVertices;
	bool ItsAmegaMesh = false;

	SM.IdMutexHandle = CreateMutex(nullptr, false, L"IdMutex");
	WaitForSingleObject(SM.IdMutexHandle, SM.milliseconds);
	int numberOfMegaMeshes = *SM.NumberOfMegaMeshes;
	ReleaseMutex(SM.IdMutexHandle);

	SM.LockMeshMutex();
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
		MString specularName = "";
		MString glowName = "";
		MString translucenceName = "";
		float bumpdepth;
		MFnDependencyNode material_node;
		MFnDependencyNode texture_node;

		
		if(mesh.numPolygons()*3 > g_maxVerticesPerMesh)
		{
			ItsAmegaMesh = true;
			//Print(mesh.fullPathName(), " isaMEGAMESH---------------------------------------------------------------------------------------------------------");
		}

		//Get the name of the transformation and the model name for the exporter.
		MString transName = cleanFullPathName(mesh.fullPathName().asChar());
		MString name = cleanFullPathName(mesh.name().asChar());

		if(ItsAmegaMesh)
		{
			SM.PmegaMeshes[meshIndex]->nrOfVertices = 0;
			memcpy(SM.PmegaMeshes[meshIndex]->transformation.name, transName.asChar(), transName.numChars());		
			memcpy(SM.PmegaMeshes[meshIndex]->modelName, name.asChar(), name.numChars());
			MObject tempTrans = mesh.parent(0);
			getMeshFlags(tempTrans, SM.PmegaMeshes[meshIndex]->transformation);
		}
		else
		{
			memcpy(SM.meshList[meshIndex].transformation.name, transName.asChar(), transName.numChars());		
			memcpy(SM.meshList[meshIndex].modelName, name.asChar(), name.numChars());
			SM.meshList[meshIndex].nrOfVertices = 0;
			MObject tempTrans = mesh.parent(0);
			getMeshFlags(tempTrans, SM.meshList[meshIndex].transformation);
		}


		if(doMaterial)
		{/////////////////////  GET MATERIALS AND ADD TO LIST /////////////////////////////////////////////

		ExtractMaterialData(mesh, texturepath, normalpath, bumpdepth, material_node, specularName, texture_node, glowName, translucenceName);
		materialName = material_node.name();
		string texPath = texturepath.asChar();		

		MImage texture;

		bool painted = false;
		bool painting = false;

		if(ItsAmegaMesh)
		{
			if(SM.PmegaMeshes[meshIndex]->transformation.flags._PaintStatus == 0)
				painting = true;
			else if(SM.PmegaMeshes[meshIndex]->transformation.flags._PaintStatus == 1)
				painted = true;
		}
		else
		{
			if(SM.meshList[meshIndex].transformation.flags._PaintStatus == 0)
				painting = true;
			else if(SM.meshList[meshIndex].transformation.flags._PaintStatus == 1)
				painted = true;
		}


		//PAINT STUFF
		if(painted || painting)
		{		
			if(ItsAmegaMesh)
			{
				GivePaintIdToMegaMesh(meshIndex, texPath);
				if(SM.PmegaMeshes[meshIndex]->paintIndex >= 0)
				{
					MString Red, Green, Blue;
					int tileFactor;

					ExtractRGBTextures(material_node, Red, Green, Blue, tileFactor);

					if(painting)
					{
						MObject temp = texture_node.object();
						texture.readFromTextureNode(temp, MImage::MPixelType::kByte);
					}

					PaintModel(SM.PmegaMeshes[meshIndex]->paintIndex, texture, Red, Green, Blue, tileFactor, painting);

				}
			}
			else
			{
				GivePaintId(meshIndex, texPath);
				if(SM.meshList[meshIndex].paintIndex >= 0)
				{
					MString Red, Green, Blue;
					int tileFactor;

					ExtractRGBTextures(material_node, Red, Green, Blue, tileFactor);

					if(painting)
					{
						MObject temp = texture_node.object();
						texture.readFromTextureNode(temp, MImage::MPixelType::kByte);
					}

					PaintModel(SM.meshList[meshIndex].paintIndex, texture, Red, Green, Blue, tileFactor, painting);

				}
			}


		}

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
			memset(SM.materialList[currNrMaterials].specularPath, NULL, sizeof(SM.materialList[currNrMaterials].specularPath));
			memcpy(SM.materialList[currNrMaterials].specularPath, specularName.asChar(),specularName.numChars());
			memset(SM.materialList[currNrMaterials].glowPath, NULL, sizeof(SM.materialList[currNrMaterials].glowPath));
			memcpy(SM.materialList[currNrMaterials].glowPath, glowName.asChar(),glowName.numChars());
			memset(SM.materialList[currNrMaterials].translucencePath, NULL, sizeof(SM.materialList[currNrMaterials].translucencePath));
			memcpy(SM.materialList[currNrMaterials].translucencePath, translucenceName.asChar(),translucenceName.numChars());

			if(ItsAmegaMesh)
			{
				SM.PmegaMeshes[meshIndex]->MaterialID = currNrMaterials;
			}
			else
			{
				SM.meshList[meshIndex].MaterialID = currNrMaterials;
			}


			currNrMaterials++;
			
		}
		else
		{
			if(ItsAmegaMesh)
			{
				SM.PmegaMeshes[meshIndex]->MaterialID = materialID;
			}
			else
			{
				SM.meshList[meshIndex].MaterialID = materialID;
			}

			//Is this needed? and in that case copy all info?
			memset(SM.materialList[materialID].texturePath, NULL, sizeof(SM.materialList[materialID].texturePath));
			memcpy(SM.materialList[materialID].texturePath, texturepath.asChar(), texturepath.numChars());
			memset(SM.materialList[materialID].normalPath, NULL, sizeof(SM.materialList[materialID].normalPath));
			memcpy(SM.materialList[materialID].normalPath, normalpath.asChar(),normalpath.numChars());
			memset(SM.materialList[currNrMaterials].specularPath, NULL, sizeof(SM.materialList[currNrMaterials].specularPath));
			memcpy(SM.materialList[currNrMaterials].specularPath, specularName.asChar(),specularName.numChars());
		}

		if(ItsAmegaMesh)
		{
			memset(SM.PmegaMeshes[meshIndex]->materialName, NULL, sizeof(SM.meshList[meshIndex].materialName));
			memcpy(SM.PmegaMeshes[meshIndex]->materialName, materialName.asChar(), materialName.numChars());
		}
		else
		{
			memset(SM.meshList[meshIndex].materialName, NULL, sizeof(SM.meshList[meshIndex].materialName));
			memcpy(SM.meshList[meshIndex].materialName, materialName.asChar(), materialName.numChars());
		}

		SM.UpdateSharedMaterials(currNrMaterials, meshIndex, ItsAmegaMesh);
		//Print("Updated material ", materialName.asChar());

		}

		if(doMesh)
		{///////////////////////////////// GET VERTEX AND UV CONVERTED TO TRIANGLES //////////////////////////////////

		//Get points from mesh
		mesh.getPoints(points_, g_space_local);

		//Convert points to float array
		points_.get(floatPoints);

		MIntArray polygonVertices;
		MPoint UVpoint;
		float2 UV;

		MPoint TangentPoint;
		MVector tempTangent;
		MVector tempBiNormal;
		MFloatVectorArray tangentsArray;

		int triangleVertices[3];
		MVector normal;
		int count = 0;
		MFloatVectorArray normals;

		int uvID;
		//int tangentID;
		//int biNormalID;
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

			//mesh.getTangents(tangentsArray, space_local);
			//mesh.getBinormals(biNormalArray, space_local);

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
					
					
					if(ItsAmegaMesh)
					{
						SM.PmegaMeshes[meshIndex]->vertex[count].x = floatPoints[triangleVertices[j]][0];
						SM.PmegaMeshes[meshIndex]->vertex[count].y = floatPoints[triangleVertices[j]][1];
						SM.PmegaMeshes[meshIndex]->vertex[count].z = floatPoints[triangleVertices[j]][2];

						//mesh.getVertexNormal(localIndex[j], normal, space_local);
						//OLD

						normal = normals[normalIDs[localIndex[j]]];					

						SM.PmegaMeshes[meshIndex]->normal[count].x = (float)normal.x;
						SM.PmegaMeshes[meshIndex]->normal[count].y = normal.y;
						SM.PmegaMeshes[meshIndex]->normal[count].z = normal.z;

						mesh.getPolygonUVid(i, localIndex[j], uvID, 0);
						mesh.getUV(uvID, U, V, 0);

						SM.PmegaMeshes[meshIndex]->UV[count].x = U;
						SM.PmegaMeshes[meshIndex]->UV[count].y = 1-V;

						int herp = mesh.getTangentId(i, localIndex[j]);
						mesh.getFaceVertexBinormal(i, localIndex[j], tempBiNormal, g_space_local);

						//SM.meshList[meshIndex].tangent[count].x = tangentsArray[herp].x;
						//SM.meshList[meshIndex].tangent[count].y = tangentsArray[herp].y;
						//SM.meshList[meshIndex].tangent[count].z = tangentsArray[herp].z;

						mesh.getFaceVertexTangent(i, localIndex[j], tempTangent, g_space_local, 0);

						//Print("X: ",tangentsArray[herp].x, " " , tempTangent.x);

						SM.PmegaMeshes[meshIndex]->binormal[count].x = tempBiNormal.x;
						SM.PmegaMeshes[meshIndex]->binormal[count].y = tempBiNormal.y;
						SM.PmegaMeshes[meshIndex]->binormal[count].z = tempBiNormal.z;

						SM.PmegaMeshes[meshIndex]->tangent[count].x = tempTangent.x;
						SM.PmegaMeshes[meshIndex]->tangent[count].y = tempTangent.y;
						SM.PmegaMeshes[meshIndex]->tangent[count].z = tempTangent.z;

						count++;
						SM.PmegaMeshes[meshIndex]->nrOfVertices = count;
					}
					else
					{
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

						int herp = mesh.getTangentId(i, localIndex[j]);
						mesh.getFaceVertexBinormal(i, localIndex[j], tempBiNormal, g_space_local);

						//SM.meshList[meshIndex].tangent[count].x = tangentsArray[herp].x;
						//SM.meshList[meshIndex].tangent[count].y = tangentsArray[herp].y;
						//SM.meshList[meshIndex].tangent[count].z = tangentsArray[herp].z;

						mesh.getFaceVertexTangent(i, localIndex[j], tempTangent, g_space_local, 0);

						//Print("X: ",tangentsArray[herp].x, " " , tempTangent.x);

						SM.meshList[meshIndex].binormal[count].x = tempBiNormal.x;
						SM.meshList[meshIndex].binormal[count].y = tempBiNormal.y;
						SM.meshList[meshIndex].binormal[count].z = tempBiNormal.z;

						SM.meshList[meshIndex].tangent[count].x = tempTangent.x;
						SM.meshList[meshIndex].tangent[count].y = tempTangent.y;
						SM.meshList[meshIndex].tangent[count].z = tempTangent.z;

						count++;
						SM.meshList[meshIndex].nrOfVertices = count;
					}

				}
			}
		}

		}

		if(doTrans)
		{///////////////////////	MESH TRANSFORM ////////////////////////////////////////
			if(mesh.parent(0,&status).hasFn(MFn::kTransform))
			{
				MFnTransform transform = mesh.parent(0, &status);
				MObject transObj = transform.object();

				if(ItsAmegaMesh)
				{
					UpdateTransformation(SM.PmegaMeshes[meshIndex]->transformation, transObj);

				}
				else
				{
					UpdateTransformation(SM.meshList[meshIndex].transformation, transObj);
				}


			}
		}


	}

	if(ItsAmegaMesh)
	{
		numberOfMegaMeshes++;
		*SM.NumberOfMegaMeshes = numberOfMegaMeshes;
		SM.AddUpdateMessage("MegaMesh", meshIndex, doTrans, doMesh, false);

	}
	else
	{
		SM.UpdateSharedMesh(meshIndex, doTrans, doMesh, currNrMeshes);
	}

	SM.UnLockMeshMutex();
}

void UpdateTransformation(Transform& destination, MObject transObject)
{
	MFnTransform transform = transObject;

	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MVector rotPivot, scalePivot;

	transform.getScale(scale);
	transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);
	rotPivot = transform.rotatePivot(MSpace::kObject);
	scalePivot = transform.scalePivot(MSpace::kObject);

	destination.position.x = transform.getTranslation(g_space_world).x;
	destination.position.y = transform.getTranslation(g_space_world).y;
	destination.position.z = transform.getTranslation(g_space_world).z;

	destination.scale.x = scale[0];
	destination.scale.y = scale[1];
	destination.scale.z = scale[2];

	destination.rotation.x = rotX;
	destination.rotation.y = rotY;
	destination.rotation.z = rotZ;
	destination.rotation.w = rotW;

	destination.rotPivot.x = rotPivot.x;
	destination.rotPivot.y = rotPivot.y;
	destination.rotPivot.z = rotPivot.z;

	destination.scalePivot.x = scalePivot.x;
	destination.scalePivot.y = scalePivot.y;
	destination.scalePivot.z = scalePivot.z;

	getMeshFlags(transObject, destination);
}

void MayaLightToList(MObject node, int lightIndex)
{
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	const MString PL = "PointLight";
	const MString DL = "DirectionalLight";
	const MString AL = "AmbientLight";

	///////////////////////	LIGHT
	if(node.hasFn(MFn::kLight))
	{
		MFnLight light = node;
		memcpy(SM.lightList[lightIndex].transformation.name, light.fullPathName().asChar(), light.fullPathName().numChars());

		if(node.hasFn(MFn::kPointLight))
		{
			memcpy(SM.lightList[lightIndex].LightType, PL.asChar(), PL.numChars());

			SM.lightList[lightIndex].color.r = light.color(&status).r;
			SM.lightList[lightIndex].color.g = light.color(&status).g;
			SM.lightList[lightIndex].color.b = light.color(&status).b;
			SM.lightList[lightIndex].color.a = light.color(&status).a;
			SM.lightList[lightIndex].Intensity = light.intensity(&status);

			///////////////////////	LIGHT TRANSFORM
			if(light.parent(0, &status).hasFn(MFn::kTransform))
			{
				MFnTransform transform = light.parent(0, &status);

				MObject transObj = transform.object();
				UpdateTransformation(SM.lightList[lightIndex].transformation, transObj);

				/*transform.getScale(scale);
				transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

				SM.lightList[lightIndex].transformation.position.x = transform.getTranslation(g_space_world).x;
				SM.lightList[lightIndex].transformation.position.y = transform.getTranslation(g_space_world).y;
				SM.lightList[lightIndex].transformation.position.z = transform.getTranslation(g_space_world).z;

				SM.lightList[lightIndex].transformation.scale.x = scale[0];
				SM.lightList[lightIndex].transformation.scale.y = scale[1];
				SM.lightList[lightIndex].transformation.scale.z = scale[2];

				SM.lightList[lightIndex].transformation.rotation.x = rotX;
				SM.lightList[lightIndex].transformation.rotation.y = rotY;
				SM.lightList[lightIndex].transformation.rotation.z = rotZ;
				SM.lightList[lightIndex].transformation.rotation.w = rotW;*/
			}

			SM.UpdateSharedLight(lightIndex, currNrLights);
		}

		if(node.hasFn(MFn::kDirectionalLight))
		{
			SM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
			WaitForSingleObject(SM.LightMutexHandle, SM.milliseconds);

			memcpy(SM.worldData->DirectionalSun.LightType, DL.asChar(), DL.numChars());
			//SM.worldData->DirectionalSun.direction.x = light.lightDirection().x;
			//SM.worldData->DirectionalSun.direction.y = light.lightDirection().y;
			//SM.worldData->DirectionalSun.direction.z = light.lightDirection().z;

			SM.worldData->DirectionalSun.color.r = light.color(&status).r;
			SM.worldData->DirectionalSun.color.g = light.color(&status).g;
			SM.worldData->DirectionalSun.color.b = light.color(&status).b;
			SM.worldData->DirectionalSun.color.a = light.color(&status).a;
			SM.worldData->DirectionalSun.Intensity = light.intensity(&status);

			///////////////////////	LIGHT TRANSFORM
			if(light.parent(0, &status).hasFn(MFn::kTransform))
			{
				MFnTransform transform = light.parent(0, &status);

				MObject transObj = transform.object();
				UpdateTransformation(SM.worldData->DirectionalSun.transformation, transObj);
				//transform.getScale(scale);
				//transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

				//SM.worldData->DirectionalSun.transformation.position.x = transform.getTranslation(g_space_world).x;
				//SM.worldData->DirectionalSun.transformation.position.y = transform.getTranslation(g_space_world).y;
				//SM.worldData->DirectionalSun.transformation.position.z = transform.getTranslation(g_space_world).z;

				//SM.worldData->DirectionalSun.transformation.scale.x = scale[0];
				//SM.worldData->DirectionalSun.transformation.scale.y = scale[1];
				//SM.worldData->DirectionalSun.transformation.scale.z = scale[2];

				//SM.worldData->DirectionalSun.transformation.rotation.x = rotX;
				//SM.worldData->DirectionalSun.transformation.rotation.y = rotY;
				//SM.worldData->DirectionalSun.transformation.rotation.z = rotZ;
				//SM.worldData->DirectionalSun.transformation.rotation.w = rotW;
			}
			ReleaseMutex(SM.LightMutexHandle);
			SM.AddUpdateMessage("DirectionalLight", 0, true, true, false);
		}

		if(node.hasFn(MFn::kAmbientLight))
		{
			SM.LightMutexHandle = CreateMutex(nullptr, false, L"LightMutex");
			WaitForSingleObject(SM.LightMutexHandle, SM.milliseconds);

			memcpy(SM.worldData->AmbientLight.LightType, AL.asChar(), AL.numChars());

			SM.worldData->AmbientLight.color.r = light.color(&status).r;
			SM.worldData->AmbientLight.color.g = light.color(&status).g;
			SM.worldData->AmbientLight.color.b = light.color(&status).b;
			SM.worldData->AmbientLight.color.a = light.color(&status).a;
			SM.worldData->AmbientLight.Intensity = light.intensity(&status);

			///////////////////////	LIGHT TRANSFORM
			if(light.parent(0, &status).hasFn(MFn::kTransform))
			{
				MFnTransform transform = light.parent(0, &status);

				MObject transObj = transform.object();
				UpdateTransformation(SM.worldData->AmbientLight.transformation, transObj);

				//transform.getScale(scale);
				//transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

				//SM.worldData->AmbientLight.transformation.position.x = transform.getTranslation(g_space_world).x;
				//SM.worldData->AmbientLight.transformation.position.y = transform.getTranslation(g_space_world).y;
				//SM.worldData->AmbientLight.transformation.position.z = transform.getTranslation(g_space_world).z;

				//SM.worldData->AmbientLight.transformation.scale.x = scale[0];
				//SM.worldData->AmbientLight.transformation.scale.y = scale[1];
				//SM.worldData->AmbientLight.transformation.scale.z = scale[2];

				//SM.worldData->AmbientLight.transformation.rotation.x = rotX;
				//SM.worldData->AmbientLight.transformation.rotation.y = rotY;
				//SM.worldData->AmbientLight.transformation.rotation.z = rotZ;
				//SM.worldData->AmbientLight.transformation.rotation.w = rotW;
			}

			SM.AddUpdateMessage("AmbientLight", 0, true, true, false);
			ReleaseMutex(SM.LightMutexHandle);
		}
	}
}

void MayaCameraToList(MObject node, int cameraIndex)
{
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;

	///////////////////////	CAMERA
	if(node.hasFn(MFn::kCamera))
	{
		MFnCamera camera = node;

		memcpy(SM.cameraList[cameraIndex].transformation.name, camera.fullPathName().asChar(), camera.fullPathName().numChars());
		//SM.cameraList[cameraIndex].transformation.name = camera.fullPathName().asChar();
		SM.cameraList[cameraIndex].aspectRatio = camera.aspectRatio();

		SM.cameraList[cameraIndex].eyePoint.x = camera.eyePoint(g_space_world).x;
		SM.cameraList[cameraIndex].eyePoint.y = camera.eyePoint(g_space_world).y;
		SM.cameraList[cameraIndex].eyePoint.z = camera.eyePoint(g_space_world).z;

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

			MObject transObj = transform.object();
			UpdateTransformation(SM.cameraList[cameraIndex].transformation, transObj);
			//transform.getScale(scale);
			//transform.getRotationQuaternion(rotX, rotY, rotZ, rotW);

			//SM.cameraList[cameraIndex].transformation.position.x = transform.getTranslation(g_space_world).x;
			//SM.cameraList[cameraIndex].transformation.position.y = transform.getTranslation(g_space_world).y;
			//SM.cameraList[cameraIndex].transformation.position.z = transform.getTranslation(g_space_world).z;

			//SM.cameraList[cameraIndex].transformation.scale.x = scale[0];
			//SM.cameraList[cameraIndex].transformation.scale.y = scale[1];
			//SM.cameraList[cameraIndex].transformation.scale.z = scale[2];

			//SM.cameraList[cameraIndex].transformation.rotation.x = rotX;
			//SM.cameraList[cameraIndex].transformation.rotation.y = rotY;
			//SM.cameraList[cameraIndex].transformation.rotation.z = rotZ;
			//SM.cameraList[cameraIndex].transformation.rotation.w = rotW;
		}
		SM.UpdateSharedCamera(cameraIndex);
	}
}

void ExtractRGBTextures(MFnDependencyNode &material_node, MString &Red, MString &Green, MString &Blue, int &tileFactor)
{
	MStatus status = MS::kSuccess;;
	//Get the texture paths.

	MPlug color_plugR,color_plugG,color_plugB, tile_plug;
	MPlugArray connections;
	color_plugR = material_node.findPlug("Red", true, &status);
	color_plugG = material_node.findPlug("Green", true, &status);
	color_plugB = material_node.findPlug("Blue", true, &status);
	tile_plug = material_node.findPlug("TileFactor", true, &status);

	Red = color_plugR.asString();
	Green = color_plugG.asString();
	Blue = color_plugB.asString();
	tileFactor = tile_plug.asInt();


	//Print("Value in plug Red: ",color_plugR.asString());
	//Print("Value in plug Green: ",color_plugG.asString());
	//Print("Value in plug Blue: ",color_plugB.asString());
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

void ExtractColor(MFnDependencyNode &material_node, MString &out_color_path, MFnDependencyNode &out_textureNode)
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

			out_textureNode.setObject(connections[n].node(&status));

			//Print("DEEP NAME ", out_textureNode.name());

			MPlug ftn = texture_node.findPlug("ftn", &status);
			out_color_path = ftn.asString(MDGContext::fsNormal);

			if(out_color_path == "")
			{
				out_color_path = "PaintTexture";
			}

			break;
		}
	}

	if (!found_color)
	{
		out_color_path = "NONE";
	}
}

void ExtractGlow(MFnDependencyNode &material_node, MString &out_glow_path)
{
	MStatus status = MS::kSuccess;;
	
	MPlug normal_plug;
	MPlugArray connections;
	normal_plug = material_node.findPlug("glowIntensity", true, &status);
	//if(status == true)
	Print("GLOW");

	MPlugArray bv_connections;
	normal_plug.connectedTo(bv_connections, true, false, &status);

	bool found_glow = false;

	for(unsigned int j=0; j<bv_connections.length(); ++j)
	{
		if(bv_connections[j].node(&status).hasFn(MFn::kFileTexture))
		{
			//Print("FoundTexture!");
			MFnDependencyNode test1(bv_connections[j].node(&status));
			MPlug ftn = test1.findPlug("ftn", &status);
			out_glow_path = ftn.asString(MDGContext::fsNormal);
			Print(out_glow_path);
			found_glow = true;
		}
	}

	if (!found_glow)
	{
		out_glow_path = "NONE";
	}
}

void ExtractTranslucence(MFnDependencyNode &material_node, MString &out_translucence_path)
{
	MStatus status = MS::kSuccess;;

	MPlug normal_plug;
	MPlugArray connections;
	normal_plug = material_node.findPlug("translucence", true, &status);
	//if(status == true)
		//Print("Translucence");

	MPlugArray bv_connections;
	normal_plug.connectedTo(bv_connections, true, false, &status);

	bool found_glow = false;

	for(unsigned int j=0; j<bv_connections.length(); ++j)
	{
		if(bv_connections[j].node(&status).hasFn(MFn::kFileTexture))
		{
			//Print("FoundTexture!");
			MFnDependencyNode test1(bv_connections[j].node(&status));
			MPlug ftn = test1.findPlug("ftn", &status);
			out_translucence_path = ftn.asString(MDGContext::fsNormal);
			//Print(out_translucence_path);
			found_glow = true;
		}
	}

	if (!found_glow)
	{
		out_translucence_path = "NONE";
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

void ExtractSpecular(MFnDependencyNode &material_node, MString &out_specular_path)
{
	MStatus status = MS::kSuccess;;
	
	MPlug normal_plug;
	MPlugArray connections;
	normal_plug = material_node.findPlug("specularColor", true, &status);
	//if(status == true)
	//Print("This Is TRUEEE");

	MPlugArray bv_connections;
	normal_plug.connectedTo(bv_connections, true, false, &status);

	bool found_Specular = false;

	for(unsigned int j=0; j<bv_connections.length(); ++j)
	{
		if(bv_connections[j].node(&status).hasFn(MFn::kFileTexture))
		{
			//Print("FoundTexture!");
			MFnDependencyNode test1(bv_connections[j].node(&status));
			MPlug ftn = test1.findPlug("ftn", &status);
			out_specular_path = ftn.asString(MDGContext::fsNormal);
			found_Specular = true;
		}
	}

	if (!found_Specular)
	{
		out_specular_path = "NONE";
	}

}
void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth, MFnDependencyNode &material_node, MString &out_specular_path, MFnDependencyNode &out_textureNode, MString &out_glow_path, MString &out_translucence_path)
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
			ExtractColor(material_node, out_color_path, out_textureNode);
			//Print("IN MATERIAL DATA NAME = ", out_textureNode.name());
			ExtractBump(material_node, out_bump_path, out_bump_depth);
			ExtractSpecular(material_node, out_specular_path);
			ExtractGlow(material_node, out_glow_path);
			ExtractTranslucence(material_node, out_translucence_path);
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