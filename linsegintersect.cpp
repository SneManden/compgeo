#include "linsegintersect.h"
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Line Segment Intersecionts");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        // :: Draw start
        // :: Draw end
        window.display();
    }

    return 0;
}
