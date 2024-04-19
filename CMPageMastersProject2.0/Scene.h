#pragma once
#include <string>
class TerrainGen;
class EngineShaders;
class SceneCamera;
class HeightmapGenerator;
class Light;
class Scene
{

public:
	Scene();//constructor 

	~Scene();//destructor 

	void RenderScene();//render function
	void Update(float frameTime);//update function used for camera movement 
private:

	//object declarations 
	TerrainGen* gTerrain;
	EngineShaders* gEngineShader;
	SceneCamera* gCamera;
	HeightmapGenerator* gHGen;
	Light* gLight;
	
	

	// functions to run of button presses 
	bool InitialiseTerrainGenerator();
	bool CreateHeightmap();
	bool OpenFile();
	void Reset();
	
	//never changes dont know why i put it there 
	bool mLockFPS = true;
	 
	 
	//display terrain bools  
	bool mDisplayTerrain = false;
	bool mTerrainSet = false;
	
	 
	 //UI variables
	 int mHeightMapWidth = 256;
	 int mHeightMapHeight = 256;
	 int mHeightMapScale = 1;
	 int mOctavesAmount = 1;
	 int mRuntimeScale = 1;
	 std::string mHMapAddress = "";
	 std::string mHMapName;
	 bool mNormalised = true;
	 

	 //constant declarations 
	 const int MAXMAPSIZE = 1000;
	 const int MINMAPSIZE = 150;

	 const int MAXMAPSCALE = 30;
	 const int MINMAPSCALE = 1;

	 const int MAXOCTAVES = 16;
	 const int MINOCTAVES = 1;

	 const char* bitmapAdd = R"(../CMPageMastersProject2.0/Data/testingHMap.bmp)";
};

