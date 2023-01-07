#include "window/windowObjects.hpp"


void buttonTestReaction(sf::Event event, windowSlaveObjects* obj) {

}


void windowHierarchy::executeEvent(sf::Event event) {
    for (auto& currentObject : this->linkedObjects) {
        if (currentObject != NULL && currentObject->reactsTo == event.type) {
            if (!currentObject->isReactionEmpty()) currentObject->react(event, (void*)currentObject);
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

windowHierarchy::~windowHierarchy() {
    //this->win->close();
    //delete this->win;
    //this->linkedObjects.~vector();
}

void windowHierarchy::drawObjects() {
    for (auto& currentObject : this->linkedObjects) {
        //printf(typeid(currentObject).name());
        if (currentObject != NULL) currentObject->drawFunction();
    }
}


void windowHierarchy::clearOnClose() {
    delete this->win;
}

windowSlaveObjects::windowSlaveObjects(windowHierarchy* win) {
    this->posX = 0;
    this->posY = 0;
    this->reactsTo = sf::Event::LostFocus;
    this->ID = win->vectorSize;
    this->linkedWindow = win;
    win->vectorSize++;

    win->linkedObjects.push_back(this);
    this->reaction = DEFAULT_VOID_FUNCTION;
}

windowSlaveObjects::~windowSlaveObjects() {
    if (this->linkedWindow != NULL) this->linkedWindow->linkedObjects.at(this->ID) = NULL;
}

windowSlaveObjects::windowSlaveObjects() {
    this->posX = 0;
    this->posY = 0;
    this->reactsTo = sf::Event::LostFocus;
    this->linkedWindow = NULL;
    this->ID = -1;
    this->reaction = DEFAULT_VOID_FUNCTION;
}

void windowSlaveObjects::setReaction(void(*function)(sf::Event, void*)) {
    this->reaction = function;
}

void windowSlaveObjects::react(sf::Event args, void* obj) {
    this->reaction(args, obj);
}

bool windowSlaveObjects::isReactionEmpty() {
    return (this->reaction == NULL);
}

void windowSlaveObjects::drawFunction() {
    printf("Error! No draw function defined for : %p!\n", this);
}

Button::Button(int posX, int posY, int width, int height, sf::Color background, sf::Color textColor, const char* fontName, const char* text, windowHierarchy* linkedWindow) : windowSlaveObjects(linkedWindow) {

    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;

    if (!this->textFont.loadFromFile(fontName)) printf("Error, unrecognized font!");
    

    this->textObject = sf::Text();
    this->textObject.setString(sf::String(text));
    this->textObject.setPosition(sf::Vector2f((float)posX,(float)posY));
    this->textObject.setFillColor(textColor);
    this->textObject.setCharacterSize(24);
    this->textObject.setFont(this->textFont);
    this->textObject.setStyle(sf::Text::Bold);
    //centerText(this->textObject, sf::Vector2f((float)(this->posX + 2 * this->width)/ 2.0f, ((float)(this->posY + 2 * this->height)/ 2.0f)));

    this->linkedWindow = linkedWindow;

    this->reactsTo = sf::Event::MouseButtonPressed;
    this->representation = sf::RectangleShape();
    this->representation.setPosition(sf::Vector2f((float)posX, (float)posY));
    this->representation.setSize(sf::Vector2f((float)width, (float)height));
    this->representation.setFillColor(background);
}


void Button::drawFunction(){
    this->linkedWindow->win->draw(this->representation);
    this->linkedWindow->win->draw(this->textObject);
}

Button::~Button() {
}

int Button::getWidth() {
    return this->width;
}

int Button::getHeight() {
    return this->height;
}

void Button::setColor(sf::Color color) {
    this->representation.setFillColor(color);
}




Slider::Slider(int posX, int posY, int length, int height, float lower, float upper, sf::Color background, sf::Color sliderColor, windowHierarchy* linkedWindow) {
    this->posX = posX;
    this->posY = posY;
    this->length = length;
    this->height = height;

    this->lowerBound = lower;
    this->upperBound = upper;

    this->bar.setFillColor(background);
    this->slider.setFillColor(sliderColor);
    this->slider.setOrigin(sf::Vector2f(posX, posY + (height/2.0f)));

    this->reactsTo = sf::Event::MouseButtonPressed;
    this->sliderPos = 0.0f;
}

Slider::~Slider() {

}

float Slider::getSliderPos() {
    return this->sliderPos;
}

void Slider::setSliderPos(float pos) {
    this->sliderPos = pos;
    this->slider.setPosition(pos * this->length , 0);
}