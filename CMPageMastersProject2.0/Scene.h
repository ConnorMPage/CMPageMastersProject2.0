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
	Scene();

	~Scene();

	void RenderScene();
	void Update(float frameTime);
private:

	TerrainGen* gTerrain;
	EngineShaders* gEngineShader;
	SceneCamera* gCamera;
	HeightmapGenerator* gHGen;
	Light* gLight;
	bool mNormalised;
	bool InitialiseTerrainGenerator();
	bool CreateHeightmap();
	bool OpenFile();
	void Reset();
	bool mDisplayTerrain = false;
	bool mTerrainSet = false;
	bool mLockFPS = true;
	 const char* bitmapAdd = R"(../CMPageMastersProject2.0/Data/testingHMap.bmp)";
	 int numParticles = 0;
	 
	 std::string mHMapAddress = "";
	 std::string mHMapName;
	 int mHeightMapWidth = 256;
	 int mHeightMapHeight = 256;
	 int mHeightMapScale = 1;
	 const int MAXMAPSIZE = 1000;
	 const int MINMAPSIZE = 150;

	 const int MAXMAPSCALE = 100;
	 const int MINMAPSCALE = 1;
};

