#include "pch.h"
#include "AppStateSimpleCompute.h"

#include "DirectXHelpers.h"
#include "ATGColors.h"
#include "ControllerFont.h"
#include "ReadData.h"
#include "FindMedia.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace
{
    const uint32_t s_numShaderThreads = 8;  // make sure to update value in shader if this changes

    const wchar_t* g_SampleTitle        = L"SimpleCompute";
    const wchar_t* g_SampleDescription  = L"Demonstrates how to use DirectCompute";
    const ATG::HelpButtonAssignment g_HelpButtons[] = {
        { ATG::HelpID::MENU_BUTTON,         L"Show/Hide Help" },
        { ATG::HelpID::VIEW_BUTTON,         L"Exit" },
        { ATG::HelpID::LEFT_STICK,          L"Pan Viewport" },
        { ATG::HelpID::RIGHT_STICK,         L"Zoom Viewport" },
        { ATG::HelpID::RIGHT_TRIGGER,       L"Increase Zoom Speed" },
        { ATG::HelpID::Y_BUTTON,            L"Reset Viewport to Default" },
    };
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleCompute::AppStateSimpleCompute(App *p_app)
    : AppState(p_app)
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::ctor()" << std::endl;
    //*/

    m_help = std::make_unique<ATG::Help>(g_SampleTitle, g_SampleDescription, g_HelpButtons, _countof(g_HelpButtons));
}

//-----------------------------------------------------------------------------

AppStateSimpleCompute::~AppStateSimpleCompute()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::Init() {
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::Init()" << std::endl;
    //*/

	AppState::Init();

    ResetWindow();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::Reinit()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::Reinit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppState* AppStateSimpleCompute::Update(
    DX::DeviceResources* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_ctrlConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_mouse;
    (void)p_mouseButtons;

    float elapsedSeconds = float(p_timer.GetElapsedSeconds());

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::Update(" << elapsedSeconds << ")" << std::endl;
	//*/
   
    m_renderFPS.Tick(elapsedSeconds);

    auto pad = p_gamePad->GetState(0);
    p_ctrlConnected = pad.IsConnected();
    if (p_ctrlConnected)
    {
        p_gamePadButtons.Update(pad);

        if (p_gamePadButtons.menu == GamePad::ButtonStateTracker::PRESSED)
        {
            m_showHelp = !m_showHelp;
        }
        else if (m_showHelp && p_gamePadButtons.b == GamePad::ButtonStateTracker::PRESSED)
        {
            m_showHelp = false;
        }

        if (!m_showHelp)
        {
            if (pad.IsViewPressed())
            {
                m_app->Quit(0);
            }

            const float ThumbLeftX = pad.thumbSticks.leftX;
            const float ThumbLeftY = pad.thumbSticks.leftY;
            const float ThumbRightY = pad.thumbSticks.rightY;
            const float RightTrigger = p_gamePadButtons.rightTrigger == DirectX::GamePad::ButtonStateTracker::HELD;

            if (p_gamePadButtons.y == DirectX::GamePad::ButtonStateTracker::PRESSED)
            {
                ResetWindow();
            }

            if (ThumbLeftX != 0.0f || ThumbLeftY != 0.0f || ThumbRightY != 0.0f)
            {
                const float ScaleSpeed = 1.0f + RightTrigger * 4.0f;
                const float WindowScale = 1.0f + ThumbRightY * -0.25f * ScaleSpeed * elapsedSeconds;
                m_window.x *= WindowScale;
                m_window.y *= WindowScale;
                m_window.z += m_window.x * ThumbLeftX * elapsedSeconds * 0.5f;
                m_window.w += m_window.y * ThumbLeftY * elapsedSeconds * 0.5f;
                m_windowUpdated = true;
            }

            m_windowUpdated = true;
        }
    }
    else
    {
        p_gamePadButtons.Reset();
    }

    auto kb = p_keyboard->GetState();
    p_keyboardButtons.Update(kb);

    if (p_keyboardButtons.IsKeyPressed(Keyboard::F1))
    {
        m_showHelp = !m_showHelp;
    }
    else if (m_showHelp && kb.Escape)
    {
        m_showHelp = false;
    }
    else
    {
        if (p_keyboardButtons.IsKeyPressed(Keyboard::Escape))
        {
            m_app->Quit(0);
        }

        if (p_keyboardButtons.IsKeyPressed(Keyboard::Home))
        {
            ResetWindow();
        }

        if (kb.W || kb.S || kb.A || kb.D || kb.PageUp || kb.PageDown)
        {
            const float ScaleSpeed = (kb.LeftShift || kb.RightShift) ? 4.f : 1.f;

            float zoom = kb.PageDown ? 1.f : (kb.PageUp ? -1.f : 0.f);
            float x = kb.D ? 1.f : (kb.A ? -1.f : 0.f);
            float y = kb.W ? 1.f : (kb.S ? -1.f : 0.f);

            const float WindowScale = 1.0f + zoom * ScaleSpeed * elapsedSeconds;
            m_window.x *= WindowScale;
            m_window.y *= WindowScale;
            m_window.z += m_window.x * x * elapsedSeconds * 0.5f;
            m_window.w += m_window.y * y * elapsedSeconds * 0.5f;
            m_windowUpdated = true;
        }
    }

    return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::Draw(DX::DeviceResources* p_deviceResources)
{
    Clear(p_deviceResources);

    auto context = p_deviceResources->GetD3DDeviceContext();

    if (m_showHelp)
    {
        m_help->Render();
    }
    else
    {
        // Flip colors for which async compute buffer is being rendered
        p_deviceResources->PIXBeginEvent(L"Render");

        UpdateFractalData(p_deviceResources);

        context->CSSetConstantBuffers(0, 1, m_cbFractal.GetAddressOf());
        context->CSSetShaderResources(0, 1, m_fractalColorMapSRV.GetAddressOf());
        context->CSSetSamplers(0, 1, m_fractalBilinearSampler.GetAddressOf());
        context->CSSetShader(m_csFractal.Get(), nullptr, 0);
        context->CSSetUnorderedAccessViews(0, 1, m_fractalUAV.GetAddressOf(), nullptr);

        D3D11_TEXTURE2D_DESC texDesc = {};
        m_fractalTexture->GetDesc(&texDesc);

        const uint32_t threadGroupX = texDesc.Width / s_numShaderThreads;
        const uint32_t threadGroupY = texDesc.Height / s_numShaderThreads;
        context->Dispatch(threadGroupX, threadGroupY, 1);

        ID3D11UnorderedAccessView* nulluav[] = { nullptr };
        context->CSSetUnorderedAccessViews(0, 1, nulluav, nullptr);

        RECT outputSize = p_deviceResources->GetOutputSize();

        RECT safeRect = SimpleMath::Viewport::ComputeTitleSafeArea(outputSize.right, outputSize.bottom);
        XMFLOAT2 pos(float(safeRect.left), float(safeRect.top));

        m_spriteBatch->Begin();
        m_spriteBatch->Draw(m_fractalSRV.Get(), outputSize);

        wchar_t outputString[256] = {};
        swprintf_s(outputString, 256, L"Simple Compute Context %0.2f fps", m_renderFPS.GetFPS());

        m_font->DrawString(m_spriteBatch.get(), outputString, pos);
        pos.y += m_font->GetLineSpacing();

        swprintf_s(outputString, 256, L"Synchronous compute %0.2f fps", m_renderFPS.GetFPS());
        m_font->DrawString(m_spriteBatch.get(), outputString, pos);

        const wchar_t* legend = m_app->IsCtrlConnected()
            ? L"[View] Exit   [Menu] Help"
            : L"WASD: Pan viewport   PageUp/Down: Zoom viewport   Esc: Exit";

        DX::DrawControllerString(m_spriteBatch.get(), m_font.get(), m_ctrlFont.get(),
            legend,
            XMFLOAT2(float(safeRect.left), float(safeRect.bottom) - m_font->GetLineSpacing()));

        m_spriteBatch->End();

        p_deviceResources->PIXEndEvent();
    }

    // Show the new frame.
    p_deviceResources->Present();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::Clear(DX::DeviceResources * p_deviceResources)
{
    p_deviceResources->PIXBeginEvent(L"Clear");

    auto context = p_deviceResources->GetD3DDeviceContext();

    // Clear the views.
    auto renderTarget = p_deviceResources->GetRenderTargetView();

    context->ClearRenderTargetView(renderTarget, ATG::Colors::Background);

    context->OMSetRenderTargets(1, &renderTarget, nullptr);

    // Set the viewport.
    auto viewport = p_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    p_deviceResources->PIXEndEvent();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::HandleCleanup()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute::HandleCleanup()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::CreateDeviceDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute::CreateDeviceDependentResources()" << std::endl;
    //*/

    auto device     = p_deviceResources->GetD3DDevice();
    auto context    = p_deviceResources->GetD3DDeviceContext();

    auto blob = DX::ReadData(L"SimpleCompute_FractalCS.cso");
    DX::ThrowIfFailed(
        device->CreateComputeShader(blob.data(), blob.size(), nullptr, m_csFractal.ReleaseAndGetAddressOf()));

    RECT outputSize = p_deviceResources->GetOutputSize();
    CD3D11_TEXTURE2D_DESC TexDesc(
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        outputSize.right,
        outputSize.bottom,
        1,
        1,
        D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE
    );

    DX::ThrowIfFailed(
        device->CreateTexture2D(&TexDesc, nullptr, m_fractalTexture.ReleaseAndGetAddressOf()));

    DX::ThrowIfFailed(
        device->CreateShaderResourceView(m_fractalTexture.Get(), nullptr, m_fractalSRV.ReleaseAndGetAddressOf()));

    CD3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc(D3D11_UAV_DIMENSION_TEXTURE2D, TexDesc.Format);
    DX::ThrowIfFailed(
        device->CreateUnorderedAccessView(m_fractalTexture.Get(), &UAVDesc, m_fractalUAV.ReleaseAndGetAddressOf()));

    CD3D11_BUFFER_DESC CBDesc(sizeof(CB_FractalCS), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
    DX::ThrowIfFailed(
        device->CreateBuffer(&CBDesc, nullptr, m_cbFractal.ReleaseAndGetAddressOf()));

    TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TexDesc.Width = 8;
    TexDesc.Height = 1;
    TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    const uint32_t GradientTexels[8] = { 0xFF000040, 0xFF000080, 0xFF0000C0, 0xFF0000FF, 0xFF0040FF, 0xFF0080FF, 0xFF00C0FF, 0xFF00FFFF };

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.SysMemPitch = sizeof(GradientTexels);
    InitData.pSysMem = GradientTexels;
    DX::ThrowIfFailed(
        device->CreateTexture2D(&TexDesc, &InitData, m_fractalColorMap.ReleaseAndGetAddressOf()));

    DX::ThrowIfFailed(
        device->CreateShaderResourceView(m_fractalColorMap.Get(), nullptr, m_fractalColorMapSRV.ReleaseAndGetAddressOf()));

    CD3D11_SAMPLER_DESC SamplerDesc(D3D11_DEFAULT);
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    DX::ThrowIfFailed(
        device->CreateSamplerState(&SamplerDesc, m_fractalBilinearSampler.ReleaseAndGetAddressOf()));

    m_fractalMaxIterations = 300;

    m_spriteBatch = std::make_unique<SpriteBatch>(context);

    wchar_t strFilePath[MAX_PATH] = {};
    DX::FindMediaFile(strFilePath, MAX_PATH, L"SegoeUI_18.spritefont");
    m_font = std::make_unique<SpriteFont>(device, strFilePath);

    DX::FindMediaFile(strFilePath, MAX_PATH, L"XboxOneControllerLegendSmall.spritefont");
    m_ctrlFont = std::make_unique<SpriteFont>(device, strFilePath);

    m_help->RestoreDevice(context);
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::CreateWindowSizeDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute::CreateWindowSizeDependentResources()" << std::endl;
    //*/

    auto size = p_deviceResources->GetOutputSize();
    m_help->SetWindow(size);

    m_spriteBatch->SetViewport(p_deviceResources->GetScreenViewport());
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::ResetResources()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute::ResetResources()" << std::endl;
    //*/

    m_cbFractal.Reset();
    m_csFractal.Reset();
    m_fractalTexture.Reset();
    m_fractalUAV.Reset();
    m_fractalSRV.Reset();
    m_fractalColorMap.Reset();
    m_fractalColorMapSRV.Reset();
    m_fractalBilinearSampler.Reset();

    m_spriteBatch.reset();
    m_font.reset();
    m_ctrlFont.reset();

    m_help->ReleaseDevice();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::ResetWindow()
{
    m_window = XMFLOAT4(4.0f, 2.25f, -0.65f, 0.0f);
    m_windowUpdated = true;
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute::UpdateFractalData(DX::DeviceResources * p_deviceResources)
{
    D3D11_TEXTURE2D_DESC texDesc = {};
    m_fractalTexture->GetDesc(&texDesc);

    auto context = p_deviceResources->GetD3DDeviceContext();
    D3D11_MAPPED_SUBRESOURCE mapped;

    DX::ThrowIfFailed(
        context->Map(m_cbFractal.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped)
    );

    auto pCBFractalData = reinterpret_cast<CB_FractalCS*> (mapped.pData);

    pCBFractalData->MaxThreadIter = XMFLOAT4(static_cast<float>(texDesc.Width), static_cast<float>(texDesc.Height), static_cast<float>(m_fractalMaxIterations), 0);
    pCBFractalData->Window = m_window;

    context->Unmap(m_cbFractal.Get(), 0);
}

} // namespace nxn
