#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "VertexTypes.h"
#include <vector>
#include "DrawableObject.h"

class Quad : public DrawableObject<VertexTypes::PosTexVertex> {
public:
    explicit Quad(ID3D11Device* device) {
        static constexpr float textureScale = 1.0f;
        const std::vector<VertexTypes::PosTexVertex> vertices = {
            { DirectX::XMFLOAT3(-1.0f, 1.0f, 0.5f),  DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, 0.5f),   DirectX::XMFLOAT2(textureScale, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, 0.5f),  DirectX::XMFLOAT2(textureScale, textureScale) },
            { DirectX::XMFLOAT3(-1.0f, -1.0f, 0.5f), DirectX::XMFLOAT2(0.0f, textureScale) }
        };

        const std::vector<WORD> indices = {
            0, 1, 3,
            2, 3, 1
        };

        initialize(device, vertices, indices);
    }
};
