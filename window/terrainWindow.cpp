#include "window/terrainWindow.hpp"


void multiThreadDrawTerrain(void* args) {
    drawTerrainArgs* realArgs = (drawTerrainArgs*)args;
    drawTerrain(realArgs->win, realArgs->terrain);
}

void drawTerrain(windowHierarchy* win, Terrain* terrain) {
    sf::Image img;
    sf::Texture tex;
    sf::Sprite background;
    terrain->makeSprite(background);
    img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
    tex.loadFromImage(img);
    background.setTexture(tex);
    win->win->draw(background);
}



void multiThreadLaunchTerrainWindow(void* args) {
	terrainWindowArgs* trueArgs = (terrainWindowArgs*)args;
	launchTerrainWindow(trueArgs->WIDTH, trueArgs->HEIGHT, trueArgs->filename);
}

void launchTerrainWindow(int width, int height, const char* filename) {

    sf::Text infoText;
    sf::Font mainFont;

    char infoString[128];

    Terrain* mainTerrain = new Terrain(width, height, 8, 1.4f, 100.0f);

    if (!mainFont.loadFromFile(DEFAULT_FONT)) {
        printf("Error, unrecognized font!");
        return;
    }

    infoText.setFont(mainFont);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setStyle(sf::Text::Bold);
    sf::Vector2f infoTextPos(4.0f, 4.0f);
    infoText.setPosition(infoTextPos);

    sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", mainTerrain->getOctaves(), mainTerrain->getBias(), (int)mainTerrain->getBlend());
    infoText.setString(infoString);

    windowHierarchy mainWin(width, height, "TIPE");
    drawTerrain(&mainWin, mainTerrain);

    while (mainWin.win->isOpen()) {
        sf::Event event;
        while (mainWin.win->pollEvent(event))
        {
            //switch (event.type) {

            if (event.type == sf::Event::Closed) mainWin.win->close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {

                case sf::Keyboard::Space:
                    mainTerrain->generateTerrain();
                    break;


                case sf::Keyboard::Left:
                    mainTerrain->setOctaves(mainTerrain->getOctaves() - 1);
                    if (mainTerrain->getOctaves() < 1) mainTerrain->setOctaves(1);
                    break;

                case sf::Keyboard::Right:
                    mainTerrain->setOctaves(mainTerrain->getOctaves() + 1);
                    if (mainTerrain->getOctaves() > 16) mainTerrain->setOctaves(16);
                    break;

                case sf::Keyboard::Up:
                    mainTerrain->setBias(mainTerrain->getBias() + 0.1f);
                    if (mainTerrain->getBias() > 3.0f) mainTerrain->setBias(3.0f);
                    break;

                case sf::Keyboard::Down:
                    mainTerrain->setBias(mainTerrain->getBias() - 0.1f);
                    if (mainTerrain->getBias() < 0.2f) mainTerrain->setBias(0.2f);
                    break;

                case sf::Keyboard::B:
                    mainTerrain->setBlend(mainTerrain->getBlend() + 5.0f);
                    break;

                case sf::Keyboard::V:
                    mainTerrain->setBlend(mainTerrain->getBlend() - 5.0f);
                    if (mainTerrain->getBlend() < 0.0f) mainTerrain->setBlend(0.0f);
                    break;

                case sf::Keyboard::S:
                    writeTableToFile(mainTerrain->getSizeY(), mainTerrain->getSizeX(), mainTerrain->getHeightMap(), "Output/");
                    break;


                default:
                    break;

                }

                sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", mainTerrain->getOctaves(), mainTerrain->getBias(), (int)mainTerrain->getBlend());
                infoText.setString(infoString);

            }

        }
        mainWin.win->clear(sf::Color::Black);
        mainWin.win->draw(infoText);
        drawTerrain(&mainWin, mainTerrain);
        mainWin.win->display();
    }

}