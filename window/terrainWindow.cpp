#include "window/terrainWindow.hpp"


void multiThreadDrawTerrain(void* args) {
    drawTerrainArgs* realArgs = (drawTerrainArgs*)args;
    drawTerrain(realArgs->win, realArgs->terrain, sf::Text());
}

void drawTerrain(windowHierarchy* win, Terrain* terrain, sf::Text infoText) {
    sf::Image img;
    sf::Texture tex;
    sf::Sprite background;
    terrain->makeSprite(background);
    img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
    tex.loadFromImage(img);
    background.setTexture(tex);
    win->win->clear(sf::Color::Black);
    win->win->draw(background);
    win->win->draw(infoText);
    win->win->display();
    return;
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

    //mainTerrain->generateFromFile("Output/tGreece.txt");
    mainTerrain->generateColorMap();


    int heightToDraw[] = {25, 50, 75, 100, 125, 150, 175, 200};
    mainTerrain->setIndicatorList(heightToDraw, 8);


    sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", mainTerrain->getOctaves(), mainTerrain->getBias(), (int)mainTerrain->getBlend());
    infoText.setString(infoString);

    windowHierarchy mainWin(width, height, "TIPE");
    drawTerrain(&mainWin, mainTerrain, infoText);

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

                case sf::Keyboard::R:
                    mainTerrain->generateColorMap();
                    drawTerrain(&mainWin, mainTerrain, infoText);
                    break;

                case sf::Keyboard::Left:
                    mainTerrain->setOctaves(mainTerrain->getOctaves() - 1);
                    if (mainTerrain->getOctaves() < 1) mainTerrain->setOctaves(1);
                    break;

                case sf::Keyboard::Right:
                    mainTerrain->setOctaves(mainTerrain->getOctaves() + 1);
                    if (mainTerrain->getOctaves() > 11) mainTerrain->setOctaves(11);
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
                    mainTerrain->setBlend(mainTerrain->getBlend() + mainTerrain->getBlendResKeyboard());
                    if (mainTerrain->getBlend() > 10.0f) mainTerrain->setBlendResKeyboard(5.0f);
                    else mainTerrain->setBlendResKeyboard(1.0f);
                    break;

                case sf::Keyboard::V:
                    mainTerrain->setBlend(mainTerrain->getBlend() - mainTerrain->getBlendResKeyboard());
                    if (mainTerrain->getBlend() < 0.0f) mainTerrain->setBlend(0.0f);
                    if (mainTerrain->getBlend() <= 10.0f) mainTerrain->setBlendResKeyboard(1.0f);
                    else mainTerrain->setBlendResKeyboard(5.0f);
                    break;

                case sf::Keyboard::S:
                    writeTableToFile(mainTerrain->getSizeY(), mainTerrain->getSizeX(), mainTerrain->getHeightMap(), "Output/");
                    break;

                case sf::Keyboard::T:
                    mainTerrain->toggleIndicatorDraw();
                    mainTerrain->generateColorMap();
                    drawTerrain(&mainWin, mainTerrain, infoText);
                    break;

                default:
                    break;

                }

                sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", mainTerrain->getOctaves(), mainTerrain->getBias(), (int)mainTerrain->getBlend());
                infoText.setString(infoString);

            }

        }
        drawTerrain(&mainWin, mainTerrain, infoText);
    }

}