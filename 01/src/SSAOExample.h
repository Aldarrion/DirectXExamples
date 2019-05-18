#pragma once
#include "BaseExample.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "Quad.h"
#include "PointWrapSampler.h"
#include "Transform.h"
#include "DeferredRenderingExample.h"
#include <random>

namespace SSAO {

struct GShaderCB {
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX View;
    DirectX::XMMATRIX Projection;
    DirectX::XMMATRIX NormalMatrix;
};

struct SSAOCB {
    DirectX::XMMATRIX Projection;
    DirectX::XMFLOAT4 Kernel[64];
    DirectX::XMFLOAT4 ScreenResolution;
};

struct SSAOLightCB {
    DirectX::XMFLOAT4 LightPos;
    DirectX::XMFLOAT4 LightCol;
    DirectX::XMFLOAT4 IsSSAOOn;
};


class SSAOExample : public BaseExample {
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

    // SSAO buffers
    ID3D11Texture2D* ssaoBuffer_ = nullptr;
    ID3D11Texture2D* ssaoBlurBuffer_ = nullptr;
    ID3D11RenderTargetView* ssaoRTView_ = nullptr;
    ID3D11RenderTargetView* ssaoBlurRTView_ = nullptr;
    ID3D11ShaderResourceView* ssaoRV_ = nullptr;
    ID3D11ShaderResourceView* ssaoBlurRV_ = nullptr;
    
    ID3D11Texture2D* noiseBuffer_ = nullptr;
    ID3D11ShaderResourceView* noiseRV_ = nullptr;

    // Shaders
    using GShader = ShaderProgram<GShaderCB>;
    using PGShader = std::unique_ptr<GShader>;
    using SSAOShader = ShaderProgram<SSAOCB>;
    using GBufferDisplayShader = ShaderProgram<Deferred::GBufferDisplayCB>;
    using PGBufferDisplayShader = std::unique_ptr<GBufferDisplayShader>;
    using PSSAOShader = std::unique_ptr<SSAOShader>;
    using SSAOBlurShader = ShaderProgram<>;
    using PSSAOBlurShader = std::unique_ptr<SSAOBlurShader>;
    using SSAOLightShader = ShaderProgram<SSAOLightCB>;
    using PSSAOLightShader = std::unique_ptr<SSAOLightShader>;

    PGShader gShader_;
    PGBufferDisplayShader gBufferDisplayShader_;
    PSSAOShader ssaoShader_;
    PSSAOBlurShader ssaoBlurShader_;
    PSSAOLightShader ssaoLightShader_;

    std::unique_ptr<Models::Model> model_;
    std::unique_ptr<Quad> quad_;

    std::unique_ptr<PointWrapSampler> pointSampler_;

    Transform modelTransform_;

    std::vector<DirectX::XMFLOAT4> ssaoKernel_;
    std::uniform_real_distribution<float> randomFloats_; // generates random floats between 0.0 and 1.0
    std::default_random_engine generator_;

    WinKeyMap::WinKeyMap toggleSSAOKey_ = WinKeyMap::E;
    std::unique_ptr<Text::Text> infoText_;

    bool isSSAOOn_ = true;

    HRESULT setup() override;
    std::vector<DirectX::XMFLOAT3> generateNoise();
    std::vector<DirectX::XMFLOAT4> generateKernel();
    void handleInput() override;
    void updateInfoText() const;
    void drawGBufferDisplays() const;
    void render() override;
};

}