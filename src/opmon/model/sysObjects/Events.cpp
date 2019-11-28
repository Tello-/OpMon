/*
Events.cpp
Author : Cyrielle
Contributors : BAKFR, Stelyus
File under GNU GPL v3.0 license
*/
#include "Events.hpp"

#include "../../../utils/defines.hpp"
#include "../../../utils/log.hpp"
#include "../../start/Core.hpp"
#include "../../view/Overworld.hpp"
#include "Position.hpp"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"

namespace OpMon {
    namespace Model {

       namespace Events {

            bool justTP = false; // again why is this here

            sf::Sound doorSound;     // doesn't belong here
            sf::Sound shopdoorSound; // doesn't belong here

            namespace DoorType {
                std::vector<sf::Texture> NORMAL, SHOP; // this doesn't belong here
            }
        }

						namespace Events { // TPEvent implementation

								TPEvent::TPEvent(const EventTrigger &_eventTrigger, const sf::Vector2f &_mapPosition, sf::Vector2i const &_tpPos, std::string const &_map,
                                 Side _ppDir, int _sides, bool _passable)
									: Event{Position{_mapPosition, true}, _eventTrigger, _sides, _passable}
									, m_tpCoord{_tpPos}
									, m_map{_map} {}

								TPEvent::TPEvent(const TPEvent &_other)
									: Event(_other.m_mapPos, _other.m_eventTrigger, m_sides, m_passable)
									, m_tpCoord{ _other.m_tpCoord}
									, m_map{_other.m_map} {}


								void TPEvent::action(Player &player, View::Overworld &overworld) {
										std::cout << "TPEvent at: " << m_map << " "
															<< m_mapPos.getPosition().x << ", " << m_mapPos.getPosition().y << std::endl;
								}

                void TPEvent::update(Player &player, View::Overworld &overworld) {
                }
            } // namespace Events -- TPEvent

						namespace Events { // SoundEvent implementation
                SoundEvent::SoundEvent(const EventTrigger &_eventTrigger, const Position &_mapPosition, const sf::Sound &_sound, const std::string &_map,
                                       Side _ppDir, int _sides, bool _passable)
                  : Event{_mapPosition, _eventTrigger, _sides, _passable}
                  , m_sound{_sound}
                  , m_map{_map}
                  , m_ppDir{_ppDir} {}
                SoundEvent::SoundEvent(const SoundEvent &_other)
                  : Event(_other.m_mapPos, _other.m_eventTrigger, m_sides, m_passable)
                  , m_sound{_other.m_sound}
                  , m_map{_other.m_map}
                  , m_ppDir{_other.m_ppDir} {}
                void SoundEvent::update(Player &_player, View::Overworld &_overworld) {
                }
                void SoundEvent::action(Player &_player, View::Overworld &_overworld) {
                    std::cout << "SoundEvent at: " << m_map << " "
                              << m_mapPos.getPosition().x << ", " << m_mapPos.getPosition().y << std::endl;
									//m_sound.play();
                }
            } // namespace Events -- Sound Event

						namespace Events { // Animation Event
                AnimationEvent::AnimationEvent(const Position &_mapPosition, const EventTrigger &_eventTrigger, const std::string &_map,
                                               int _sides = SIDE_ALL, bool _passable = true)
                  : Event{_mapPosition, _eventTrigger, _sides, _passable}
                  , m_map{_map} {}
                AnimationEvent::AnimationEvent(const AnimationEvent &_other)
                  : Event(_other.m_mapPos, _other.m_eventTrigger, m_sides, m_passable)
                  , m_map{_other.m_map} {}
                void AnimationEvent::update(Player &_player, View::Overworld &_overworld) { /* Move to next frame..? */}
                void AnimationEvent::action(Player &_player, View::Overworld &_overworld) { /* Begin Animation sequence..? */}
                } // namespace Events -- Animation Event


						namespace Events { // DoorEvent

                DoorEvent::DoorEvent(const TPEvent &_tpEvent, const SoundEvent &_soundEvent, const AnimationEvent& _animationEvent)
                  : m_tpEvent{_tpEvent}
                  , m_soundEvent{_soundEvent}
                  , m_animationEvent{_animationEvent}
            {}
										void DoorEvent::update(Player &_player, View::Overworld &_overworld) {}
                    void DoorEvent::action(Player &_player, View::Overworld &_overworld) {
                        std::cout << "DoorEvent triggered" << _player.getPosition().getPosition().x << ", " << _player.getPosition().getPosition().y << std::endl;
                        m_tpEvent.action(_player, _overworld);
                        m_soundEvent.action(_player, _overworld);
                        m_animationEvent.action(_player, _overworld);
                    }
            } // namespace events --DoorEvent macro event
											 					 

            //DoorEvent::DoorEvent(std::vector<sf::Texture> &doorTextures, std::string doorType, sf::Vector2f const &position, sf::Vector2i const &tpPos,
            //                     std::string const &map, EventTrigger eventTrigger, Side ppDir, int sides, bool passable)
            //  : Event(doorTextures, eventTrigger, position, sides, passable)
            //  , TPEvent(doorTextures, eventTrigger, position, tpPos, map, ppDir, sides, passable)
            //  , doorType(doorType) {
            //    //The doors doesn't fit exactly.
            //    this->position += sf::Vector2f(0, -6);
            //    if(doorType == "shop door") {
            //        this->position.x -= 4;
            //    }
            //    if(doorType != "door" && doorType != "shop door") {
            //        handleError(std::string("Warning - DoorEvent : Unknown door type \" ") + doorType + "\"");
            //    }
            //}

            //TalkingEvent::TalkingEvent(std::vector<sf::Texture> &otherTextures, sf::Vector2f const &position,
            //                           Utils::OpString const &dialogKey, int sides, EventTrigger eventTrigger,
            //                           bool passable)
            //  : Event(otherTextures, eventTrigger, position, sides, passable)
            //  , dialogKey(dialogKey) {
            //    this->onLangChanged();
            //}

            //void TalkingEvent::onLangChanged() {
            //    dialog.clear();
            //    dialog = dialogKey.getString();
            //}

            //LockedDoorEvent::LockedDoorEvent(std::vector<sf::Texture> &doorTextures, std::string doorType, Item *needed, sf::Vector2f const &position,
            //                                 sf::Vector2i const &tpPos, std::string const &map, Side ppDir,
            //                                 EventTrigger eventTrigger, bool consumeItem, int sides, bool passable)
            //  : DoorEvent(doorTextures, doorType, position, tpPos, map, eventTrigger, ppDir, sides, passable)
            //  , Event(doorTextures, eventTrigger, position, sides, passable)
            //  , TalkingEvent(doorTextures, position, LockedDoorEvent::keysLock, sides, eventTrigger, passable)
            //  , needed(needed)
            //  , consumeItem(consumeItem) {
            //}

            //CharacterEvent::CharacterEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position, Side posDir, MoveStyle moveStyle,
            //                               EventTrigger eventTrigger, std::vector<Side> predefinedPath, bool passable,
            //                               int sides)
            //  : Event(textures, eventTrigger, position, sides, passable)
            //  , moveStyle(moveStyle) {
            //    sprite->setScale(2, 2);
            //    sprite->setOrigin(16, 16);
            //    this->position += sf::Vector2f(16, 0);
            //    setPredefinedMove(predefinedPath);
            //    mapPos.setDir(posDir);
            //}

            //TalkingCharaEvent::TalkingCharaEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position,
            //                                     Utils::OpString const &dialogKey, Side posDir, EventTrigger eventTrigger,
            //                                     MoveStyle moveStyle, std::vector<Side> predefinedPath, bool passable,
            //                                     int sides)
            //  : Event(textures, eventTrigger, position, sides, passable)
            //  , CharacterEvent(textures, position, posDir, moveStyle, eventTrigger, predefinedPath, passable, sides)
            //  , TalkingEvent(textures, position, dialogKey, sides, eventTrigger, passable) {
            //}

            //TrainerEvent::TrainerEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position, OpTeam *team, Utils::OpString const &dialogKeys, Utils::OpString const &defeatedDialog, Side posDir, EventTrigger eventTrigger, MoveStyle moveStyle, std::vector<Side> predefinedPath, bool passable, int side)
            //  : Event(textures, eventTrigger, position, side, passable)
            //  , TalkingCharaEvent(textures, position, dialogKeys, posDir, eventTrigger, moveStyle, predefinedPath, passable, side)
            //  , team(team)
            //  , defeatedDialog(defeatedDialog) {
            //}

            //TrainerEvent::~TrainerEvent() {
            //    delete(team);
            //}

            //TrainerEyesightEvent::TrainerEyesightEvent(TrainerEvent *trainer, sf::Vector2f position)
            //  : Event(alpha, EventTrigger::BE_IN, position, SIDE_ALL, true)
            //  , trainer(trainer) {
            //}

            ////Actions and updates

           

            //

            //void DoorEvent::action(Model::Player &player, View::Overworld &overworld) {
            //    //Starts the animation. The animation itself will be done in update()
            //    animStarted = 0;
            //    player.getPosition().lockMove();
            //    overworld.getData().getUiDataPtr()->getJukebox().playSound(doorType + " sound");
            //}

            //void DoorEvent::update(Model::Player &player, View::Overworld &overworld) {
            //    if(animStarted != -1) { //If true, action() have started the animation
            //        ++animStarted;
            //        if(animStarted < 8 && (animStarted / 2) * 2 == animStarted) {
            //            currentTexture = otherTextures.begin() + (animStarted / 2);
            //        } else if(animStarted > 10) { //End of the animation
            //            player.getPosition().unlockMove();
            //            TPEvent::action(player, overworld);
            //            animStarted = -1;
            //            currentTexture = otherTextures.begin();
            //        }
            //    }
            //}

            //void TalkingEvent::action(Model::Player &player, View::Overworld &overworld) {
            //    overworld.startDialog(this->dialog);
            //}

            //void TalkingEvent::update(Model::Player &player, View::Overworld &overworld) {
            //}

            //void TalkingEvent::changeDialog(Utils::OpString newDialog) {
            //    dialogKey = newDialog;
            //    this->onLangChanged();
            //}

            //void CharacterEvent::update(Model::Player &player, View::Overworld &overworld) {
            //    frames++;
            //    if(!mapPos.isAnim()) { //Checks if not already moving
            //        int randomMove;
            //        switch(moveStyle) {
            //        case MoveStyle::PREDEFINED: //Movement predefined during the npc's initialization
            //            predefinedCounter++;
            //            if(predefinedCounter >= movements.size()) { //TODO : Add the possibility of disabling the loop, for one-time movements
            //                predefinedCounter = 0;
            //            }

            //            if(!move(movements[predefinedCounter], overworld.getData().getCurrentMap())) { //If the movement is impossible, decreases the counter to not ignore the movement.
            //                if(predefinedCounter != 0) {
            //                    predefinedCounter--;
            //                } else {
            //                    predefinedCounter = movements.size() - 1;
            //                }
            //            }
            //            break;

            //        case MoveStyle::NO_MOVE: //This is easy to do.
            //            break;

            //        case MoveStyle::RANDOM: //I don't think I will be using this often, but I keep it here, who knows ?
            //            randomMove = Utils::Misc::randUI(5) - 1;
            //            switch(randomMove) {
            //            case -1:
            //                move(Side::NO_MOVE, overworld.getData().getCurrentMap());
            //                break;
            //            case 0:
            //                move(Side::TO_UP, overworld.getData().getCurrentMap());
            //                break;
            //            case 1:
            //                move(Side::TO_DOWN, overworld.getData().getCurrentMap());
            //                break;
            //            case 2:
            //                move(Side::TO_LEFT, overworld.getData().getCurrentMap());
            //                break;
            //            case 3:
            //                move(Side::TO_RIGHT, overworld.getData().getCurrentMap());
            //                break;
            //            default: //This would be weird
            //                handleError("[WARNING] - Random number out of bounds CharacterEvent::update");
            //                move(Side::NO_MOVE, overworld.getData().getCurrentMap());
            //            }
            //            break;

            //        case MoveStyle::FOLLOWING:
            //            //TODO
            //            break;
            //        }
            //    }
            //    //Changes the texture to print, handles the movement itself.
            //    if(mapPos.isAnim() && !anims && mapPos.getDir() != Side::STAY) { //First part of the animation
            //        currentTexture = otherTextures.begin() + ((int)mapPos.getDir() + 4);
            //        animsCounter++;
            //        anims = animsCounter > 8;
            //    } else if(mapPos.isAnim() && anims && mapPos.getDir() != Side::STAY) { //Second part of the animation
            //        currentTexture = otherTextures.begin() + ((int)mapPos.getDir() + 8);
            //        animsCounter++;
            //        if(animsCounter > 16) {
            //            anims = false;
            //            animsCounter = 0;
            //        }
            //    } else if(!mapPos.isAnim()) { //The NPC is resting. With all these movements, maybe it's tired.
            //        currentTexture = otherTextures.begin() + (int)mapPos.getDir();
            //    }

            //    //This part moves the sprite's position
            //    if(mapPos.isAnim()) {
            //        sf::Vector2f toMove;
            //        switch(mapPos.getDir()) {
            //        case Side::TO_UP:
            //            toMove = sf::Vector2f(0, -4);
            //            break;

            //        case Side::TO_DOWN:
            //            toMove = sf::Vector2f(0, 4);
            //            break;

            //        case Side::TO_LEFT:
            //            toMove = sf::Vector2f(4, 0);
            //            break;

            //        case Side::TO_RIGHT:
            //            toMove = sf::Vector2f(-4, 0);
            //            break;
            //        default:
            //            break;
            //        }
            //        if(mapPos.isMoving())
            //            position -= toMove;
            //        if(frames - startFrames >= 7)
            //            mapPos.stopMove();
            //    }
            //}

            //bool CharacterEvent::move(Side direction, Map *map) {
            //    startFrames = frames;
            //    return mapPos.move(direction, map);
            //}

            //void CharacterEvent::move(Side direction, Model::Player &player, View::Overworld &overworld) {
            //    move(direction, overworld.getData().getCurrentMap());
            //}

            //void TalkingCharaEvent::action(Model::Player &player, View::Overworld &overworld) {
            //    mapPos.lockMove();
            //    talking = true;
            //}

            //void TalkingCharaEvent::update(Model::Player &player, View::Overworld &overworld) {
            //    CharacterEvent::update(player, overworld);
            //    if(talking && !mapPos.isAnim()) {           //Talking is set by "action".
            //        switch(player.getPosition().getDir()) { //Put the character's face in front of the player's one
            //        case Side::TO_DOWN:
            //            mapPos.setDir(Side::TO_UP);
            //            break;
            //        case Side::TO_UP:
            //            mapPos.setDir(Side::TO_DOWN);
            //            break;
            //        case Side::TO_LEFT:
            //            mapPos.setDir(Side::TO_RIGHT);
            //            break;
            //        case Side::TO_RIGHT:
            //            mapPos.setDir(Side::TO_LEFT);
            //            break;
            //        default:
            //            break;
            //        }
            //        //Put the correct texture to the NPC
            //        currentTexture = otherTextures.begin() + (int)mapPos.getDir();
            //        updateTexture();
            //        mapPos.unlockMove();
            //        talking = false;
            //        overworld.startDialog(this->dialog);
            //    }
            //}

            //void CharacterEvent::setPredefinedMove(std::vector<Side> moves) {
            //    this->movements = moves;
            //}

            //void LockedDoorEvent::action(Model::Player &player, View::Overworld &overworld) {
            //}

            //void LockedDoorEvent::update(Model::Player &player, View::Overworld &overworld) {
            //}

            //void TrainerEvent::action(Model::Player &player, View::Overworld &overworld) {
            //    TalkingCharaEvent::action(player, overworld);
            //}

            //void TrainerEvent::update(Model::Player &player, View::Overworld &overworld) {
            //    if(triggerBattle) {
            //        triggerBattle = false;
            //        overworld.declareBattle(this);
            //    }
            //    if(!defeated) {
            //        if(talking && !checkTalking) {
            //            checkTalking = true;
            //        }
            //        if(!talking && checkTalking) {
            //            triggerBattle = true;
            //        }
            //    }
            //    TalkingCharaEvent::update(player, overworld);
            //}

            //void TrainerEvent::defeat() {
            //    defeated = true;
            //    this->changeDialog(defeatedDialog);
            //}

            //Utils::OpString LockedDoorEvent::keysLock = Utils::OpString();

        } // namespace Events

    } // namespace Model
} // namespace OpMon
#pragma GCC diagnostic pop
