#pragma once

namespace PlatformerKinematicState
{
    enum Type : int8
    {
        OnGround,
        OnAir,
        OnSlope,
        OnLadder,
        Attached,
        OnStandable,
        End
    };
}
