Global = {
	UserID = -1;
	IsClient = false;
}

ReservedUserID = {
	NONE = 0xFFFD;
	UNCONNECTED = 0xFFFE;
	ALL = 0xFFFF;
}

ReservedActionID = {
	NONE = 0xFFFFFFFD;
	CONNECT = 0xFFFFFFFE;
	ALL = 0xFFFFFFFF;
}

ReservedSequenceID = {
	NONE = 0xFFFD;
	CLIENT_ENTITY = 0xFFFE;
	ALL = 0xFFFF;
}

EntityState = {
	ASCENDING = 0;
	DESCENDING = 1;
	LANDING = 2;
	GROUNDED = 3;
	RAGDOLL = 4;
}

RenderPass = {
	RENDERPASS_SKYBOX = 0;
	RENDERPASS_TERRAIN = 1;
	RENDERPASS_DYNAMIC = 2;
}

LogLevel = {
	FATAL_ERROR = 0;
	NON_FATAL_ERROR = 1;
	WARNING = 2;
	SUCCESS = 3;
	DEBUG_PRINT = 4;
	INIT_PRINT = 5;
	MASS_DATA_PRINT = 6;
}

PhysicsType = {
	TYPE_STATIC = 0;
	TYPE_ABILITY = 1;
	TYPE_DYNAMIC = 2;
	TYPE_PLAYER = 3;
};