#include "pch.h"
#include "AppStateSimpleMSAA.h"

#include "ATGColors.h"
#include "FindMedia.h"
#include "ControllerFont.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

namespace
{
    unsigned int c_targetSampleCount = 4;
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleMSAA::AppStateSimpleMSAA(App *p_app)
    : AppState(p_app)
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::ctor()" << std::endl;
    //*/

}

//-----------------------------------------------------------------------------

AppStateSimpleMSAA::~AppStateSimpleMSAA()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::Init() {
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::Init()" << std::endl;
    //*/

	AppState::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::Reinit()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::Reinit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppState* AppStateSimpleMSAA::Update(
    DX::DeviceResources* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_gamePadConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_mouse;
    (void)p_mouseButtons;

    float elapsedSeconds    = float(p_timer.GetElapsedSeconds());
    float time              = float(p_timer.GetTotalSeconds());
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::Update(" << elapsedSeconds << ")" << std::endl;
	//*/
   
    m_world = Matrix::CreateRotationZ(cosf(time / 4.f));

    auto pad = p_gamePad->GetState(0);
    p_gamePadConnected = pad.IsConnected();
    if (p_gamePadConnected)
    {
        p_gamePadButtons.Update(pad);

        if (pad.IsViewPressed())
        {
            m_app->Quit(0);
        }

        if (p_gamePadButtons.a == GamePad::ButtonStateTracker::PRESSED)
        {
            m_msaa = !m_msaa;
        }
    }
    else
    {
        p_gamePadButtons.Reset();
    }

    auto kb = p_keyboard->GetState();
    p_keyboardButtons.Update(kb);

    if (kb.Escape)
    {
        m_app->Quit(0);
    }

    if (p_keyboardButtons.IsKeyPressed(Keyboard::Keys::Space))
    {
        m_msaa = !m_msaa;
    }

    return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::Draw(DX::DeviceResources* p_deviceResources)
{
    Clear(p_deviceResources);

    p_deviceResources->PIXBeginEvent(L"Render");
    auto context = p_deviceResources->GetD3DDeviceContext();

    // Draw the scene.
    m_model->Draw(context, *m_states, m_world, m_view, m_proj);

    p_deviceResources->PIXEndEvent();

    if (m_msaa)
    {
        // Resolve the MSAA render target.
        p_deviceResources->PIXBeginEvent(L"Resolve");

        auto backBuffer = p_deviceResources->GetRenderTarget();
        context->ResolveSubresource(backBuffer, 0, m_msaaRenderTarget.Get(), 0, c_backBufferFormat);

        p_deviceResources->PIXEndEvent();

        // Set render target for UI which is typically rendered without MSAA.
        auto renderTarget = p_deviceResources->GetRenderTargetView();
        context->OMSetRenderTargets(1, &renderTarget, nullptr);
    }

    // Draw UI
    p_deviceResources->PIXBeginEvent(L"Draw UI");

    auto size = p_deviceResources->GetOutputSize();
    auto safe = SimpleMath::Viewport::ComputeTitleSafeArea(size.right, size.bottom);

    m_batch->Begin();

    wchar_t str[32] = {};
    swprintf_s(str, L"Sample count: %u", m_msaa ? m_sampleCount : 1);
    m_smallFont->DrawString(m_batch.get(), str, XMFLOAT2(float(safe.left), float(safe.top)), ATG::Colors::White);

    const wchar_t* legend = m_app->IsGamePadConnected()
        ? L"[A] Toggle MSAA   [View] Exit"
        : L"Space: Toggle MSAA   Esc: Exit";

    DX::DrawControllerString(m_batch.get(),
        m_smallFont.get(), m_ctrlFont.get(),
        legend,
        XMFLOAT2(float(safe.left),
            float(safe.bottom) - m_smallFont->GetLineSpacing()),
        ATG::Colors::LightGrey);

    m_batch->End();

    p_deviceResources->PIXEndEvent();

    // Show the new frame.
    p_deviceResources->Present();
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::Clear(DX::DeviceResources * p_deviceResources)
{
    auto context = p_deviceResources->GetD3DDeviceContext();
    p_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.

    if (m_msaa)
    {
        //
        // Rather than operate on the swapchain render target, we set up to render the scene to our MSAA resources instead.
        //

        context->ClearRenderTargetView(m_msaaRenderTargetView.Get(), ATG::ColorsLinear::Background);
        context->ClearDepthStencilView(m_msaaDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        context->OMSetRenderTargets(1, m_msaaRenderTargetView.GetAddressOf(), m_msaaDepthStencilView.Get());
    }
    else
    {
        auto renderTarget = p_deviceResources->GetRenderTargetView();
        auto depthStencil = p_deviceResources->GetDepthStencilView();

        context->ClearRenderTargetView(renderTarget, ATG::ColorsLinear::Background);
        context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        context->OMSetRenderTargets(1, &renderTarget, depthStencil);
    }

    // Set the viewport.
    auto viewport = p_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    p_deviceResources->PIXEndEvent();
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::HandleCleanup()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleMSAA::HandleCleanup()" << std::endl;
    //*/

}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::CreateDeviceDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleMSAA::CreateDeviceDependentResources()" << std::endl;
    //*/

    auto device = p_deviceResources->GetD3DDevice();

    auto context = p_deviceResources->GetD3DDeviceContext();
    m_batch = std::make_unique<SpriteBatch>(context);

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);

    wchar_t strFilePath[MAX_PATH] = {};
    DX::FindMediaFile(strFilePath, MAX_PATH, L"AliasSampleCityBlock\\CityBlockConcrete.sdkmesh");

    {
        wchar_t drive[_MAX_DRIVE];
        wchar_t path[_MAX_PATH];

        if (_wsplitpath_s(strFilePath, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
            throw std::exception("_wsplitpath_s");

        wchar_t directory[_MAX_PATH];
        if (_wmakepath_s(directory, _MAX_PATH, drive, path, nullptr, nullptr))
            throw std::exception("_wmakepath_s");

        m_fxFactory->SetDirectory(directory);
    }

    m_model = Model::CreateFromSDKMESH(device, strFilePath, *m_fxFactory);

    m_world = Matrix::Identity;

    // Load UI.
    DX::FindMediaFile(strFilePath, MAX_PATH, L"SegoeUI_18.spritefont");
    m_smallFont = std::make_unique<SpriteFont>(device, strFilePath);

    DX::FindMediaFile(strFilePath, MAX_PATH, L"XboxOneControllerLegendSmall.spritefont");
    m_ctrlFont = std::make_unique<SpriteFont>(device, strFilePath);

    //
    // Check for MSAA support.
    //
    // Note that 4x MSAA is required for Direct3D Feature Level 10.1 or better
    //           8x MSAA is required for Direct3D Feature Level 11.0 or better
    //

    for (m_sampleCount = c_targetSampleCount; m_sampleCount > 1; m_sampleCount--)
    {
        UINT levels = 0;
        if (FAILED(device->CheckMultisampleQualityLevels(c_backBufferFormat, m_sampleCount, &levels)))
            continue;

        if (levels > 0)
            break;
    }

    if (m_sampleCount < 2)
    {
        throw std::exception("MSAA not supported");
    }
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::CreateWindowSizeDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleMSAA::CreateWindowSizeDependentResources()" << std::endl;
    //*/

    auto output = p_deviceResources->GetOutputSize();

    // Determine the render target size in pixels.
    UINT backBufferWidth = std::max<UINT>(output.right - output.left, 1);
    UINT backBufferHeight = std::max<UINT>(output.bottom - output.top, 1);

    // Create an MSAA render target.
    CD3D11_TEXTURE2D_DESC renderTargetDesc(
        c_backBufferFormat,
        backBufferWidth,
        backBufferHeight,
        1, // The render target view has only one texture.
        1, // Use a single mipmap level.
        D3D11_BIND_RENDER_TARGET,
        D3D11_USAGE_DEFAULT,
        0,
        m_sampleCount
    );

    auto device = p_deviceResources->GetD3DDevice();
    DX::ThrowIfFailed(device->CreateTexture2D(
        &renderTargetDesc,
        nullptr,
        m_msaaRenderTarget.ReleaseAndGetAddressOf()
    ));

    CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2DMS, c_backBufferFormat);

    DX::ThrowIfFailed(device->CreateRenderTargetView(
        m_msaaRenderTarget.Get(),
        &renderTargetViewDesc,
        m_msaaRenderTargetView.ReleaseAndGetAddressOf()
    ));

    // Create an MSAA depth stencil view.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(
        c_depthBufferFormat,
        backBufferWidth,
        backBufferHeight,
        1, // This depth stencil view has only one texture.
        1, // Use a single mipmap level.
        D3D11_BIND_DEPTH_STENCIL,
        D3D11_USAGE_DEFAULT,
        0,
        m_sampleCount
    );

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(device->CreateTexture2D(
        &depthStencilDesc,
        nullptr,
        depthStencil.GetAddressOf()
    ));

    DX::ThrowIfFailed(device->CreateDepthStencilView(
        depthStencil.Get(),
        nullptr,
        m_msaaDepthStencilView.ReleaseAndGetAddressOf()
    ));

    // Setup test scene.
    m_view = Matrix::CreateLookAt(Vector3(0, -211.f, -23.f), Vector3(6.f, 0.f, -37.f), -Vector3::UnitZ);

    m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
        float(backBufferWidth) / float(backBufferHeight), 0.1f, 1000.f);

    auto viewport = p_deviceResources->GetScreenViewport();
    m_batch->SetViewport(viewport);
}

//-----------------------------------------------------------------------------

void AppStateSimpleMSAA::ResetResources()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleMSAA::ResetRessources()" << std::endl;
    //*/

    m_msaaRenderTarget.Reset();
    m_msaaRenderTargetView.Reset();
    m_msaaDepthStencilView.Reset();

    m_batch.reset();
    m_smallFont.reset();
    m_ctrlFont.reset();
    m_states.reset();
    m_model.reset();
    m_fxFactory.reset();
}

} // namespace nxn
