#include <SFML/Graphics.hpp>

#include <thread>
#include <chrono>
auto drawGrid(int& offset, sf::RenderWindow& window){
    for (int i = 0; i <= (window.getSize().x / offset) + 1; i++){
        sf::RectangleShape shape (sf::Vector2f(1, window.getSize().x));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(offset * i, 0);
        window.draw(shape);
    }

    for (int i = 0; i <= (window.getSize().y / offset) + 1; i++){
        sf::RectangleShape shape (sf::Vector2f(window.getSize().y, 1));
        shape.setFillColor(sf::Color::White);
        shape.setPosition(0, offset * i);
        window.draw(shape);
    }
}

auto drawSingleSquare(int& offset, sf::RenderWindow& window, int position, int rows){
    sf::RectangleShape shape (sf::Vector2f(offset, offset));
    shape.setFillColor(sf::Color::White);
    shape.setPosition((position % rows) * offset, (position / rows) * offset);
    window.draw(shape);
}

auto drawSquares(std::vector<std::vector<int>>& map, int& offset, sf::RenderWindow& window, int gridSize){
    for (int i = 0; i < map.size(); i++){
        for (int j = 0; j < map[i].size(); j++){
            if (map[i][j])
                drawSingleSquare(offset, window, i * map[i].size() + j, gridSize);
        }
    }
}

auto isLineComplete(std::vector<int>& line) -> bool{
    int sum = 0;
    for (auto el : line){
        sum += el;
    }
    return sum == line.size();
}

auto getPhysicsRange(std::vector<std::vector<int>>& map) -> std::vector<int>{

    std::vector<int> borders(2, map.size());

    for (int i = map.size() - 1; i >= 0; i--){

        bool isEmpty = true;

        for (int j = 0; j < map[i].size(); j++){
            if (map[i][j]){
                isEmpty = false;
                if (isLineComplete(map[i]))
                    borders[0] = i;
                break;
            }
        }

        if (!isEmpty)
            borders[1] = i;
    }

    return borders;

}

auto makePhysicStep(std::vector<std::vector<int>>& map){

    std::vector<std::vector<int>> newMap = map;

    auto borders = getPhysicsRange(map);

    for (int i = borders[1]; i < borders[0]; i++){
        for (int j = 0; j < map[i].size(); j++){
            if (map[i][j]){

                if (i + 1 < map.size() && !map[i + 1][j]){
                    newMap[i][j] = 0;
                    newMap[i + 1][j] = 1;
                }else if (i > borders[1] && j + 1 < map[0].size() && i + 1 < map.size() && !map[i + 1][j + 1]){
                    newMap[i][j] = 0;
                    newMap[i + 1][j + 1] = 1;
                }else if (i > borders[1] && j - 1 >= 0 && i + 1 < map.size() && !map[i + 1][j - 1]){
                    newMap[i][j] = 0;
                    newMap[i+1][j-1] = 1;
                }
            }
        }
    }

    map = newMap;
}

auto getPosFromMouse(sf::Vector2f& mousePos, int offset, int gridSize) -> int{
    int x = mousePos.x / offset;
    int y = mousePos.y / offset;
    if (mousePos.x <= offset)
        x = 0;
    if (mousePos.y <= offset)
        y = 0;
    return x + y * gridSize;
}


int main(){

    int rows = 80;
    int columns = 80;
    int offset = 4;

    std::vector<std::vector<int>> map(rows, std::vector<int>(columns, 0));

    sf::RenderWindow window(sf::VideoMode(rows * offset + 1, columns * offset + 1), "SandSim");

    while (window.isOpen()){

        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed){

                sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                int pos = getPosFromMouse(worldPos, offset, rows);

                int elRow = pos/columns;
                int elColumn = pos - elRow * columns;

                if (!map[elRow][elColumn])
                    map[elRow][elColumn] = 1;
                else
                    map[elRow][elColumn] = 0;

            }

            if (event.type == sf::Event::MouseMoved){
                sf::Vector2<int> mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                int pos = getPosFromMouse(worldPos, offset, rows);

                int elRow = pos/columns;
                int elColumn = pos - elRow * columns;

                if (!map[elRow][elColumn])
                    map[elRow][elColumn] = 1;
            }
        }

        window.clear(sf::Color::Black);
        drawSquares(map, offset, window, rows);
        window.display();
        makePhysicStep(map);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}