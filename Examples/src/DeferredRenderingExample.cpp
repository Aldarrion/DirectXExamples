#include "DeferredRenderingExample.h"
#include "WinKeyMap.h"
#include <algorithm>
#include <directxcolors.h>

namespace Deferred {

using namespace DirectX;

/*
 * All ligths and objects in this example are considered dynamic. For static objects and lights we could bake the lighting.
 */


ContextSettings DeferredRenderingExample::getSettings() const {
    ContextSettings settings = BaseExample::getSettings();
    
    // Disable multisampling in deferred rendering, it makes no sense there
    // and we would have to take care to always use the same multisampling 
    // settings for render/depth buffers which are used together
    settings.MultisampleCount = 1;

    // This example is annotated for easier viewing in frame debuggers such as RenderDoc
    settings.UseCustomGPUAnnotations = true;

    return settings;
}

HRESULT DeferredRenderingExample::setup() {
    BaseExample::setup();

    D3D11_TEXTURE2D_DESC texDesc;
    ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
    texDesc.Width = context_.WIDTH;
    texDesc.Height = context_.HEIGHT;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    // We will look at this texture with 2 different views -> typeless
    texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;

    ID3D11Texture2D* depthBuffer = nullptr;
    auto hr = context_.d3dDevice_->CreateTexture2D(&texDesc, nullptr, &depthBuffer);
    if (FAILED(hr))
        return hr;

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    // Put all precision to the depth
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    hr = context_.d3dDevice_->CreateDepthStencilView(depthBuffer, &descDSV, depthBufferDSV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    // In shader sample it like regular texture with single red channel
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;

    hr = context_.d3dDevice_->CreateShaderResourceView(depthBuffer, &srvDesc, depthBufferSRV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    depthBuffer->Release();


    // ===================
    // Initialize G-buffer
    // ===================
    D3D11_TEXTURE2D_DESC gBufferTextDesc;
    ZeroMemory(&gBufferTextDesc, sizeof(D3D11_TEXTURE2D_DESC));
    gBufferTextDesc.Width = context_.WIDTH;
    gBufferTextDesc.Height = context_.HEIGHT;
    gBufferTextDesc.MipLevels = 1;
    gBufferTextDesc.ArraySize = 1;
    gBufferTextDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    gBufferTextDesc.SampleDesc.Count = 1;
    gBufferTextDesc.SampleDesc.Quality = 0;
    gBufferTextDesc.Usage = D3D11_USAGE_DEFAULT;
    gBufferTextDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    gBufferTextDesc.CPUAccessFlags = 0;
    gBufferTextDesc.MiscFlags = 0;

    ID3D11Texture2D* gPosition = nullptr;
    ID3D11Texture2D* gNormal = nullptr;
    ID3D11Texture2D* gAlbedo = nullptr;

    hr = context_.d3dDevice_->CreateTexture2D(&gBufferTextDesc, nullptr, &gPosition);
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateTexture2D(&gBufferTextDesc, nullptr, &gNormal);
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateTexture2D(&gBufferTextDesc, nullptr, &gAlbedo);
    if (FAILED(hr))
        return hr;

    D3D11_RENDER_TARGET_VIEW_DESC gBufferDesc;
    ZeroMemory(&gBufferDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
    gBufferDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    gBufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    hr = context_.d3dDevice_->CreateRenderTargetView(gPosition, &gBufferDesc, gPositionRTV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateRenderTargetView(gNormal, &gBufferDesc, gNormalRTV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateRenderTargetView(gAlbedo, &gBufferDesc, gAlbedoRTV_.GetAddressOf());
    if (FAILED(hr))
        return hr;


    D3D11_SHADER_RESOURCE_VIEW_DESC gResourceView;
    ZeroMemory(&gResourceView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
    gResourceView.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    gResourceView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    gResourceView.Texture2D.MipLevels = texDesc.MipLevels;
    gResourceView.Texture2D.MostDetailedMip = 0;

    hr = context_.d3dDevice_->CreateShaderResourceView(gPosition, &gResourceView, gPositionSRV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateShaderResourceView(gNormal, &gResourceView, gNormalSRV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    hr = context_.d3dDevice_->CreateShaderResourceView(gAlbedo, &gResourceView, gAlbedoSRV_.GetAddressOf());
    if (FAILED(hr))
        return hr;

    gPosition->Release();
    gNormal->Release();
    gAlbedo->Release();

    // ====================
    // Other initialization
    // ====================
    Text::makeDefaultSDFFont(context_, font_);
    infoText_ = std::make_unique<Text::TextSDF>("Frame time: 0", &font_);

    std::string path = "models/nanosuit/nanosuit.obj";
    model_ = std::make_unique<Models::Model>(context_, path);
    colorCube_ = std::make_unique<ColorCube>(context_.d3dDevice_);

    hr = reloadShaders();
    if (FAILED(hr))
        return hr;

    anisoSampler_ = Samplers::createAnisoSampler(context_);
    pointSampler_ = std::make_unique<PointWrapSampler>(context_.d3dDevice_);

    quad_ = std::make_unique<Quad>(context_.d3dDevice_);

    srand(42);
    for (auto& light : lights_) {
        // calculate slightly random offsets
        const float xPos = static_cast<float>(((rand() % 100) / 100.0) * 32.0 - 16.0);
        const float yPos = static_cast<float>(((rand() % 100) / 100.0) * 32.0 - 10.0);
        const float zPos = static_cast<float>(((rand() % 100) / 100.0) * 32.0 - 16.0);
        light.Position = XMFLOAT4(xPos, yPos, zPos, 1.0f);
        // also calculate random color
        const float rColor = ((rand() % 100) / 800.0f);// + 0.5; // between 0.5 and 1.0
        const float gColor = ((rand() % 100) / 800.0f);// + 0.5; // between 0.5 and 1.0
        const float bColor = ((rand() % 100) / 800.0f);// + 0.5; // between 0.5 and 1.0
        light.Color = XMFLOAT4(rColor, gColor, bColor, 1.0f);
    }

    for (int x = -3; x < 3; x++) {
        for (int z = -3; z < 3; z++) {
            modelTransforms_.emplace_back(XMFLOAT3(x * 7.0f, 0.0f, z * 5.0f));
        }
    }

    camera_.positionCamera(
        XMFLOAT3(17.8f, 18.3f, -18.2f),
        XMFLOAT3(0.0f, 1.0f, 0.0f),
        -40.6000977f,
        17.0000191f
    );

    return S_OK;
}

bool DeferredRenderingExample::reloadShadersInternal() {
    return
        Shaders::makeShader<GShader>(gShader_, context_.d3dDevice_, "shaders/DeferredGBuffer.fx", "VS", "shaders/DeferredGBuffer.fx", "PS", Layouts::POS_NORM_UV_LAYOUT)
        && Shaders::makeShader<DefferedShader>(defferedLightShader_, context_.d3dDevice_, "shaders/DeferredShader.fx", "VS", "shaders/DeferredShader.fx", "PS", Layouts::POS_UV_LAYOUT)
        && Shaders::makeShader<ForwardShader>(forwardShader_, context_.d3dDevice_, "shaders/ForwardShader.fx", "VS", "shaders/ForwardShader.fx", "PS", Layouts::POS_NORM_UV_LAYOUT)
        && Shaders::makeSolidShader(lightShader_, context_)
        && Shaders::makeShader<Shaders::TexturedQuad>(gBufferDisplayShader_, context_.d3dDevice_, "shaders/TexturedQuad.fx", "VS", "shaders/TexturedQuad.fx", "PS", Layouts::POS_UV_LAYOUT);
}

void DeferredRenderingExample::handleInput() {
    BaseExample::handleInput();

    if (GetAsyncKeyState(switchRenderingModeKey_) & 1) {
        isDeferredRendering_ = !isDeferredRendering_;
    }
}

void DeferredRenderingExample::render() {
    BaseExample::render();

    if (isDeferredRendering_) {
        renderDeferred();
    } else {
        renderForward();
    }
}

void DeferredRenderingExample::drawText() const {
    using std::to_string;
    infoText_->setText(
        "\n " + to_string(switchRenderingModeKey_) + ": switch rendering mode."
        + "\n Frame time is a SMA, it takes a few sec to stabilize. Models: " + to_string(modelTransforms_.size()) + " Lights: " + to_string(NUM_LIGHTS)
        + "\n\n Frame time (ms): " + to_string(deltaTimeSMA_ * 1000)
        + "\n Is dffered rendering: " + to_string(isDeferredRendering_)
    );
    infoText_->draw(context_);
}

void DeferredRenderingExample::renderLights() const {
    beginEvent(L"Light cubes");
    const XMMATRIX lightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
    ConstantBuffers::SolidConstBuffer solidCb;
    solidCb.View = XMMatrixTranspose(camera_.getViewMatrix());
    solidCb.Projection = XMMatrixTranspose(projection_);
    lightShader_->use(context_.immediateContext_);
    for (const auto& light : lights_) {
        solidCb.OutputColor = light.Color;
        solidCb.World = XMMatrixTranspose(lightScale * XMMatrixTranslationFromVector(XMLoadFloat4(&light.Position)));
        lightShader_->updateConstantBuffer(context_.immediateContext_, solidCb);
        colorCube_->draw(context_);
    }
    endEvent();
}

void DeferredRenderingExample::drawGBufferDisplays() const {
    std::array<ID3D11ShaderResourceView*, 3> gBufferViews = {
        gPositionSRV_.Get(),
        gNormalSRV_.Get(),
        gAlbedoSRV_.Get()
    };
    
    const float mapDisplaySize = 0.2f;

    beginEvent(L"GBuffer displays");
    for (size_t i = 0; i < gBufferViews.size(); ++i) {
        Transform shadowMapDisplayTransform(
            XMFLOAT3(1 - mapDisplaySize, -1 + mapDisplaySize * (i * 2 + 1), 0),
            XMFLOAT3(0, 0, 0),
            XMFLOAT3(mapDisplaySize, mapDisplaySize, mapDisplaySize)
        );
        Shaders::TexturedQuadCB gbdcb;
        gbdcb.World = XMMatrixTranspose(shadowMapDisplayTransform.generateModelMatrix());

        gBufferDisplayShader_->use(context_.immediateContext_);
        gBufferDisplayShader_->updateConstantBuffer(context_.immediateContext_, gbdcb);
        context_.immediateContext_->PSSetShaderResources(0, 1, &gBufferViews[i]);
        pointSampler_->use(context_.immediateContext_, 0);
        quad_->draw(context_);
    }

    // Unbind from PS so the driver does not have to force it when binding textures as RTs
    static ID3D11ShaderResourceView* nullViews[] = { nullptr, nullptr, nullptr };
    context_.immediateContext_->PSSetShaderResources(0, 3, nullViews);
    endEvent();
}

void DeferredRenderingExample::renderDeferred() {
    // ===========================================
    // Render infromation to geometry buffer views
    // ===========================================
    std::array<ID3D11RenderTargetView*, 3> views = {
        gPositionRTV_.Get(),
        gNormalRTV_.Get(),
        gAlbedoRTV_.Get()
    };
    
    beginEvent(L"GBuffer pass");

    // Set multiple rendering targets
    context_.immediateContext_->OMSetRenderTargets(static_cast<UINT>(views.size()), views.data(), depthBufferDSV_.Get());
    for (auto& view : views) {
        context_.immediateContext_->ClearRenderTargetView(view, Util::srgbToLinear(DirectX::Colors::Black));
    }
    context_.immediateContext_->ClearDepthStencilView(depthBufferDSV_.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    GShaderCB gscb;
    gscb.View = XMMatrixTranspose(camera_.getViewMatrix());
    gscb.Projection = XMMatrixTranspose(projection_);

    gShader_->use(context_.immediateContext_);
    anisoSampler_->use(context_.immediateContext_, 0);
    for (const auto& transform : modelTransforms_) {
        gscb.World = XMMatrixTranspose(transform.generateModelMatrix());
        gscb.NormalMatrix = XMMatrixTranspose(computeNormalMatrix(gscb.World));
        gShader_->updateConstantBuffer(context_.immediateContext_, gscb);
        model_->draw(context_);
    }

    endEvent();

    beginEvent(L"Deferred Quad");
    // =================
    // Render final quad
    // =================
    context_.immediateContext_->OMSetRenderTargets(1, &context_.renderTargetView_, context_.depthStencilView_);
    context_.immediateContext_->ClearRenderTargetView(context_.renderTargetView_, Colors::Black);
    context_.immediateContext_->ClearDepthStencilView(context_.depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

    DeferredLightCB lightCb;
    std::copy(lights_.begin(), lights_.end(), lightCb.Lights);
    lightCb.ViewPos = XMFLOAT4(camera_.Position.x, camera_.Position.y, camera_.Position.z, 1.0f);
    
    // Set geometry buffer data as input to deferred lighting pass
    context_.immediateContext_->PSSetShaderResources(0, 1, gPositionSRV_.GetAddressOf());
    context_.immediateContext_->PSSetShaderResources(1, 1, gNormalSRV_.GetAddressOf());
    context_.immediateContext_->PSSetShaderResources(2, 1, gAlbedoSRV_.GetAddressOf());

    defferedLightShader_->updateConstantBuffer(context_.immediateContext_, lightCb);
    defferedLightShader_->use(context_.immediateContext_);
    anisoSampler_->use(context_.immediateContext_, 0);
    pointSampler_->use(context_.immediateContext_, 1);

    quad_->draw(context_);
    
    endEvent();

    // ====================================================
    // Forward pass to render the cubes representing lights
    // ====================================================
    // Set depth information from the geometry pass 
    context_.immediateContext_->OMSetRenderTargets(1, &context_.renderTargetView_, depthBufferDSV_.Get());
    renderLights();

    // Render other billboards 
    drawGBufferDisplays();
    drawText();

    context_.swapChain_->Present(0, 0);
}

void DeferredRenderingExample::renderForward() {
    beginEvent(L"Forward pass");
    context_.immediateContext_->OMSetRenderTargets(1, &context_.renderTargetView_, context_.depthStencilView_);
    context_.immediateContext_->ClearRenderTargetView(context_.renderTargetView_, Colors::Black);
    context_.immediateContext_->ClearDepthStencilView(context_.depthStencilView_, D3D11_CLEAR_DEPTH, 1.0f, 0);

    anisoSampler_->use(context_.immediateContext_, 0);
    forwardShader_->use(context_.immediateContext_);

    ForwardCB cb;
    cb.View = XMMatrixTranspose(camera_.getViewMatrix());
    cb.Projection = XMMatrixTranspose(projection_);
    std::copy(lights_.begin(), lights_.end(), cb.Lights);
    cb.ViewPos = camera_.Position;
    for (const auto& transform : modelTransforms_) {
        cb.World = XMMatrixTranspose(transform.generateModelMatrix());
        cb.NormalMatrix = XMMatrixTranspose(computeNormalMatrix(cb.World));
        forwardShader_->updateConstantBuffer(context_.immediateContext_, cb);
        model_->draw(context_);
    }
    endEvent();

    renderLights();

    drawText();

    context_.swapChain_->Present(0, 0);
}
}
