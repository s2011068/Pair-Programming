
const int MapSize = 9;

class GameMap {
public:
    int gameMap[MapSize][MapSize];
public:
    void generate_gameMap();
    void save_gameMap();
};
