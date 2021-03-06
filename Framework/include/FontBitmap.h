#pragma once
#include "PointWrapSampler.h"
#include "Texture.h"
#include "DX11Forward.h"

#include <DirectXMath.h>
#include <string>

namespace Text {
class FontBitmap {
public:
    FontBitmap(
        ID3D11Device* device, ID3D11DeviceContext* context,
        const std::wstring& fontMap,
        const std::string& glyphs,
        int xCount, int yCount,
        int glyphPxWidth, int glyphPxHeight,
        int texturePxWidth, int texturePxHeight
    );

    void use(ID3D11DeviceContext* context) const;

    float getWidthSizeScale() const;
    float getHeightSizeScale() const;
    float getFontAspectRatio() const;

    DirectX::XMFLOAT4 getUVWH(char c) const;

private:
    Texture fontMap_;
    PointWrapSampler sampler_;
    std::string glyphs_;
    int xCount_;
    int yCount_;
    int glyphPxWidth_;
    int glyphPxHeight_;
    int texturePxWidth_;
    int texturePxHeight_;
    float sizeScale_;
};


// ======================
// Font factory functions
// ======================
FontBitmap makeInconsolata(ID3D11Device* device, ID3D11DeviceContext* context);
}
