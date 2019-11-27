/* Header for TPEvent Class

Class describes the Teleport Event.
							This is a specific type of event that allows for the teleportation of an entity from point to point
							within a map or across different maps. */

#pragma once
#include "src/opmon/model/sysObjects/Events.hpp"

class Position;

namespace Events {

    class TPEvent : public OpMon::Model::Event {

      protected:
        sf::Vector2i m_tpCoord; // map coord to teleport to
        std::string m_map;      // map teleporting to
                                //Side ppDir;

      public:
        TPEvent(const Position &_mapPosition, const OpMon::Model::Events::EventTrigger &_eventTrigger, sf::Vector2i const &_tpPos, const std::string &_map,
                int _sides = SIDE_ALL, bool _passable = true);
        virtual void update(OpMon::Model::Player &player, OpMon::View::Overworld &overworld);
        virtual void action(OpMon::Model::Player &player, OpMon::View::Overworld &overworld);
    };

} // namespace Events