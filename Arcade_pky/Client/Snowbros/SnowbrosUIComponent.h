#pragma once
#include "Core/Animation/Animation2DSprite.h"
#include "Core/SceneComponent.h"

struct ScoreUI
{
    Animation2DSprite spriteData;

    float position      = 0.f;
    float size          = 1.f;
    int32 paletteNumber = 0;
    bool  render        = true;
};

namespace ScoreType
{
    enum Type
    {
        Player1,
        Player2,
        HighScore,
        End
    };
};

struct ScoreText
{
    int32           score  = 0;
    ScoreType::Type player = ScoreType::End;
};

class SnowbrosUIComponent : public SceneComponent
{
public:
    SnowbrosUIComponent();
    ~SnowbrosUIComponent() override = default;

protected:
    std::vector<Animation2DSprite>           _numberSpriteData;
    std::unordered_map<std::string, ScoreUI> _scoreUIs;

    ScoreText _scoreTexts[ScoreType::End] = {};

    Ptr<class Mesh>            _uiMesh;
    Ptr<class StructureBuffer> _uiStructureBuffer;
    Ptr<class Shader>          _uiInstanceShader;
    Ptr<class StructureBuffer> _paletteBuffer = nullptr;
    Ptr<class Texture>  _texture       = nullptr;

    int32 _numberPaletteID         = 0;
    bool  _shouldRefreshUIInstance = false;

public:
    bool Init(int32 componentID, const std::string& name, Ptr<class Actor> owner) override;
    void Destroy() override;
    void Tick(float deltaTime) override;

    void Render(float deltaTime) override;

    void RefreshInstance(bool refresh);
    void SwitchUI(const std::string& name, bool shouldRender);

    void SetScore(ScoreType::Type player, int32 score);

private:
    void AddScoreDataToBuffer(const ScoreText& scoreText);
    void AddBufferData(
      Vector2 position, Vector2 size, Animation2DSprite sprite, int32 paletteNumber);
};
