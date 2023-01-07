#include "window/window.hpp"

void press_printHello(sf::Event event, void* obj) {
    Button* button = (Button*)obj;
    int width = button->getWidth();
    int height = button->getHeight();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*button->linkedWindow->win);
    if (mousePos.x >= button->posX && mousePos.x <= button->posX + width) {
        if (mousePos.y >= button->posY && mousePos.y <= button->posY + height) {
            printf("Pressed!\n");
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

void multiThreadedBaseWindow(void* args) {
    int* realArgs = (int*)args;
    baseWindow(realArgs[0], realArgs[1]);
}

void baseWindow(int width, int height) {

    sf::Font mainFont;
    sf::Text title;
    sf::Vector2f titlePos(16.0f, 16.0f);
    title.setPosition(titlePos);
    title.setString("TIPE");
    title.setFillColor(sf::Color::White);
    title.setCharacterSize(24);
    title.setStyle(sf::Text::Bold);

    if (!mainFont.loadFromFile(DEFAULT_FONT)) {
        printf("Error, unrecognized font!");
        return;
    }

    title.setFont(mainFont);


    sf::ContextSettings mainWinSettings;
    mainWinSettings.antialiasingLevel = 8;
    windowHierarchy mainWin(width, height, "TIPE");
    Button button(200, 200, 300, 100, sf::Color::Green, sf::Color::Red, DEFAULT_FONT, "Press me!", &mainWin);
    Slider slider(400, 400, 200, 25, 10.0f, 20.0f, GRAY_COLOR, sf::Color::Blue, &mainWin);
    
    button.setReaction(press_printHello);
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
        mainWin.drawObjects();
        mainWin.win->display();
	}

    mainWin.clearOnClose();

}