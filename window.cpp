#include "window.hpp"


void windowHierarchy::executeEvent(sf::Event::EventType event) {
    for (auto& currentObject : this->linkedObjects) {
        if (currentObject != NULL && currentObject->reactsTo == event) {
            currentObject->reaction();
        }
    }
}

windowHierarchy::windowHierarchy(int width, int height, const char* title) {
    this->win = new sf::RenderWindow(sf::VideoMode(width, height), title);
    this->linkedObjects = std::vector<windowSlaveObjects*>();
    this->vectorSize = 0;
}

windowHierarchy::windowHierarchy(sf::RenderWindow* win) {
    this->win = win;
    this->linkedObjects = std::vector<windowSlaveObjects*>();
    this->vectorSize = 0;
}

windowSlaveObjects::windowSlaveObjects(windowHierarchy* win) {
    this->posX = 0;
    this->posY = 0;
    this->reactsTo = sf::Event::LostFocus;
    this->ID = win->vectorSize;
    this->linkedWindow = win;
    win->vectorSize++;

    win->linkedObjects.push_back(this);
}

windowSlaveObjects::~windowSlaveObjects(){
    if(this->linkedWindow != NULL) this->linkedWindow->linkedObjects.at(this->ID) = NULL;
}

windowSlaveObjects::windowSlaveObjects(){
    this->posX = 0;
    this->posY = 0;
    this->reactsTo = sf::Event::LostFocus;
    this->linkedWindow = NULL;
    this->ID = -1;
}

Button::Button(int posX, int posY, int width, int height, sf::Color background, const char* text, windowHierarchy* linkedWindow) : windowSlaveObjects(linkedWindow) {

    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;

    this->background = sf::Color(background);

    this->textObject = sf::Text();
    this->textObject.setString(sf::String(text));

    this->linkedWindow = linkedWindow;

    this->reactsTo = sf::Event::MouseLeft;
}


Button::~Button() {
    delete &this->textObject;
    delete &this->background;
}



void baseWindow(int width, int height) {

    sf::Font mainFont;
    sf::Text title;
    sf::Vector2f titlePos(width / 2, 16.0f);
    title.setPosition(titlePos);
    title.setString("TIPE");

    if (!mainFont.loadFromFile("Resources/firaCode.ttf")) {
        printf("Error, unrecognized font!");
        return;
    }


	sf::RenderWindow window(sf::VideoMode(width, height), "TIPE");

	while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {

                }

            }
        }

        window.clear(sf::Color::Black);
        window.draw(title);
        window.display();
	}

}