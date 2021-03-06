#pragma once

#include "VertexTypes.h"
#include "DrawableObject.h"

#include <DirectXMath.h>

class TexturedCube : public DrawableObject<VertexTypes::FullVertex> {
public:
    explicit TexturedCube(ID3D11Device* device) {
        
        const std::vector<VertexTypes::FullVertex> vertices = {
            { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),     DirectX::XMFLOAT3(0.2f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),      DirectX::XMFLOAT3(0.2f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),       DirectX::XMFLOAT3(0.2f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),      DirectX::XMFLOAT3(1.0f, 0.2f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
                                                                                                                          
            { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),   DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),    DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),     DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
            { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),    DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
                                                                                                                             
            { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),    DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),   DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
            { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),    DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),     DirectX::XMFLOAT3(1.0f, 0.2f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
                                                                                                                               
            { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),      DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),     DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),      DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),       DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
                                                                                                                               
            { DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),   DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),    DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f), DirectX::XMFLOAT2(1.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),     DirectX::XMFLOAT3(0.5f, 0.7f, 0.5f), DirectX::XMFLOAT2(1.0f, 0.0f) },
            { DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),    DirectX::XMFLOAT3(0.1f, 0.7f, 0.5f), DirectX::XMFLOAT2(0.0f, 0.0f) },
                                                                                                                               
            { DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),     DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),      DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
            { DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),       DirectX::XMFLOAT3(0.1f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
            { DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),      DirectX::XMFLOAT3(1.0f, 0.2f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
        };

        const std::vector<WORD> indices = {
            3,1,0,
            2,1,3,

            6,4,5,
            7,4,6,

            11,9,8,
            10,9,11,

            14,12,13,
            15,12,14,

            19,17,16,
            18,17,19,

            22,20,21,
            23,20,22
        };
        initialize(device, vertices, indices);
    }

    Layouts::VertexLayout_t getVertexLayout() const override {
        return Layouts::TEXTURED_LAYOUT;
    }

    const wchar_t* getObjectName() const override {
        return L"Textured Cube";
    }
};

