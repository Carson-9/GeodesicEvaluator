#include <SFML/Graphics.hpp>
#include "algebra.hpp"

int main()
{

    
    Vector3<int> A(0, 1, 0);
    Vector3<int> B(1, 0, 1);
    Vector3<int> C = A + B;
    Vector3<int> D = A ^ B;
    printf("Magnitude of C : %d, Dot product : %d\n", C.MagnitudeSquared(), A * B);
    C.log();
    D.log();

    D.Normalize();
    D.log();

    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}