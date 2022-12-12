#include "window/window.hpp"
#include "utilitaries/algebra.hpp"
#include "terrain/terrain.hpp"
#include "window/terrainWindow.hpp"

int main()
{

    const int gridSize = 1024;
    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int octaves = 8;
    const float bias = 1.8f;
    const float sigmoidBlend = 100.f;

    threadpool* threads = threadpool_init(16);

    int baseDimensions[2] = { 800, 800 };
    //int terrainDimensions[2] = { 1920, 1080 };

    terrainWindowArgs terrainWinArgs = { 1920, 1080, "TIPE" };


    //sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TIPE");
    sf::Image img;
    sf::Texture tex;
    sf::Sprite background;
    sf::Text infoText;
    sf::Font mainFont;

    char infoString[128];

    launch_newThread(threads, multiThreadedBaseWindow, (void*)baseDimensions);
    launch_newThread(threads, multiThreadLaunchTerrainWindow, (void*)&terrainWinArgs);
    //baseWindow(800, 800);
    
    if (!mainFont.loadFromFile("Resources/ALBold.ttf")) {
        printf("Error, unrecognized font!");
        return EXIT_FAILURE;
    }
   
    infoText.setFont(mainFont);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setStyle(sf::Text::Bold);
    sf::Vector2f infoTextPos(4.0f, 4.0f);
    infoText.setPosition(infoTextPos);

    sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", octaves, bias, (int)sigmoidBlend);
    infoText.setString(infoString);

    //Terrain* terrain = new Terrain(WIDTH, HEIGHT, octaves, bias, sigmoidBlend);
    //terrain->generateFromFile("Output/tGreece.txt");
    //terrain->makeSprite(background);
    //img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
    //tex.loadFromImage(img);
    //background.setTexture(tex);

    /*while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //switch (event.type) {

            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {

                case sf::Keyboard::Space:
                    terrain->generateTerrain();
                    terrain->makeSprite(background);
                    img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
                    tex.loadFromImage(img);
                    background.setTexture(tex);
                    break;


                case sf::Keyboard::Left:
                    terrain->setOctaves(terrain->getOctaves() - 1);
                    if (terrain->getOctaves() < 1) terrain->setOctaves(1);
                    break;

                case sf::Keyboard::Right:
                    terrain->setOctaves(terrain->getOctaves() + 1);
                    if (terrain->getOctaves() > 16) terrain->setOctaves(16);
                    break;

                case sf::Keyboard::Up:
                    terrain->setBias(terrain->getBias() + 0.1f);
                    if (terrain->getBias() > 3.0f) terrain->setBias(3.0f);
                    break;

                case sf::Keyboard::Down:
                    terrain->setBias(terrain->getBias() - 0.1f);
                    if (terrain->getBias() < 0.2f) terrain->setBias(0.2f);
                    break;

                case sf::Keyboard::B:
                    terrain->setBlend(terrain->getBlend() + 5.0f);
                    break;

                case sf::Keyboard::V:
                    terrain->setBlend(terrain->getBlend() - 5.0f);
                    if (terrain->getBlend() < 0.0f) terrain->setBlend(0.0f);
                    break;

                case sf::Keyboard::S:
                    writeTableToFile(terrain->getSizeY(), terrain->getSizeX(), terrain->getHeightMap(), "Output/");
                    break;


                default:
                    break;

                }

                sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", octaves, bias, (int)sigmoidBlend);
                infoText.setString(infoString);

            }

        }
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(infoText);
        window.display();
    }*/
    threadpool_join(threads);
    return 0;
}