#include "pch.h"

#include "IndexedAnimation2D.h"

#include "Core/ResourceManager.h"

#include "IndexedAnimation2DConstantBuffer.h"
#include "Core/Texture.h"

bool IndexedAnimation2D::Init()
{
    _constantBuffer = FIND_CONSTANT_BUFFER("IndexedAnimation2D", IndexedAnimation2DConstantBuffer);

    return true;
}

void IndexedAnimation2D::BindConstantBuffer(Animation2DSprite& sprite)
{
    Ptr<IndexedTextureAnimation2DSpriteSheet> spriteSheet
      = Cast<Animation2DSpriteSheet, IndexedTextureAnimation2DSpriteSheet>(sprite.spriteSheet);

    SpriteData          data    = spriteSheet->GetSpriteData(sprite.spriteIndex);
    Ptr<IndexedTexture> texture = spriteSheet->GetTexture();
    texture->SetShaderResource(0, ShaderType::Pixel, 0);

    Vector2 uvLT;
    Vector2 uvRB;

    uvLT.x = data.start.x / texture->GetWidth();
    uvLT.y = data.start.y / texture->GetHeight();

    uvRB.x = uvLT.x + data.size.x / texture->GetWidth();
    uvRB.y = uvLT.y + data.size.y / texture->GetHeight();

    SetConstantBuffer<IndexedAnimation2DConstantBuffer>(uvLT, uvRB, _paletteNumber, _flipX);
    _constantBuffer->Update();
}

void IndexedAnimation2D::SetPaletteNumber(int32 paletteNumber)
{
    _paletteNumber = paletteNumber;
}
