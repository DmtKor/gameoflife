#include <SFML/Graphics.hpp>
#include <iostream>

const int SIZE = 100;

void draw(sf::RenderWindow &window, int(*field)[SIZE], bool update) {
    window.clear(sf::Color::White);
    sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            switch (field[i][j]) {
                case 0:
                    shape.setFillColor(sf::Color::White);
                    shape.setPosition(i * 10, j * 10);
                    window.draw(shape);
                    break;
                case 1:
                    shape.setFillColor(sf::Color::Black);
                    shape.setPosition(i * 10, j * 10);
                    window.draw(shape);
                    break;
                default:
                    shape.setFillColor(sf::Color::Red);
                    shape.setPosition(i * 10, j * 10);
                    window.draw(shape);
                    break;
            }
        }
    }
    sf::CircleShape upd(5.f);
    upd.setPosition(0, 0);
    upd.setFillColor(update? sf::Color::Green : sf::Color::Red);
    window.draw(upd);
}

void updateField(int(*field)[SIZE]) { 
    int newField[SIZE][SIZE] = {};
    int neighbors = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            neighbors = 0;
            for(int i2 = (i > 0? i - 1 : i); i2 <= (i < (SIZE - 1)? i + 1 : i); i2++) {
                for(int j2 = (j > 0? j - 1 : j); j2 <= (j < (SIZE - 1)? j + 1 : j); j2++) {
                    if(!(i2 == i && j2 == j) && field[i2][j2] == 1) neighbors += 1;
                }
            }
            if (field[i][j] == 0 && neighbors == 3) newField[i][j] = 1;
            else if (field[i][j] == 1 && (neighbors > 3 || neighbors < 2)) newField[i][j] = 0;
            else newField[i][j] = field[i][j];
        }
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            field[i][j] = newField[i][j];
        }
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(10 * SIZE, 10 * SIZE), "Game of Life", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    
    int field[SIZE][SIZE] {};
    bool update = false;
    float speed = 2;

    sf::Time elapsed = sf::seconds(0);
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyReleased && window.hasFocus())
            {
                if (event.key.code == sf::Keyboard::Space) update = !update;
                else if (event.key.code == sf::Keyboard::BackSpace) {
                    for (int i = 0; i < SIZE; i++) {
                        for (int j = 0; j < SIZE; j++) {
                            field[i][j] = 0;
                        }
                    }
                }
                else if (event.key.code == sf::Keyboard::Down && speed < 20  && window.hasFocus()) speed += 0.5;
                else if (event.key.code == sf::Keyboard::Up && speed > 0.5 && window.hasFocus()) speed -= 0.5;
            }
            
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !update && window.hasFocus()) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            pos.x = pos.x / 10;
            pos.y = pos.y / 10;
            if (field[pos.x][pos.y] == 0) field[pos.x][pos.y] = 1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !update && window.hasFocus()) {
            sf::Vector2i pos = sf::Mouse::getPosition(window);
            pos.x = pos.x / 10;
            pos.y = pos.y / 10;
            if (field[pos.x][pos.y] == 1) field[pos.x][pos.y] = 0;
        }

        if (update && (elapsed.asSeconds() == 0)) {
            elapsed = sf::seconds(1);
            clock.restart();
        }
        else if (update && (elapsed.asSeconds() < speed))
        {
            elapsed += clock.getElapsedTime();
            clock.restart();
        }
        else if (update && (elapsed.asSeconds() >= speed))
        {
            updateField(field);
            elapsed = sf::seconds(0);
            clock.restart();
        }
        
        draw(window, field, update);
        window.display();
    }

    return 0;
}