#pragma once
#include "BaseExample.h"
#include "ShaderProgram.h"
#include "AnisotropicSampler.h"
#include "Quad.h"
#include "Model.h"
#include "PointWrapSampler.h"
#include "Transform.h"
#include "Text.h"

namespace Deferred {

struct GShaderCB {
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
    DirectX::XMMATRIX NormalMatrix;
};

struct UnlitCB {
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
    DirectX::XMMATRIX NormalMatrix;
};

#define NUM_LIGHTS 32

struct DeferredLightCB {
    PointLight Lights[NUM_LIGHTS];
    DirectX::XMFLOAT4 ViewPos;
};

class DeferredRenderingExample : public BaseExample {
protected:
    ID3D11Texture2D * depthBuffer_ = nullptr;
    ID3D11DepthStencilView* depthBufferDepthView_ = nullptr;
    ID3D11ShaderResourceView* depthBufferResourceView_ = nullptr;

    // Geometry buffers
    ID3D11Texture2D * gPosition_ = nullptr;
    ID3D11Texture2D* gNormal_ = nullptr;
    ID3D11Texture2D* gAlbedo_ = nullptr;
    ID3D11RenderTargetView* gPositionView_ = nullptr;
    ID3D11RenderTargetView* gNormalView_ = nullptr;
    ID3D11RenderTargetView* gAlbedoView_ = nullptr;
    ID3D11ShaderResourceView* gPositionRV_ = nullptr;
    ID3D11ShaderResourceView* gNormalRV_ = nullptr;
    ID3D11ShaderResourceView* gAlbedoRV_ = nullptr;

    // Shaders
    using GShader = ShaderProgram<GShaderCB>;
    using UnlitShader = ShaderProgram<UnlitCB>;
    using PGShader = std::unique_ptr<GShader>;
    using DefferedShader = ShaderProgram<DeferredLightCB>;
    using PDeferredShader = std::unique_ptr<DefferedShader>;

    PGShader gShader_;
    PDeferredShader defferedLightShader_;

    std::unique_ptr<Models::Model> model_;
    std::unique_ptr<Quad> quad_;

    Samplers::PAnisotropicSampler anisoSampler_;
    std::unique_ptr<PointWrapSampler> pointSampler_;

    std::array<PointLight, NUM_LIGHTS> lights_;
    std::vector<Transform> modelTransforms_;

    std::unique_ptr<Text::Text> frameTimeText_;

    HRESULT setup() override;    
    void render() override;
};
}