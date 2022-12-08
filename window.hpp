#pragma once
#include <SFML/Graphics.hpp>
#include "stringManipulation.hpp"


class windowSlaveObjects;
class windowHierarchy;

class windowHierarchy {
public:

	windowHierarchy(int width, int height, const char* title);
	windowHierarchy(sf::RenderWindow* win);

	sf::RenderWindow* win;
	std::vector<windowSlaveObjects*> linkedObjects;
	int vectorSize;

	void executeEvent(sf::Event::EventType event);

private:
};

class windowSlaveObjects {
public:

	windowSlaveObjects(windowHierarchy* window);
	windowSlaveObjects();

	~windowSlaveObjects();

	void reaction() {};

	windowHierarchy* linkedWindow;
	int posX;
	int posY;
	sf::Event::EventType reactsTo;

private:
	int ID;
};


class Button : public windowSlaveObjects {

	//Anne Canteaut

public:

	Button(int posX, int posY, int width, int height, sf::Color background, const char* text, windowHierarchy* linkedWindow);
	~Button();


private:
	int width;
	int height;
	sf::Color background;
	sf::Text textObject;

};



void baseWindow(int width, int height);
