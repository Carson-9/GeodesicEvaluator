#pragma once

#include <SFML/Graphics.hpp>
#include "utilitaries/stringManipulation.hpp"

class windowSlaveObjects;
class windowHierarchy;

inline void DEFAULT_VOID_FUNCTION(sf::Event, void*) {}

class windowHierarchy {
public:

	windowHierarchy(int width, int height, const char* title);
	windowHierarchy(sf::RenderWindow* win);

	~windowHierarchy();

	sf::RenderWindow*					win;
	std::vector<windowSlaveObjects*>	linkedObjects;
	int									vectorSize;

	void executeEvent(sf::Event event);
	void drawObjects();
	void clearOnClose();

private:
};

class windowSlaveObjects {
public:

	windowSlaveObjects(windowHierarchy* window);
	windowSlaveObjects();

	~windowSlaveObjects();

	void setReaction(void(*function)(sf::Event, void*));
	void react(sf::Event args, void*);
	bool isReactionEmpty();

	windowHierarchy*		linkedWindow;
	int						posX;
	int						posY;
	sf::Event::EventType	reactsTo;
	virtual void drawFunction();

private:
	int ID;
	void(*reaction)(sf::Event, void*);
};


class Button : public windowSlaveObjects {

	//Anne Canteaut

public:

	Button(int posX, int posY, int width, int height, sf::Color background, sf::Color textColor, const char* fontName, const char* text, windowHierarchy* linkedWindow);
	~Button();

	int getWidth();
	int getHeight();
	
	void setColor(sf::Color color);
	void drawFunction();

private:
	int					width;
	int					height;
	sf::Text			textObject;
	sf::Font			textFont;
	sf::RectangleShape	representation;

};