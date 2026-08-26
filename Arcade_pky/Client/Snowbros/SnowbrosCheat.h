#pragma once
#include "Core/Actor.h"
#include "Core/Input/Types.h"

class SnowbrosCheat : public Actor
{
public:
    SnowbrosCheat()           = default;
    ~SnowbrosCheat() override = default;

protected:
    std::vector<Weak<class SnowbrosEnemy>> _stressTestEntries;
    std::vector<std::pair<float, float>>   _stressTestPositions;
    int32                                  _stressTestEntryIndex    = 0;
    int32                                  _stressTestPositionIndex = 0;

public:
    bool Init(int32 id, Vector3 position, Vector3 scale, Vector3 rotation) override;
    void SpawnMonster(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void SpawnMonsterStress(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void ChangePalette(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void ChangeLevel(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void ToggleCamera(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void StartPlayer2(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void MakeSnowball(Ptr<class InputAction> action, ButtonEventType::Type buttonEvent);
    void Reset();
};
