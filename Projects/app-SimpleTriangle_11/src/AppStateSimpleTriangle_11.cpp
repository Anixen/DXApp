#include "pch.h"
#include "AppStateSimpleTriangle_11.h"

#include "ATGColors.h"
#include "ReadData.h"

using namespace DirectX;

namespace
{
    struct Vertex
    {
        XMFLOAT4 position;
        XMFLOAT4 color;
    };
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleTriangle_11::AppStateSimpleTriangle_11(App_11 *p_app) :
	AppState_11(p_app)
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateSimpleTriangle_11::~AppStateSimpleTriangle_11()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::Init() 
{
    GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::Init()" << std::endl;

	AppState_11::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::Reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::Reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState_11* AppStateSimpleTriangle_11::Update(
    DX::DeviceResources_11* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_gamePadConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_gamePadConnected;

    (void)p_mouse;

    (void)p_gamePadButtons;
    (void)p_keyboardButtons;
    (void)p_mouseButtons;

    double elapsedSeconds = p_timer.GetElapsedSeconds();
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::Update(" << elapsedSeconds << ")" << std::endl;
	//*/

    auto pad = p_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        if (pad.IsViewPressed())
        {
            m_app->Quit(0);
        }
    }

    auto kb = p_keyboard->GetState();
    if (kb.Escape)
    {
        m_app->Quit(0);
    }

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::Draw(DX::DeviceResources_11* p_deviceResources)
{
    Clear(p_deviceResources);

    p_deviceResources->PIXBeginEvent(L"Render");
    auto context = p_deviceResources->GetD3DDeviceContext();

    // Set input assembler state.
    context->IASetInputLayout(m_spInputLayout.Get());

    UINT strides = sizeof(Vertex);
    UINT offsets = 0;
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetVertexBuffers(0, 1, m_spVertexBuffer.GetAddressOf(), &strides, &offsets);

    // Set shaders.
    context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
    context->GSSetShader(nullptr, nullptr, 0);
    context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);

    // Draw triangle.
    context->Draw(3, 0);

    p_deviceResources->PIXEndEvent();

    // Show the new frame.
    p_deviceResources->Present();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::Clear(DX::DeviceResources_11* p_deviceResources)
{
    p_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = p_deviceResources->GetD3DDeviceContext();
    auto renderTarget = p_deviceResources->GetRenderTargetView();
    auto depthStencil = p_deviceResources->GetDepthStencilView();

    // Use linear clear color for gamma-correct rendering.
    context->ClearRenderTargetView(renderTarget, ATG::ColorsLinear::Background);

    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = p_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    p_deviceResources->PIXEndEvent();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::HandleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_11::HandleCleanup()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::CreateDeviceDependentResources(DX::DeviceResources_11* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_11::CreateDeviceDependentResources()" << std::endl;

    auto device = p_deviceResources->GetD3DDevice();

    // Load and create shaders.
    auto vertexShaderBlob = DX::ReadData(L"SimpleTriangle_11_VertexShader.cso");

    DX::ThrowIfFailed(
        device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(),
            nullptr, m_spVertexShader.ReleaseAndGetAddressOf()));

    auto pixelShaderBlob = DX::ReadData(L"SimpleTriangle_11_PixelShader.cso");

    DX::ThrowIfFailed(
        device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
            nullptr, m_spPixelShader.ReleaseAndGetAddressOf()));

    // Create input layout.
    static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
    {
        { "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
        { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
    };

    DX::ThrowIfFailed(
        device->CreateInputLayout(s_inputElementDesc, _countof(s_inputElementDesc),
            vertexShaderBlob.data(), vertexShaderBlob.size(),
            m_spInputLayout.ReleaseAndGetAddressOf()));

    // Create vertex buffer.
    static const Vertex s_vertexData[3] =
    {
        { { 0.0f,   0.5f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
        { { 0.5f,  -0.5f,  0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
        { { -0.5f, -0.5f,  0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }   // Left / Blue
    };

    D3D11_SUBRESOURCE_DATA initialData = {};
    initialData.pSysMem = s_vertexData;

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(s_vertexData);
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.StructureByteStride = sizeof(Vertex);

    DX::ThrowIfFailed(
        device->CreateBuffer(&bufferDesc, &initialData,
            m_spVertexBuffer.ReleaseAndGetAddressOf()));
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::CreateWindowSizeDependentResources(DX::DeviceResources_11* p_deviceResources)
{
    (void)p_deviceResources;

    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_11::CreateWindowSizeDependentResources()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_11::ResetResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_11::ResetRessources()" << std::endl;

    m_spInputLayout .Reset();
    m_spVertexBuffer.Reset();
    m_spVertexShader.Reset();
    m_spPixelShader .Reset();
}

} // namespace nxn
