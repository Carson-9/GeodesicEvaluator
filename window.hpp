#pragma once
#include <SFML/Graphics.hpp>
#include "utilitaries/stringManipulation.hpp"

inline void DEFAULT_VOID_FUNCTION(sf::Event){}

class windowSlaveObjects;
class windowHierarchy;

class windowHierarchy {
public:

	windowHierarchy(int width, int height, const char* title);
	windowHierarchy(sf::RenderWindow* win);
	
	~windowHierarchy();

	sf::RenderWindow* win;
	std::vector<windowSlaveObjects*> linkedObjects;
	int vectorSize;

	void executeEvent(sf::Event event);

private:
};

class windowSlaveObjects {
public:

	windowSlaveObjects(windowHierarchy* window);
	windowSlaveObjects();

	~windowSlaveObjects();

	void setReaction(void(*function)(sf::Event));
	void react(sf::Event args);
	bool isReactionEmpty();

	windowHierarchy* linkedWindow;
	int posX;
	int posY;
	sf::Event::EventType reactsTo;

private:
	int ID;
	void(*reaction)(sf::Event);
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
