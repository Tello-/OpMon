/*
Battle.hpp
Author : Cyrielle
Contributor : Navet56
File under GNU GPL v3.0 license
*/
#pragma once

#include "../../utils/CycleCounter.hpp"
#include "../model/objects/Turn.hpp"
#include "../model/storage/BattleData.hpp"
#include "../start/Core.hpp"
#include "Dialog.hpp"
#include "Elements.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <queue>

namespace OpMon {

    namespace Model {
        class OpTeam;
        class BattleData;
    } // namespace Model

    namespace View {

        class Battle {
          private:
            //The OpMons sprites
            sf::Sprite atk;
            sf::Sprite def;

            //The OpMons Transform objects used for the animations
            sf::Transform atkTr;
            sf::Transform defTr;

            const Model::OpTeam *atkTeam;
            const Model::OpTeam *defTeam;

            std::queue<Transformation> *currentOpAnims = nullptr;

            //-1 means uninitalized here
            int atkHp = -1;
            int defHp = -1;

            int turnNber = 0;

            //True if the turn have been calculated
            bool turnLaunched = false;

            sf::Sprite background;
            sf::Sprite playerSpr;
            sf::Sprite trainerSpr;
            sf::Sprite dialogSpr;
            sf::Text choicesTxt[4];
            sf::Vector2f posChoices[4];
            sf::Sprite cursor;
            sf::Text waitText;

            sf::Text opName[2];
            sf::Text opLevel[2];
            sf::Text opHp;

            sf::Sprite healthbar1[2];
            sf::Sprite healthbar2[2];
            sf::Sprite infoboxPlayer;
            sf::Sprite infoboxTrainer;
            sf::Sprite shadowTrainer;
            sf::Sprite shadowPlayer;

            Utils::CycleCounter curPos = Utils::CycleCounter(4);

            /* If true, the attack selection dialog is printed. If false, the action selection dialog is printed. */
            bool attackChoice = false;

            sf::Text attacks[4];
            //Prints the pp count
            sf::Text ppTxt;
            //Just prints the text "PP:"
            sf::Text ppStrTxt;
            sf::Sprite type;

            sf::Sprite dialogArrow;

            int phase = 0;

            Dialog *dialog = nullptr;

            Model::BattleData &data;

            bool dialogOver = true;

          public:
            Battle(const Model::OpTeam *atkTeam, const Model::OpTeam *defTeam, std::string trainerClass, std::string background, Model::BattleData &data);

            GameStatus operator()(sf::RenderTexture &frame, Model::TurnData const &atk, Model::TurnData const &def, std::queue<Model::TurnAction> &actionQueue, bool *turnActivated, bool atkFirst);
            //Moves the cursor
            void moveCur(Model::Side where);
            //Returns the cursor's position
            int getCurPos() { return curPos.getValue(); }
            //Tooggle the interface printed, the action or attack selection
            void toggleAttackChoice();

            int getPhase() { return phase; }

            bool isAttackChoice() { return attackChoice; }

            void passDialog() {
                if(dialog != nullptr)
                    dialog->pass();
            }
        };

    } // namespace View
} // namespace OpMon
