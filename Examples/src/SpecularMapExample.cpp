#include "SpecularMapExample.h"
#include "Layouts.h"
#include "Transform.h"

namespace Specular {
using namespace DirectX;

HRESULT SpecularMapExample::setup() {
    auto hr = BaseExample::setup();
    if (FAILED(hr))
        return hr;

    hr = reloadShaders();
    if (FAILED(hr))
        return hr;

    seaFloorTexture_ = Textures::createSeaFloorTexture(context_);
    boxDiffuse_ = Textures::createBoxDiffuse(context_);
    boxSpecular_ = Textures::createBoxSpecular(context_);
    sampler_ = Samplers::createAnisoSampler(context_);
    cube_ = std::make_unique<TexturedCube>(context_.d3dDevice_);

    camera_.positionCamera(
        XMFLOAT3(7.50966454f, -4.39316130f, -6.65817976f),
        XMFLOAT3(0.0f, 0.1f, 0.0f),
        -46.8005867f,
        -25.4000607f
    );

    return S_OK;
}

bool SpecularMapExample::reloadShadersInternal() {
    return
        Shaders::makeShader<SpecularShader>(shader_, context_.d3dDevice_, "shaders/Specular.fx", "VS", "shaders/Specular.fx", "PS", Layouts::TEXTURED_LAYOUT)
        && Shaders::makeSolidShader(solidShader_, context_);
}

void SpecularMapExample::render() {
    BaseExample::render();

    const Transform sunTrans = Transform(XMFLOAT3(-10.0f, 10.0f, -10.0f), XMFLOAT3(0, 0, 0), XMFLOAT3(0.2f, 0.2f, 0.2f));
    const XMFLOAT4 lightColor = XMFLOAT4(1, 1, 1, 1);

    const std::vector<Transform> cubeTransforms = {
        Transform(XMFLOAT3(0, 0, 0)),
        Transform(XMFLOAT3(2.5f, 0, 0)),
        Transform(XMFLOAT3(5.f, 0, 0)),
        Transform(XMFLOAT3(-2.5f, 0, 0))
    };

    clearViews();

    // Draw cubes
    {
        SpecularCB cb;
        cb.Projection = XMMatrixTranspose(projection_);
        cb.View = XMMatrixTranspose(camera_.getViewMatrix());
        cb.NormalMatrix = computeNormalMatrix(cb.World);
        cb.SunLight.Color = lightColor;
        cb.SunLight.Direction = XMFLOAT4(-sunTrans.Position.x, -sunTrans.Position.y, -sunTrans.Position.z, 1.0f);
        cb.ViewPos = camera_.Position;

        shader_->use(context_.immediateContext_);
        boxDiffuse_->use(context_.immediateContext_, 0);
        boxSpecular_->use(context_.immediateContext_, 1);
        sampler_->use(context_.immediateContext_, 0);

        for (size_t i = 0; i < cubeTransforms.size(); ++i) {
            cb.UseSpecular = i & 1;
            cb.World = XMMatrixTranspose(cubeTransforms[i].generateModelMatrix());
            cb.NormalMatrix = computeNormalMatrix(cb.World);
            shader_->updateConstantBuffer(context_.immediateContext_, cb);
            cube_->draw(context_);
        }
    }

    // Draw sun
    {
        ConstantBuffers::SolidConstBuffer cb;
        cb.World = XMMatrixTranspose(sunTrans.generateModelMatrix());
        cb.View = XMMatrixTranspose(camera_.getViewMatrix());
        cb.Projection = XMMatrixTranspose(projection_);
        cb.OutputColor = lightColor;

        solidShader_->use(context_.immediateContext_);
        solidShader_->updateConstantBuffer(context_.immediateContext_, cb);
        cube_->draw(context_);
    }

    context_.swapChain_->Present(0, 0);
}
}
