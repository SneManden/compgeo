#include <math.h>
#include "../linsegintersect.h"
#include <SFML/Graphics.hpp>

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


class LineSegment : public sf::Drawable {
public:
    LineSegment(const sf::Vector2f& _p, const sf::Vector2f& _q, double _radius = 2.0)
    : p(_p), q(_q), points(sf::LinesStrip, 2) {
        radius = _radius;
        dotp.setPosition(_p.x-_radius/2, _p.y-_radius/2);
        dotp.setRadius(_radius);
        dotq.setPosition(_q.x-_radius/2, _q.y-_radius/2);
        dotq.setRadius(_radius);
        points[0].position = _p;
        points[1].position = _q;
    }
    void movePoint(int index, const sf::Vector2f& pos) {
        switch (index) {
            case 0: p.x = pos.x; p.y = pos.y; break;
            case 1: q.x = pos.x; q.y = pos.y; break;
            default: return;
        }
        // index is either 0 or 1 (else switch will have returned)
        points[index].position = pos;
        // dots
        if (index == 0)
            dotp.setPosition(pos.x-radius/2, pos.y-radius/2);
        else
            dotq.setPosition(pos.x-radius/2, pos.y-radius/2);
    }
    void setRadius(double radius) {
        dotp.setRadius(radius);
        dotq.setRadius(radius);
    }
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // Draw end points
        target.draw(dotp);
        target.draw(dotq);
        // Draw line
        target.draw(points);
    }
    double radius;
    sf::Vector2f p, q;
    sf::VertexArray points;
    sf::CircleShape dotp, dotq;
};


int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Line Segment Intersecionts");

    LineSegment linseg(sf::Vector2f(50,50), sf::Vector2f(WINDOW_WIDTH-250,WINDOW_HEIGHT-250), 5.0);

    int x, y;
    double t = 0.0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // :: Update start
        t = 0.1;
        x = WINDOW_WIDTH -150 + 50*cos(t);
        y = WINDOW_HEIGHT-150 + 50*sin(t);
        linseg.movePoint(1, sf::Vector2f(x, y));
        // :: Update end

        window.clear();
        // :: Draw start
        window.draw(linseg);
        // :: Draw end
        window.display();
    }

    return 0;
}
