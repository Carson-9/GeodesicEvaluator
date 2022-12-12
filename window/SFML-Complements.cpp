#include "SFML-Complements.hpp"

void centerText(sf::Text text, sf::Vector2f pos) {
	sf::FloatRect textBoundingBox = text.getLocalBounds();
	printf("%f, %f, %f, %f\n", textBoundingBox.width, textBoundingBox.height, textBoundingBox.left, textBoundingBox.top);
	text.setOrigin(sf::Vector2f(textBoundingBox.width / 2, textBoundingBox.height / 2));
	text.setPosition(pos);
}