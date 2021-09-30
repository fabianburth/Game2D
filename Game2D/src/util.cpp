//
// Created by burth on 14.09.2021.
//

#include "util.h"


auto stringDirection(Direction direction) -> std::string {
    switch (direction) {
        case Direction::RIGHT:
            return "Right";
        case Direction::LEFT:
            return "Left";
        case Direction::UP:
            return "Up";
        case Direction::DOWN:
            return "Down";
        case Direction::NONE:
            return "None";
        default:
            return "";
    }
}

auto oppositeDirection(Direction direction) -> Direction {
    switch (direction) {
        case Direction::RIGHT:
            return Direction::LEFT;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::NONE:
        default:
            return Direction::NONE;
    }
}

auto stringState(PengoState state) -> std::string {
    switch (state) {
        case PengoState::STAND:
            return "Stand";
        case PengoState::WALK:
            return "Walk";
        case PengoState::PUSH:
            return "Push";
        case PengoState::BREAK:
            return "Break";
        default:
            return "";
    }
}