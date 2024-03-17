#pragma once

class TerrainGen;
class EngineShaders;
class SceneCamera;
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

	bool mLockFPS = true;
	 const char* bitmapAdd = R"(../CMPageMastersProject2.0/Data/testingHMap.bmp)";
};

