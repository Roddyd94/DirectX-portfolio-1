#pragma once

using ActorID         = int32;
using ComponentID     = int32;
using ComponentIDPair = std::pair<ActorID, ComponentID>;

namespace Direction
{
    enum Type
    {
        North,
        Top = North,
        Northeast,
        RightTop = Northeast,
        East,
        Right = East,
        Southeast,
        RightBottom = Southeast,
        South,
        Bottom = South,
        Southwest,
        LeftBottom = Southwest,
        West,
        Left = West,
        NorthWest,
        LeftTop = NorthWest,
        End
    };
}
