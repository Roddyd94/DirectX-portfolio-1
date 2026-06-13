#include "pch.h"

#include "SnowbrosUIComponent.h"

#include "Core/ResourceManager.h"

#include "SnowbrosLevel.h"
#include "Core/Animation/Animation2DSprite.h"
#include "Core/Level.h"
#include "Core/Mesh.h"
#include "Core/Palette.h"
#include "Core/PaletteStructureBuffer.h"
#include "Core/Texture.h"
#include "Core/TransformConstantBuffer.h"
#include "Tilemap/TileInstanceShader.h"
#include "Tilemap/TileStructureBuffer.h"

SnowbrosUIComponent::SnowbrosUIComponent()
{
    _shouldRender = true;
}

bool SnowbrosUIComponent::Init(int32 componentID, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(componentID, name, owner);
    auto spriteSheet = FIND_SPRITESHEET("snowbros_player", TextureAnimation2DSpriteSheet);

    _uiMesh            = MESH_TEXTURE_RECT;
    _uiStructureBuffer = FIND_STRUCTURE_BUFFER("UI", TileStructureBuffer);
    _uiInstanceShader  = FIND_SHADER("TileInstanceShader", TileInstanceShader);

    _paletteBuffer = FIND_STRUCTURE_BUFFER("Palette", PaletteStructureBuffer);
    _texture       = FIND_TEXTURE("snowbros_player");

    _numberSpriteData.resize(10);

    _numberPaletteID = FIND_PALETTE("ui_font")->GetID();

    for (int i = 0; i < 10; ++i)
    {
        Animation2DSprite sprite;
        sprite.spriteSheet = spriteSheet;
        sprite.spriteIndex = 203 + i;

        _numberSpriteData[i] = sprite;
    }
    ScoreUI ui;
    ui.spriteData.spriteSheet = spriteSheet;
    ui.size                   = 1.f;
    ui.position               = 0.f;
    ui.spriteData.spriteIndex = 213;
    ui.paletteNumber          = FIND_PALETTE("player_1")->GetID();
    _scoreUIs["1P"]           = ui;
    ui.position               = 11.f;
    ui.spriteData.spriteIndex = 214;
    ui.paletteNumber          = FIND_PALETTE("player_2")->GetID();
    _scoreUIs["2P"]           = ui;
    ui.position               = 5.5f;
    ui.spriteData.spriteIndex = 215;
    ui.paletteNumber          = FIND_PALETTE("item_upgrade")->GetID();
    _scoreUIs["HI"]           = ui;
    ui.paletteNumber          = FIND_PALETTE("ui_font")->GetID();
    ui.spriteData.spriteIndex = 202;
    ui.size                   = 5.f;
    ui.position               = 1.f;
    _scoreUIs["Black1P"]      = ui;
    ui.position               = 12.f;
    _scoreUIs["Black2P"]      = ui;
    ui.position               = 6.5f;
    _scoreUIs["BlackHI"]      = ui;
    ui.paletteNumber          = FIND_PALETTE("player_1")->GetID();
    ui.spriteData.spriteIndex = 200;
    ui.position               = 1.f;
    _scoreUIs["InsertCoin1P"] = ui;
    ui.spriteData.spriteIndex = 201;
    _scoreUIs["PushButton1P"] = ui;
    ui.paletteNumber          = FIND_PALETTE("player_2")->GetID();
    ui.spriteData.spriteIndex = 200;
    ui.position               = 12.f;
    _scoreUIs["InsertCoin2P"] = ui;
    ui.spriteData.spriteIndex = 201;
    _scoreUIs["PushButton2P"] = ui;

    SwitchUI("InsertCoin1P", false);
    SwitchUI("PushButton1P", false);
    SwitchUI("PushButton2P", false);

    for (int i = 0; i < 3; ++i)
        _scoreTexts[i].player = static_cast<ScoreType::Type>(i);

    _shouldRefreshUIInstance = true;

    return true;
}

void SnowbrosUIComponent::Destroy()
{
    SceneComponent::Destroy();
}

void SnowbrosUIComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void SnowbrosUIComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    if (_shouldRefreshUIInstance)
    {
        _uiStructureBuffer->Clear();
        for (auto& [_, scoreUI] : _scoreUIs)
        {
            if (!scoreUI.render)
                continue;

            Vector2 position = GetWorldPosition().ToVector2();
            position.x += scoreUI.position + scoreUI.size / 2.f;
            position.y -= 0.5f;

            AddBufferData(position, {scoreUI.size, 1.f}, scoreUI.spriteData, scoreUI.paletteNumber);
        }

        for (auto& scoreText : _scoreTexts)
            AddScoreDataToBuffer(scoreText);

        _uiStructureBuffer->Update();
        RefreshInstance(false);
    }

    auto level = GetLevel();

    auto transformBuffer = Lock(_transformConstantBuffer);

    transformBuffer->SetWorldMatrix(Matrix());
    transformBuffer->SetViewMatrix(level->GetViewMatrix());
    transformBuffer->SetProjMatrix(level->GetProjMatrix());
    transformBuffer->Update();

    _uiStructureBuffer->Bind();
    _uiInstanceShader->SetShader();

    _texture->SetShaderResource(0, ShaderType::Pixel, 0);
    _paletteBuffer->Update();
    _paletteBuffer->Bind();

    _uiMesh->RenderInstancing(_uiStructureBuffer->GetElementCount());
}

void SnowbrosUIComponent::RefreshInstance(bool refresh)
{
    _shouldRefreshUIInstance = refresh;
}

void SnowbrosUIComponent::SwitchUI(const std::string& name, bool shouldRender)
{
    auto it = _scoreUIs.find(name);

    if (_scoreUIs.end() == it)
        return;

    it->second.render = shouldRender;
}

void SnowbrosUIComponent::SetScore(ScoreType::Type player, int32 score)
{
    _scoreTexts[player].score = score;
    _shouldRefreshUIInstance  = true;
}

void SnowbrosUIComponent::AddScoreDataToBuffer(const ScoreText& scoreText)
{
    float startPosition = 2.f;

    auto level = Cast<Level, SnowbrosLevel>(GetLevel());
    switch (scoreText.player)
    {
    case ScoreType::HighScore:
        startPosition = 7.5f;
        break;
    case ScoreType::Player1:
    {
        if (!level->GetPlayer(0))
            return;

        startPosition = 2.f;
    }
    break;
    case ScoreType::Player2:
    {
        if (!level->GetPlayer(1))
            return;

        startPosition = 13.f;
    }
    break;
    }

    bool  shouldRender = false;
    int32 radix        = 1'000'000;
    int32 score        = scoreText.score;
    float accPosition  = 0.f;

    while (radix > 0)
    {
        int32 digit = score / radix % 10;
        radix /= 10;

        if (0 != digit)
            shouldRender = true;

        if (!shouldRender)
        {
            accPosition += 0.5f;
            continue;
        }

        Vector2 position = GetWorldPosition().ToVector2();
        position.x += startPosition + accPosition;
        position.y -= 0.5f;

        AddBufferData(position, Vector2::one, _numberSpriteData[digit], _numberPaletteID);

        accPosition += 0.5f;
    }

    if (!shouldRender)
    {
        Vector2 position = GetWorldPosition().ToVector2();
        position.x += startPosition + 3.f;
        position.y -= 0.5f;

        AddBufferData(position, Vector2::one, _numberSpriteData[0], _numberPaletteID);
    }
}

void SnowbrosUIComponent::AddBufferData(
  Vector2 position, Vector2 size, Animation2DSprite sprite, int32 paletteNumber)
{
    auto structureBuffer = Cast<StructureBuffer, TileStructureBuffer>(_uiStructureBuffer);

    SpriteData spriteData = sprite.spriteSheet->GetSpriteData(sprite.spriteIndex);

    Vector2 textureSize;
    textureSize.x = static_cast<float>(_texture->GetWidth());
    textureSize.y = static_cast<float>(_texture->GetHeight());

    Vector2 uvLT, uvRB;
    uvLT.x = spriteData.start.x / textureSize.x;
    uvLT.y = spriteData.start.y / textureSize.y;
    uvRB.x = uvLT.x + spriteData.size.x / textureSize.x;
    uvRB.y = uvLT.y + spriteData.size.y / textureSize.y;

    structureBuffer->AddData(position, uvLT, uvRB, size);
}
