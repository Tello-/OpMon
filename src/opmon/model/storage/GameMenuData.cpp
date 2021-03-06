/*
GameMenuData.cpp
Author : Cyrielle
File under GNU GPL v3.0 license
*/

#include "GameMenuData.hpp"

namespace OpMon {
    namespace Model {

        GameMenuData::GameMenuData(UiData *data, Player *player)
          : uidata(data)
          , player(player) {
            ResourceLoader::load(menuTexture, "backgrounds/menu.png");
            ResourceLoader::loadTextureArray(selectionTexture, "backgrounds/menuS%d.png", 6, 1);

            selectionPos[0] = sf::Vector2f(106, 77);
            selectionPos[1] = sf::Vector2f(252, selectionPos[0].y);
            selectionPos[2] = sf::Vector2f(selectionPos[0].x - 9, selectionPos[0].y + 100);
            selectionPos[3] = sf::Vector2f(selectionPos[1].x, selectionPos[2].y - 4);
            selectionPos[4] = sf::Vector2f(selectionPos[0].x, selectionPos[2].y + 115);
            selectionPos[5] = sf::Vector2f(selectionPos[1].x, selectionPos[4].y);
        }

    } // namespace Model
} // namespace OpMon
