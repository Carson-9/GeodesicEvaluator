#pragma once

#include "../utilitaries/stringManipulation.hpp"
#include "../window/SFML-Complements.hpp"
#include "../utilitaries/types.hpp"

class windowSlaveObjects;
class windowHierarchy;

inline void DEFAULT_VOID_FUNCTION(sf::Event, void*) {}

// Transforms sf::EventType into a Flag, used to quickly determine whether 
// an object reacts to an eventType
inline int eventTypeToFlag(sf::Event::EventType t) { return 1 << t; }


class windowHierarchy {
public:

	windowHierarchy(int width, int height, const char* title, sf::Vector2i ScreenPos);
	windowHierarchy(sf::RenderWindow* win);

	~windowHierarchy();

	sf::RenderWindow*					win;
	std::vector<windowSlaveObjects*>	linkedObjects;
	int									vectorSize;

	void executeEvent(sf::Event event);
	void drawObjects();
	void clearOnClose();

	int Width;
	int Height;

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
	int						reactsTo;  //Flag
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
	void toggleIsPressed();
	bool isBeingPressed();
	void drawFunction();

private:
	int					width;
	int					height;
	bool				isPressed;
	sf::Text			textObject;
	sf::Font			textFont;
	sf::RectangleShape	representation;

};

class Slider : public windowSlaveObjects {
	public:

		Slider(int posX, int posY, int length, int height, float lower, float upper, sf::Color background, sf::Color sliderColor, windowHierarchy* linkedWindow);
		~Slider();

		void setSliderPos(float pos);
		void setSliderRadius(float r);
		void toggleBeingMoved();

		float getSliderPos();
		int getLength();
		int getHeight();
		float getLowerBound();
		float getUpperBound();
		float getSliderRadius();
		float getValue();
		sf::Vector2f getSliderAbsolutePos();

		bool isBeingMoved();
		void drawFunction();

	private:

		int		length;
		int		height;
		float	sliderRadius;
		float	lowerBound;
		float	upperBound;
		float	sliderPos;
		bool	beingMoved;

		sf::RectangleShape bar;
		sf::CircleShape slider;
};


class WindowPoint : public windowSlaveObjects {

public:

	WindowPoint(f32 posX, f32 posY, f32 radius, sf::Color pointColor, windowHierarchy* linkedWindow);
	~WindowPoint();

	f32 getX();
	f32 getY();
	f32 getRadius();

	bool isBeingMoved();
	void drawFunction();
	void setPosition(f32 x, f32 y);
	void toggleBeingMoved();

	Point* getReference();


private:

	b8 beingMoved;
	f32 radius;

	Point actualPoint;

	sf::CircleShape shape;

};