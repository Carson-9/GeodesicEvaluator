#include "window/window.hpp"
#include "terrain/terrain.hpp"
#include "window/terrainWindow.hpp"

int main()
{

    const int WIDTH = 1000;     // 1000 //1920
    const int HEIGHT = 1000;    // 1000 //924
    const int octaves = 10;
    const float bias = 1.6f;
    const float sigmoidBlend = 100.f;

    threadpool* threads = threadpool_init(16);

    Terrain* mainTerrain = new Terrain(WIDTH, HEIGHT, octaves, bias, sigmoidBlend);
    controlWindowArgs controlWinArg = { 800, 800, mainTerrain };
    terrainWindowArgs terrainWinArgs = { WIDTH, HEIGHT, mainTerrain};

    launch_newThread(threads, multiThreadedBaseWindow, (void*)&controlWinArg);
    launch_newThread(threads, multiThreadLaunchTerrainWindow, (void*)&terrainWinArgs);
  
    threadpool_join(threads);
    return 0;
}