#include "window/terrainWindow.hpp"


void pointMove(sf::Event event, void* obj) {
    WindowPoint* pt = (WindowPoint*)obj;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*pt->linkedWindow->win);
    f32 radius = pt->getRadius();

    f32 pointX = pt->getX() + radius;
    f32 pointY = pt->getY() + radius;

    if (!pt->isBeingMoved() && !((mousePos.x - pointX) * (mousePos.x - pointX)
        + (mousePos.y - pointY) * (mousePos.y - pointY) <= (radius * radius))) 
        return;
    

    if (event.type == sf::Event::MouseButtonPressed && !pt->isBeingMoved()) {
        pt->toggleBeingMoved();
        return;
    }

    if (event.type == sf::Event::MouseButtonReleased && pt->isBeingMoved()) {
        pt->toggleBeingMoved();
        return;
    }

    if (event.type == sf::Event::MouseMoved && pt->isBeingMoved()) {
        pt->setPosition(
            CLAMP((f32)mousePos.x - radius, -radius, pt->linkedWindow->Width - radius),
            CLAMP((f32)mousePos.y - radius, -radius, pt->linkedWindow->Height - radius));
        return;
    }

}






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
    return;
}



void multiThreadLaunchTerrainWindow(void* args) {
	terrainWindowArgs* trueArgs = (terrainWindowArgs*)args;
	launchTerrainWindow(trueArgs->WIDTH, trueArgs->HEIGHT, trueArgs->terrain);
}

void launchTerrainWindow(int width, int height, Terrain* mainTerrain) {

    sf::Text infoText;
    sf::Font mainFont;

    char infoString[128];

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

    mainTerrain->generateFromFile("Output/tGreece.txt");
    mainTerrain->generateColorMap();

    sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", mainTerrain->getOctaves(), mainTerrain->getBias(), (int)mainTerrain->getBlend());
    infoText.setString(infoString);

    windowHierarchy mainWin(width, height, "TIPE - Terrain", sf::Vector2i(900, 0));
    drawTerrain(&mainWin, mainTerrain, infoText);
    mainWin.win->setKeyRepeatEnabled(true);


    WindowPoint pointA(0, 0, 15.0f, sf::Color::Red, &mainWin);
    WindowPoint pointB((f32)width / 2 + 30.0f, (f32)height / 2, 15.0f, sf::Color::Red, &mainWin);

    mainTerrain->setPoints(pointA.getReference(), pointB.getReference());

    pointA.setReaction(pointMove);
    pointB.setReaction(pointMove);


    while (mainWin.win->isOpen()) {
        sf::Event event;
        while (mainWin.win->pollEvent(event))
        {
            //switch (event.type) {

            mainWin.executeEvent(event);

            if (event.type == sf::Event::Closed) mainWin.win->close();
            else if (event.type == sf::Event::KeyPressed) {
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
        mainWin.drawObjects();
        mainWin.win->display();
    }

}