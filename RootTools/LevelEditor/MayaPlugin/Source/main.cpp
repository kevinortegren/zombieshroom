#include "stdafx.h"
#include "maya_includes.h"

#include "SharedMemory.h"

SharedMemory SM;

// Globala variabler.
MCallbackIdArray g_callback_ids;
MObject g_selectedObject;

MObject g_objectList[g_maxSceneObjects], g_mayaMeshList[g_maxMeshes], g_mayaCameraList[g_maxCameras], g_mayaLightList[g_maxLights];
int currNrSceneObjects=0, currNrMeshes=0, currNrLights=0, currNrCameras=0;

void ConnectionCB(MPlug& srcPlug, MPlug& destPlug, bool made, void *clientData);
void dirtyMeshNodeCB(MObject &node, MPlug &plug, void *clientData);
void dirtyTransformNodeCB(MObject &node, MPlug &plug, void *clientData);
void NodeAddedCB(MObject &node, void *clientData);

void checkForNewMeshes(bool made, MObject source, MObject destination);
void checkForNewCameras(MObject &node, void *clientData);
void loadScene();
void sortObjectList();
void printLists();
void checkForNewLights(MObject &node, void *clientData);
void GetMaterial(MObject node);
void MayaListToList(MObject node);
void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth);
MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle );

int meshIndex, cameraIndex, lightIndex;

// Lägger till ett callback-id i callback-arrayen.
void AddCallbackID(MStatus status, MCallbackId id)
{
	if (Status(__LINE__, status))
		g_callback_ids.append(id);
}

// Initiera pluginet.
EXPORT MStatus initializePlugin(MObject obj)
{	
	
	sortObjectList();
	//printLists();
	loadScene();
	SM.InitalizeSharedMemory();

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

	}

	int nrTotalVertices = 0;

	for(int i = 0; i < currNrMeshes; i++)
	{
		
		int maxCount = SM.meshList[i].nrOfVertices;
		//Print("MeshName ", SM.meshList[i].transformation.name.c_str());
		//Print("TexturePath ", SM.meshList[i].texturePath.c_str());
		//Print("NormalPath ", SM.meshList[i].normalPath.c_str());

		//for(int j = 0; j < maxCount; j++)
		//{
		//	Print("v ", SM.meshList[i].vertex[j].x, " ", SM.meshList[i].vertex[j].y," ", SM.meshList[i].vertex[j].z);
		//	Print("vn ", SM.meshList[i].normal[j].x, " ", SM.meshList[i].normal[j].y," ", SM.meshList[i].normal[j].z);
		//	Print("tt 0 0 0");
		//	Print("vt ", SM.meshList[i].UV[j][0], " ", SM.meshList[0].UV[j][1]);
		//	nrTotalVertices++;
		//}
	}

	//Print("Total vertices: ", nrTotalVertices);
	return status;
}

void sortObjectList()
{
	MItDependencyNodes it(MFn::kInvalid);
	MStatus status;

	currNrSceneObjects = 0;

	while(!it.isDone())
	{
		g_objectList[currNrSceneObjects] = it.item();
		currNrSceneObjects++;
		it.next();
	}

	for(int i= 0; i < currNrSceneObjects; i ++)
	{
		if(g_objectList[i].hasFn(MFn::kMesh))
		{
			MFnMesh tempMesh = g_objectList[i];
			g_mayaMeshList[currNrMeshes] = g_objectList[i];
			MayaListToList(g_mayaMeshList[currNrMeshes]);
			currNrMeshes ++;
		}
		else if(g_objectList[i].hasFn(MFn::kCamera))
		{
			MFnCamera tempCam = g_objectList[i];
			g_mayaCameraList[currNrCameras] = g_objectList[i];
			MayaListToList(g_mayaCameraList[currNrCameras]);
			currNrCameras++;
		}
		else if(g_objectList[i].hasFn(MFn::kLight))
		{
			MFnLight tempLight = g_objectList[i];
			g_mayaLightList[currNrLights] = g_objectList[i];
			MayaListToList(g_mayaLightList[currNrLights]);
			currNrLights++;
		}
	}
}

void printLists()
{
	Print("Meshes");
	for(int i = 0; i < currNrMeshes; i++)
	{
		MFnMesh tempMesh = g_mayaMeshList[i];
		//Print(tempMesh.fullPathName());
		
	}

	Print("Cameras");
	for(int i = 0; i < currNrCameras; i++)
	{
		MFnCamera tempCamera = g_mayaCameraList[i];
		//Print(tempCamera.fullPathName());
		
	}

	Print("Lights");
	for(int i = 0; i < currNrLights; i++)
	{
		MFnLight tempLight = g_mayaLightList[i];
		//Print(tempLight.fullPathName());
		
	}


}

void loadScene()
{
	MStatus status;

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
	}
	//Light transformation CB
	for(int i = 0; i < currNrLights; i++)
	{
		MFnLight light = g_mayaLightList[i];

		if(light.parent(0,&status).hasFn(MFn::kTransform))
		{
			MObject transform = light.parent(0, &status);
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}
	}
	//Camera transformation CB
	for(int i = 0; i < currNrCameras; i++)
	{
		MFnCamera camera = g_mayaCameraList[i];

		if(camera.parent(0,&status).hasFn(MFn::kTransform))
		{
			MObject transform = camera.parent(0, &status);
			MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);
		}
	}

}

////////////////////////////	LOOK IF A MESH NODE IS DIRTY  //////////////////////////////////////////
void dirtyMeshNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	MStatus status;
	MFnMesh	mesh = node;
	MPointArray points_;
	MFloatArray U, V;
	MString UVsetName;

	UVsetName = mesh.currentUVSetName(&status, -1);

	MSpace::Space world_space = MSpace::kObject;
	float myPoints[g_maxVerticesPerMesh][4];

	
	//Print("Full pathname: ", mesh.fullPathName());

	mesh.getPoints(points_, world_space);

	points_.get(myPoints);

	mesh.getUVs(U,V,&UVsetName);

	//for(int i = 0; i < points_.length(); i++)
	//{
	//	Print("Vertex ", i, " X: ", myPoints[i][0]," Y: ", myPoints[i][1], " Z: ", myPoints[i][2]);
	//	Print("U: ", U[i], " V: ", V[i]);
	//}

	MayaListToList(node);
}
////////////////////////////	LOOK IF A TRANSFORMATION NODE IS DIRTY  //////////////////////////////////////////
void dirtyTransformNodeCB(MObject &node, MPlug &plug, void *clientData)
{
	MStatus status = MS::kSuccess;
	MSpace::Space _spaceWorld = MSpace::kWorld;
	MSpace::Space _space = MSpace::kObject;
	MSpace::Space _spaceTrans = MSpace::kTransform;
	MFnTransform trans = node;

	double scale[3];
	double rotX, rotY, rotZ, rotW;
	
	MFnMesh mesh = trans.child(0, &status);

	trans.getScale(scale);
	MVector translation = trans.getTranslation(_spaceTrans, &status);

	trans.getRotationQuaternion(rotX, rotY, rotZ, rotW, _space);

	MayaListToList(trans.child(0, &status));

	//Print("Trans name: ", trans.fullPathName());
	//Print("Scale: X: ", scale[0], " Y: ", scale[1], " Z: ", scale[2]);
	//Print("Translation: X: ", translation.x, " Y: ", translation.y, " Z: ", translation.z);
	//Print("Rotation X: ", rotX, " Y: ", rotY, " Z: ", rotZ, " W: ", rotW);
}

////////////////////////////	SORT OUT WHAT TYPE THE NEW NODE ARE //////////////////////////////////////////
void NodeAddedCB(MObject &node, void *clientData)
{
	checkForNewCameras(node, clientData);
	checkForNewLights(node, clientData);
	//printLists();
}

// MESHES
// CONNECTION CB FOR MESHES
void ConnectionCB(MPlug& srcPlug, MPlug& destPlug, bool made, void *clientData)
{
	MStatus status1 = MS::kSuccess;
	MStatus status2 = MS::kSuccess;

	MObject source = srcPlug.node(&status1);
	MObject destination = destPlug.node(&status2);
	
	if (Status(__LINE__, status1) && Status(__LINE__, status2))
	{
		checkForNewMeshes(made, source, destination);
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

		//Print("Added ", mesh.fullPathName());
		id = MNodeMessage::addNodeDirtyPlugCallback(g_mayaMeshList[currNrMeshes], dirtyMeshNodeCB, nullptr, &status);
		AddCallbackID(status, id);


		if(mesh.parent(0, &status).hasFn(MFn::kTransform))
		{
			MObject transform = mesh.parent(0, &status);

			id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
			AddCallbackID(status, id);

			MayaListToList(destination);
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
		currNrLights++;
	}
	
	if(light.parent(0,&status).hasFn(MFn::kTransform))
	{
		MObject transform = light.parent(0, &status);
		MCallbackId id = MNodeMessage::addNodeDirtyPlugCallback(transform, dirtyTransformNodeCB, nullptr, &status);
		AddCallbackID(status, id);
	}
}

bool nodeExists(MObject node)
{
	bool answer = false;
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = node;
		for(int i = 0; i < currNrMeshes; i++)
		{
			MFnMesh temp = g_mayaMeshList[i];

			if(temp.fullPathName() == mesh.fullPathName())
			{
				meshIndex = i;
				answer = true;
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
				cameraIndex = i;
				answer = true;
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
				lightIndex = i;
				answer = true;
				break;
			}
		}	
	}

	return answer;
}

////////////////////////////	ADD THE NODES TO THE SHARED LIST	 //////////////////////////////////////////
void MayaListToList(MObject node)
{
	MStatus status;
	double scale[3];
	double rotX, rotY, rotZ, rotW;
	MSpace::Space space_transform = MSpace::kTransform;
	MSpace::Space space_local = MSpace::kObject;

	MIntArray triangleCounts, triangleVertices;

	bool alreadyExists = nodeExists(node);

	///////////////////////	MESH
	if (node.hasFn(MFn::kMesh))
	{
		MFnMesh mesh = node;
		bool updateTrans = false;
		bool updateMesh = false;

		int numVertices = mesh.numVertices();
		//SM.meshList[meshIndex].nrOfVertices = numVertices;
		int numPolygons = mesh.numPolygons();
		MPointArray points_;
		float floatPoints[g_maxVerticesPerMesh][4];
		float U, V;

		//Get, set texturepaths
		MString texturepath = "";
		MString normalpath = "";
		float bumpdepth;

		ExtractMaterialData(mesh, texturepath, normalpath, bumpdepth);

		SM.meshList[meshIndex].texturePath = texturepath.asChar();
		SM.meshList[meshIndex].normalPath = normalpath.asChar();

		//Get and set mesh name
		memcpy(SM.meshList[meshIndex].transformation.name, mesh.fullPathName().asChar(), mesh.fullPathName().numChars());
		//SM.meshList[meshIndex].transformation.name = mesh.fullPathName().asChar();

		//UVsetName = mesh.currentUVSetName(&status, -1);		

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

		SM.meshList[meshIndex].nrOfVertices = 0;

		int uvID;


		for(int i = 0; i < numPolygons; i++)
		{
			MIntArray polygonVertices, localIndex;
			int polygonVertexCount = mesh.polygonVertexCount(i);

			int nrOfTriangles;

			if(polygonVertexCount >= 4)
				nrOfTriangles = 2;
			else
				nrOfTriangles = 1;
			
			mesh.getPolygonVertices(i, polygonVertices);

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

					mesh.getVertexNormal(localIndex[j],false, normal, space_local);
					SM.meshList[meshIndex].normal[count].x = normal.x;
					SM.meshList[meshIndex].normal[count].y = normal.y;
					SM.meshList[meshIndex].normal[count].z = normal.z;

					mesh.getPolygonUVid(i, localIndex[j], uvID, 0);
					mesh.getUV(uvID, U, V, 0);

					SM.meshList[meshIndex].UV[count].x = U;
					SM.meshList[meshIndex].UV[count].y = V;

					count++;
					SM.meshList[meshIndex].nrOfVertices ++;
					updateMesh = true;
				}
			}
		}

		///////////////////////	MESH TRANSFORM
		if(mesh.parent(0,&status).hasFn(MFn::kTransform))
		{
			MFnTransform transform = mesh.parent(0, &status);

			transform.getScale(scale);
			transform.getRotationQuaternion(rotX, rotY, rotZ, rotW, space_local);

			SM.meshList[meshIndex].transformation.position.x = transform.getTranslation(space_transform).x;
			SM.meshList[meshIndex].transformation.position.y = transform.getTranslation(space_transform).y;
			SM.meshList[meshIndex].transformation.position.z = transform.getTranslation(space_transform).z;

			SM.meshList[meshIndex].transformation.scale.x = scale[0];
			SM.meshList[meshIndex].transformation.scale.y = scale[1];
			SM.meshList[meshIndex].transformation.scale.z = scale[2];

			SM.meshList[meshIndex].transformation.rotation.x = rotX;
			SM.meshList[meshIndex].transformation.rotation.y = rotY;
			SM.meshList[meshIndex].transformation.rotation.z = rotZ;
			SM.meshList[meshIndex].transformation.rotation.w = rotW;

			updateTrans = true;
		}

		if(updateMesh || updateTrans)
			{
			if(!alreadyExists)
			meshIndex++;
			SM.UpdateSharedMesh(meshIndex, updateTrans, updateMesh, currNrMeshes);
			}

		updateMesh, updateTrans = false;

		// ID : 2 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
		// ID : 3 // 
	}
	///////////////////////	LIGHT
	else if(node.hasFn(MFn::kLight))
	{
		bool updateTrans = false;
		bool updateLight = false;

		MFnLight light = node;
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


			if(!alreadyExists)
			lightIndex++;
			SM.UpdateSharedLight(lightIndex, currNrLights);
		}
	}
	///////////////////////	CAMERA
	else if(node.hasFn(MFn::kCamera))
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

			//MString tempString = SM.cameraList[cameraIndex].transformation.name.c_str();			
			//Print("Camera ", tempString, " has been addedd to list at index ", cameraIndex);

			if(!alreadyExists)
				cameraIndex++;
		}
	}
}

void GetMaterialNode(MObject &shading_engine, MFnDependencyNode &out_material_node)
{
	MStatus status = MS::kSuccess;;

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

void ExtractColor(MFnDependencyNode &material_node, MString &out_color_path)
{
	MStatus status = MS::kSuccess;;

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

			//ftn == fileTextureName
			MPlug ftn = texture_node.findPlug("ftn", &status);
			out_color_path = ftn.asString(MDGContext::fsNormal);

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

void ExtractMaterialData(MFnMesh &mesh, MString &out_color_path, MString &out_bump_path, float &out_bump_depth)
{
	MStatus status = MS::kSuccess;;

	MObjectArray shaders;
	MIntArray face_indices;

	int parentCount = mesh.parentCount(&status);

	for(int i = 0; i < parentCount; i++)
	{
		mesh.getConnectedShaders(i, shaders, face_indices);
		
		for(int j = 0; j < shaders.length(); j++)
		{
			MFnDependencyNode material_node;

			GetMaterialNode(shaders[j], material_node);
			ExtractColor(material_node, out_color_path);
			ExtractBump(material_node, out_bump_path, out_bump_depth);
		}
	}
}

MIntArray GetLocalIndex( MIntArray & getVertices, MIntArray & getTriangle )
{
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