#include "window/window.hpp"
#include "utilitaries/algebra.hpp"
#include "terrain/terrain.hpp"

int main()
{

    const int gridSize = 1024;
    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    int octaves = 8;
    float bias = 1.8f;
    float sigmoidBlend = 100.f;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TIPE");
    sf::Image img;
    sf::Texture tex;
    sf::Sprite background;
    sf::Text infoText;
    sf::Font mainFont;

    char infoString[128];
    
    if (!mainFont.loadFromFile("Resources/firaCode.ttf")) {
        printf("Error, unrecognized font!");
        return EXIT_FAILURE;
    }
   
    infoText.setFont(mainFont);
    infoText.setCharacterSize(24);
    infoText.setFillColor(sf::Color::White);
    infoText.setStyle(sf::Text::Bold);
    sf::Vector2f infoTextPos(4.0f, 4.0f);
    infoText.setPosition(infoTextPos);

    sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", octaves, bias, (int)sigmoidBlend);
    infoText.setString(infoString);

    Terrain* terrain = new Terrain(WIDTH, HEIGHT, octaves, bias, sigmoidBlend);
    //terrain->generateFromFile("Output/tGreece.txt");
    terrain->makeSprite(background);
    img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
    tex.loadFromImage(img);
    background.setTexture(tex);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            //switch (event.type) {

            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyReleased) {
                switch (event.key.code) {

                case sf::Keyboard::Space:
                    terrain->setBlend(sigmoidBlend);
                    terrain->generateTerrain(octaves, bias);
                    terrain->makeSprite(background);
                    img.create(terrain->getSizeX(), terrain->getSizeY(), terrain->getColorMap());
                    tex.loadFromImage(img);
                    background.setTexture(tex);
                    break;


                case sf::Keyboard::Left:
                    octaves--;
                    if (octaves < 1) octaves = 1;
                    break;

                case sf::Keyboard::Right:
                    octaves++;
                    if (octaves > 16) octaves = 16;
                    break;

                case sf::Keyboard::Up:
                    bias += 0.1f;
                    if (bias > 3.0f) bias = 3.0f;
                    break;

                case sf::Keyboard::Down:
                    bias -= 0.1f;
                    if (bias < 0.2f) bias = 0.2f;
                    break;

                case sf::Keyboard::B:
                    sigmoidBlend += 5.0f;
                    break;

                case sf::Keyboard::V:
                    sigmoidBlend -= 5.0f;
                    if (sigmoidBlend < 0.0f) sigmoidBlend = 0.0f;
                    break;

                case sf::Keyboard::S:
                    writeTableToFile(terrain->getSizeY(), terrain->getSizeX(), terrain->getHeightMap(), "Output/");
                    break;


                default:
                    break;

                }

                sprintf_s(infoString, "Octaves : %d\nBias : %g\nColor Blend : %d", octaves, bias, (int)sigmoidBlend);
                infoText.setString(infoString);

            }

        }
        window.clear(sf::Color::Black);
        window.draw(background);
        window.draw(infoText);
        window.display();
    }

    return 0;
}