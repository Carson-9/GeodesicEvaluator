#include "../window/window.hpp"

void press_togglePressed(sf::Event event, void* obj) {
    Button* button = (Button*)obj;
    int width = button->getWidth();
    int height = button->getHeight();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*button->linkedWindow->win);
    if (mousePos.x >= button->posX && mousePos.x <= button->posX + width) {
        if (mousePos.y >= button->posY && mousePos.y <= button->posY + height) {
            button->toggleIsPressed();
        }
    }
}

void slider_move(sf::Event event, void* obj) {

    Slider* slider = (Slider*)obj;
    sf::Vector2i mousePos = sf::Mouse::getPosition(*slider->linkedWindow->win);
    float radius = slider->getSliderRadius();
    sf::Vector2f sliderPos = slider->getSliderAbsolutePos() + sf::Vector2f(radius, radius);

    if (!slider->isBeingMoved() && !((mousePos.x - sliderPos.x) * (mousePos.x - sliderPos.x)
        + (mousePos.y - sliderPos.y) * (mousePos.y - sliderPos.y) <= (radius * radius))) return;

    if (event.type == sf::Event::MouseButtonPressed && !slider->isBeingMoved()) {
        slider->toggleBeingMoved();
        return;
    }

    if (event.type == sf::Event::MouseMoved && slider->isBeingMoved()) {
        int l = slider->getLength();
        if (mousePos.x < slider->posX) slider->setSliderPos(0.0f);
        else if (mousePos.x > slider->posX + l) slider->setSliderPos(1.0f);
        else slider->setSliderPos(((float)mousePos.x - (float)slider->posX) / (float)l);
        return;
    }

    if (event.type == sf::Event::MouseButtonReleased && slider->isBeingMoved()) {
        slider->toggleBeingMoved();
        return;
    }

    return;
}

void generateFieldLines(Terrain* terrain, int FieldLineNumber) {
    float step = 256.0f / FieldLineNumber;
    int* fieldLines = new int[FieldLineNumber];
    for (int i = 0; i < FieldLineNumber; i++) fieldLines[i] = (int)(i * step);
    terrain->setIndicatorList(fieldLines, FieldLineNumber);
    delete[] fieldLines;
    terrain->generateColorMap();
}



void multiThreadedBaseWindow(void* args) {
    controlWindowArgs* realArgs = (controlWindowArgs*)args;
    baseWindow(realArgs->WIDTH, realArgs->HEIGHT, realArgs->terrain);
}

void baseWindow(int width, int height, Terrain* terrain) {

    int FieldLineNumber = 1;

    sf::Font mainFont;
    sf::Text title;
    sf::Text sliderText;
    sf::Vector2f titlePos(16.0f, 16.0f);
    sf::Vector2f sliderTextPos(285.0f, 625.0f);
    title.setPosition(titlePos);
    title.setString("TIPE");
    title.setFillColor(sf::Color::White);
    title.setCharacterSize(24);
    title.setStyle(sf::Text::Bold);

    sliderText.setPosition(sliderTextPos);
    sliderText.setString("Lignes de Niveau");
    sliderText.setFillColor(sf::Color::White);
    sliderText.setCharacterSize(24);
    sliderText.setStyle(sf::Text::Bold);

    if (!mainFont.loadFromFile(DEFAULT_FONT)) {
        printf("Error, unrecognized font!");
        return;
    }

    title.setFont(mainFont);
    sliderText.setFont(mainFont);


    sf::ContextSettings mainWinSettings;
    mainWinSettings.antialiasingLevel = 8;
    windowHierarchy mainWin(width, height, "TIPE", sf::Vector2i(80, 80));

    Button drawPathButton(550, 175, 250, 75, sf::Color::White, sf::Color::Blue, DEFAULT_FONT, "Straight Path", &mainWin);
    Button naivePathButton(550, 300, 250, 75, sf::Color::White, sf::Color::Blue, DEFAULT_FONT, "Bezier Sol.", &mainWin);
    Button evolvedPathButton(550, 425, 250, 75, sf::Color::White, sf::Color::Blue, DEFAULT_FONT, "Educated Sol.", &mainWin);
    Button saveOBJ(550, 50, 250, 75, sf::Color::White, sf::Color::Blue, DEFAULT_FONT, "Save to OBJ", &mainWin);
    Slider slider(300, 700, 200, 25, 1.0f, 21.0f, GRAY_COLOR, sf::Color::Blue, &mainWin);


    // press_togglePressed is a function that allows buttons to "send messages" by
    // triggering a boolean that is captured in the main loop.
    // One could do other methods for the buttons

    drawPathButton.setReaction(press_togglePressed);
    naivePathButton.setReaction(press_togglePressed);
    evolvedPathButton.setReaction(press_togglePressed);
    saveOBJ.setReaction(press_togglePressed);
    slider.setReaction(slider_move);

    mainWin.win->setKeyRepeatEnabled(true);

    while (mainWin.win->isOpen()) {
        sf::Event event;
        while (mainWin.win->pollEvent(event)) {

            if (event.type == sf::Event::Closed) mainWin.win->close();
            else mainWin.executeEvent(event);
        }

        mainWin.win->clear(sf::Color::Black);
        mainWin.win->draw(title);
        mainWin.win->draw(sliderText);
        mainWin.drawObjects();
        mainWin.win->display();

        int sliderVal = slider.getValue();
        if ((int)sliderVal != FieldLineNumber) {
            FieldLineNumber = (int)sliderVal;
            generateFieldLines(terrain, FieldLineNumber);
        }

        if (saveOBJ.isBeingPressed()) {
            saveOBJ.toggleIsPressed();
            terrain->generateOBJFile("Output/", 2.0f);
        }

        if (drawPathButton.isBeingPressed()) {
            drawPathButton.toggleIsPressed();
            terrain->generatePath();
        }

        if (naivePathButton.isBeingPressed()) {
            naivePathButton.toggleIsPressed();
            terrain->bezierPath();
        }

        if (evolvedPathButton.isBeingPressed()) {
            evolvedPathButton.toggleIsPressed();
            terrain->best_path();
        }
    }

    mainWin.clearOnClose();

}