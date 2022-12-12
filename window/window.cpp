#include "window/window.hpp"



void press_printHello(sf::Event event, void* obj) {
    Button* button = (Button*)obj;
    int width = button->getWidth();
    int height = button->getHeight();
    sf::Vector2i mousePos = sf::Mouse::getPosition(*button->linkedWindow->win);
    if (mousePos.x >= button->posX && mousePos.x <= button->posX + width) {
        if (mousePos.y >= button->posY && mousePos.y <= button->posY + height) {
            button->setColor(sf::Color::Blue);
            button->setColor(sf::Color::Green);
        }
    }
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

    windowHierarchy mainWin(width, height, "TIPE");
    Button button(200, 200, 300, 100, sf::Color::Green, sf::Color::Red, DEFAULT_FONT, "Press me!", &mainWin);
    button.setReaction(press_printHello);

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