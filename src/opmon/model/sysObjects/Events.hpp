/*
  Events.hpp
  Author : Cyrielle
  Contributors : BAKFR, Navet56
  File under GNU GPL v3.0 license.
*/
#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

#include "../../../utils/OpString.hpp"
#include "../../../utils/defines.hpp"
#include "../../start/i18n/ATranslatable.hpp"
#include "../objects/Enums.hpp"
#include "Player.hpp"
#include "Position.hpp"

//Macros defining constants to know the side from where the events can be triggered.
//Inspired by SDL's system to initialize the different modules

#define SIDE_UP 0x0001
#define SIDE_DOWN 0x0002
#define SIDE_LEFT 0x0010
#define SIDE_RIGHT 0x0020
#define SIDE_ALL (SIDE_UP | SIDE_DOWN | SIDE_RIGHT | SIDE_LEFT)

#pragma GCC diagnostic ignored "-Wreorder"

namespace OpMon {

    namespace View {
        class Overworld;
    }

    namespace Model {

        namespace Events {
            //Used to know when the event must be triggered, relatively to the player
            enum class EventTrigger {
                PRESS = 0,
                GO_IN = 1,
                ZONE = 2,
                BE_IN = 3
            };
        } // namespace Events

        /* OpMon::View::Event Describes an abstract class for creating many types of events.
					Other simple events can inherit from this event interface and then be combined to form complex events.
					Ex: TPEvent + SoundEvent + AnimationEvent = DoorEvent; In other words a "DoorEvent" shouldn't inherit from
					all of these specific events but instead should be composed of them. Motivation: To defeat diamond inheritance*/
        class Event {
          protected: // ctor
            Event(const Position& _mapPos, const Events::EventTrigger& _eventTrigger, int _sides = SIDE_ALL, bool _passable = false)
              : m_mapPos{_mapPos}
              , m_eventTrigger{_eventTrigger}
              , m_sides{_sides}
              , m_passable{_passable} {}						     

          public: // public methods
            Event() = delete;
            Event(const Event &) = delete;
            /* Allow destruction of inherited classes*/
            virtual ~Event() = default;

            /**This method is called at each frame*/
            virtual void update(Model::Player &player, View::Overworld &overworld) = 0;
            /**This method is called when the player interacts with the event*/
            virtual void action(Model::Player &player, View::Overworld &overworld) = 0;

            inline int getSide() const { return m_sides; }            
            inline Events::EventTrigger getEventTrigger() const { return m_eventTrigger; }
            inline const Position& getPositionMap() const { return m_mapPos; }
            inline bool isPassable() const { return m_passable; }

          protected: // data members
            Position m_mapPos;
            Events::EventTrigger m_eventTrigger;
            bool m_passable;
            int m_sides;       
        };



				namespace Events { // TPEvent
					/* TPEvent is a specific type of Event in which the player or any other entity is teleported from one position on the map
					to another position on the same map or a different map entirely. */
            class TPEvent : public Event {
            public:
                TPEvent(const EventTrigger &_eventTrigger, const sf::Vector2f &_mapPosition, sf::Vector2i const &_tpPos, const std::string &_map,
                      Side _ppDir = Side::NO_MOVE, int _sides = SIDE_ALL, bool _passable = true);
								TPEvent(const TPEvent &_other);
              virtual void update(Player &player, View::Overworld &overworld);
              virtual void action(Player &player, View::Overworld &overworld);

              protected:
                sf::Vector2i m_tpCoord; // map coord to teleport to
                std::string m_map;      // map teleporting to              
            };
        } // namespace Events -- TPEvent

				namespace Events { // SoundEvent
          /* SoundEvent is a specific version of an event that when triggered, plays a sound */
            class SoundEvent : public Event {
              public:
                SoundEvent(const Position &_mapPosition, const EventTrigger &_eventTrigger, const sf::Sound& _sound, const std::string &_map,
                           int _sides = SIDE_ALL, bool _passable = true);
                //TODO: Needs copy constructor!
                virtual void update(Player &_player, View::Overworld &_overworld);
                virtual void action(Player &_player, View::Overworld &_overworld);
              protected:
                sf::Sound m_sound; // TODO: I think this needs to be a pointer. Actually I dont think so. The sf::SoundBuffer just needs to exist
                std::string m_map;
						};
				} // namespace events --SoundEvent

				namespace Events { // DoorEvent
               /* DoorEvent is a macro event composed of micro events.
									TPEvent + SoundEvent + TODO: AnimationEvent = DoorEvent
									DoorEvent can be safely derived to use different types of door and sound animations*/
            class DoorEvent{
              public:
                DoorEvent(const TPEvent& _tpEvent, const SoundEvent& _soundEvent);
                virtual void update(Player &_player, View::Overworld &_overworld);
                virtual void action(Player &_player, View::Overworld &_overworld);

              private:
                TPEvent m_tpEvent;
                SoundEvent m_soundEvent;
            };
        } // namespace events --SoundEvent
				





//  /*
//	   Contains stuff in relation with the events, mostly different types of events.
//	*/
//        namespace Events {						
//
//            class DoorEvent : public Event {
//              protected:
//                std::string doorType;
//                int animStarted = -1;
//
//              public:
//                /* Types of door (to put in doorType (TODO)) : "door" | "shop door"*/
//                DoorEvent(std::vector<sf::Texture> &doorTextures, std::string doorType, sf::Vector2f const &position, sf::Vector2i const &tpPos, std::string const &map, EventTrigger eventTrigger = EventTrigger::GO_IN, Side ppDir = Side::NO_MOVE, int sides = SIDE_ALL, bool passable = true);
//                virtual void action(Model::Player &player, View::Overworld &overworld);
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//            };
//
//            class TalkingEvent : public virtual Event, I18n::ATranslatable {
//              private:
//                Utils::OpString dialogKey;
//
//              protected:
//                sf::String dialog;
//
//              public:
//                TalkingEvent(std::vector<sf::Texture> &otherTextures, sf::Vector2f const &position, Utils::OpString const &dialogKey, int sides = SIDE_ALL, EventTrigger eventTrigger = EventTrigger::PRESS, bool passable = false);
//                void onLangChanged() override;
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//                virtual void action(Model::Player &player, View::Overworld &overworld);
//                virtual void changeDialog(Utils::OpString newDialog);
//            };
//
//            class LockedDoorEvent : public DoorEvent, TalkingEvent {
//              protected:
//                Item *needed;
//                bool consumeItem;
//                static Utils::OpString keysLock;
//
//              public:
//                virtual void action(Model::Player &player, View::Overworld &overworld);
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//                LockedDoorEvent(std::vector<sf::Texture> &doorTextures, std::string doorType, Item *needed, sf::Vector2f const &position, sf::Vector2i const &tpPos, std::string const &map, Side ppDir = Side::NO_MOVE, EventTrigger eventTrigger = EventTrigger::PRESS, bool consumeItem = false, int sides = SIDE_ALL, bool passable = false);
//            };
//
//            //Ways to move for the npcs
//            enum class MoveStyle : int {
//                NO_MOVE = 0,
//                PREDEFINED = 1,
//                RANDOM = 2,
//                FOLLOWING = 3
//            };
//
//            class CharacterEvent : public virtual Event {
//              protected:
//                MoveStyle moveStyle;
//
//                unsigned int predefinedCounter = 0;
//                int animsCounter = 0;
//
//                std::vector<Side> movements;
//
//                int startFrames = 0;
//                bool anims = false;
//                int frames = 0;
//
//              public:
//                CharacterEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position, Side posDir = Side::TO_UP, MoveStyle moveStyle = MoveStyle::NO_MOVE, EventTrigger eventTrigger = EventTrigger::PRESS, std::vector<Side> predefinedPath = std::vector<Side>(), bool passable = false, int sides = SIDE_ALL);
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//                virtual void action(Model::Player &, View::Overworld &){};
//                void setPredefinedMove(std::vector<Side> movement);
//                OP_DEPRECATED void move(Side direction, Model::Player &player, View::Overworld &overworld);
//                bool move(Side direction, Map *map);
//            };
//
//            class TalkingCharaEvent : public CharacterEvent, TalkingEvent {
//              protected:
//                bool talking = false;
//
//              public:
//                TalkingCharaEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position, Utils::OpString const &dialogKey, Side posDir = Side::TO_UP, EventTrigger eventTrigger = EventTrigger::PRESS, MoveStyle moveStyle = MoveStyle::NO_MOVE, std::vector<Side> predefinedPath = std::vector<Side>(), bool passable = false, int side = SIDE_ALL);
//
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//                virtual void action(Model::Player &player, View::Overworld &overworld);
//                virtual void changeDialog(Utils::OpString newDialog) { TalkingEvent::changeDialog(newDialog); }
//            };
//
//            class TrainerEvent : public TalkingCharaEvent {
//              private:
//                OpTeam *team;
//                bool defeated = false;
//                bool triggerBattle = false;
//                bool checkTalking = false;
//                Utils::OpString defeatedDialog;
//
//              public:
//                TrainerEvent(std::vector<sf::Texture> &textures, sf::Vector2f const &position, OpTeam *team, Utils::OpString const &dialogKeys, Utils::OpString const &defeatedDialog, Side posDir = Side::TO_UP, EventTrigger eventTrigger = EventTrigger::PRESS, MoveStyle moveStyle = MoveStyle::NO_MOVE, std::vector<Side> predefinedPath = std::vector<Side>(), bool passable = false, int side = SIDE_ALL);
//
//                virtual void update(Model::Player &player, View::Overworld &overworld);
//                virtual void action(Model::Player &player, View::Overworld &overworld);
//
//                OpTeam *getOpTeam() {
//                    return team;
//                }
//
//                void defeat();
//
//                bool isDefeated() {
//                    return defeated;
//                }
//
//                virtual ~TrainerEvent();
//            };
//
//            class TrainerEyesightEvent : public Event {
//              public:
//                TrainerEvent *trainer;
//                bool checkTalking = false;
//                bool triggerBattle = false;
//
//              public:
//                TrainerEyesightEvent(TrainerEvent *trainer, sf::Vector2f position);
//#pragma GCC diagnostic ignored "-Wunused-parameter"
//                virtual void update(Model::Player &player, View::Overworld &overworld){};
//                virtual void action(Model::Player &player, View::Overworld &overworld){};
//#pragma GCC diagnostic pop
//            };

    } // namespace Model
} // namespace OpMon
#endif // EVENTS_HPP
