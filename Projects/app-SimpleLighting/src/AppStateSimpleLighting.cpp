#include "pch.h"
#include "AppStateSimpleLighting.h"

#include "ATGColors.h"
#include "FindMedia.h"
#include "ReadData.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace
{
    struct Vertex
    {
        XMFLOAT3 pos;
        XMFLOAT3 normal;
    };

    struct ConstantBuffer
    {
        XMMATRIX worldMatrix;
        XMMATRIX viewMatrix;
        XMMATRIX projectionMatrix;
        XMVECTOR lightDir[2];
        XMVECTOR lightColor[2];
        XMVECTOR outputColor;
    };

    static_assert((sizeof(ConstantBuffer) % 16) == 0, "Constant buffer must always be 16-byte aligned");
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleLighting::AppStateSimpleLighting(App *p_app) :
	AppState(p_app)
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppStateSimpleLighting::~AppStateSimpleLighting()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::Init() {
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::Init()" << std::endl;
    //*/

	AppState::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::Reinit()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::Reinit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppState* AppStateSimpleLighting::Update(
    DX::DeviceResources* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_gamePadConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_gamePadConnected;

    (void)p_mouse;
    (void)p_mouseButtons;

    float elapsedSeconds = float(p_timer.GetElapsedSeconds());

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::Update(" << elapsedSeconds << ")" << std::endl;
	//*/

    // Update the rotation constant
    m_curRotationAngleRad += elapsedSeconds / 3.f;
    if (m_curRotationAngleRad >= XM_2PI)
    {
        m_curRotationAngleRad -= XM_2PI;
    }

    // Rotate the cube around the origin
    XMStoreFloat4x4(&m_worldMatrix, XMMatrixRotationY(m_curRotationAngleRad));

    // Setup our lighting parameters
    m_lightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
    m_lightDirs[1] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);

    m_lightColors[0] = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_lightColors[1] = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);

    // Rotate the second light around the origin
    XMMATRIX rotate = XMMatrixRotationY(-2.0f * m_curRotationAngleRad);
    XMVECTOR lightDir = XMLoadFloat4(&m_lightDirs[1]);
    lightDir = XMVector3Transform(lightDir, rotate);
    XMStoreFloat4(&m_lightDirs[1], lightDir);

    // Handle controller input for exit
    auto pad = p_gamePad->GetState(0);
    if (pad.IsConnected())
    {
        p_gamePadButtons.Update(pad);

        if (pad.IsViewPressed())
        {
            m_app->Quit(0);
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

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::Draw(DX::DeviceResources* p_deviceResources)
{
    Clear(p_deviceResources);

    auto context = p_deviceResources->GetD3DDeviceContext();
    p_deviceResources->PIXBeginEvent(L"Render");

    // Set the vertex buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

    // Set the index buffer
    context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Set the input layout
    context->IASetInputLayout(m_inputLayout.Get());

    // Set the primitive toplogy
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set the per-frame constants
    ConstantBuffer sceneParameters = {};

    // Shaders compiled with default row-major matrices
    sceneParameters.worldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_worldMatrix));
    sceneParameters.viewMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_viewMatrix));
    sceneParameters.projectionMatrix = XMMatrixTranspose(XMLoadFloat4x4(&m_projectionMatrix));

    sceneParameters.lightDir[0] = XMLoadFloat4(&m_lightDirs[0]);
    sceneParameters.lightDir[1] = XMLoadFloat4(&m_lightDirs[1]);
    sceneParameters.lightColor[0] = XMLoadFloat4(&m_lightColors[0]);
    sceneParameters.lightColor[1] = XMLoadFloat4(&m_lightColors[1]);
    sceneParameters.outputColor = XMLoadFloat4(&m_outputColor);

    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        DX::ThrowIfFailed(context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
        memcpy(mapped.pData, &sceneParameters, sizeof(ConstantBuffer));
        context->Unmap(m_constantBuffer.Get(), 0);
    }

    // Render the cube
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

    // Vertex shader needs view and projection matrices to perform vertex transform
    context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Pixel shader needs light-direction vectors to perform per-pixel lighting
    context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
    context->DrawIndexed(36, 0, 0);

    // Render each light
    for (int m = 0; m < 2; ++m)
    {
        XMMATRIX lightMatrix = XMMatrixTranslationFromVector(5.0f * sceneParameters.lightDir[m]);
        XMMATRIX lightScaleMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
        lightMatrix = lightScaleMatrix * lightMatrix;

        // Update the world variable to reflect the current light
        sceneParameters.worldMatrix = XMMatrixTranspose(lightMatrix);
        sceneParameters.outputColor = sceneParameters.lightColor[m];

        {
            D3D11_MAPPED_SUBRESOURCE mapped;
            DX::ThrowIfFailed(context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
            memcpy(mapped.pData, &sceneParameters, sizeof(ConstantBuffer));
            context->Unmap(m_constantBuffer.Get(), 0);
        }

        context->VSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());
        context->PSSetConstantBuffers(0, 1, m_constantBuffer.GetAddressOf());

        context->PSSetShader(m_pixelShaderSolid.Get(), nullptr, 0);
        context->DrawIndexed(36, 0, 0);
    }
    p_deviceResources->PIXEndEvent();

    // Show the new frame.
    p_deviceResources->Present();
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::Clear(DX::DeviceResources * p_deviceResources)
{
    p_deviceResources->PIXBeginEvent(L"Clear");

    auto context = p_deviceResources->GetD3DDeviceContext();

    // Clear the views.
    auto renderTarget = p_deviceResources->GetRenderTargetView();
    auto depthStencil = p_deviceResources->GetDepthStencilView();

    // Use linear clear color for gamma-correct rendering
    context->ClearRenderTargetView(renderTarget, ATG::ColorsLinear::Background);

    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = p_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    p_deviceResources->PIXEndEvent();
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::HandleCleanup()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting::HandleCleanup()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::CreateDeviceDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting::CreateDeviceDependentResources()" << std::endl;
    //*/

    auto device = p_deviceResources->GetD3DDevice();

    // Load and create shaders
    {
        auto blob = DX::ReadData(L"SimpleLighting_TriangleVS.cso");

        DX::ThrowIfFailed(
            device->CreateVertexShader(blob.data(), blob.size(),
                nullptr, m_vertexShader.ReleaseAndGetAddressOf()));

        // Create the input layout
        const D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DX::ThrowIfFailed(
            device->CreateInputLayout(inputElementDesc, _countof(inputElementDesc),
                blob.data(), blob.size(),
                m_inputLayout.ReleaseAndGetAddressOf()));
    }

    {
        auto blob = DX::ReadData(L"SimpleLighting_LambertPS.cso");

        DX::ThrowIfFailed(
            device->CreatePixelShader(blob.data(), blob.size(),
                nullptr, m_pixelShader.ReleaseAndGetAddressOf()));
    }

    {
        auto blob = DX::ReadData(L"SimpleLighting_SolidColorPS.cso");

        DX::ThrowIfFailed(
            device->CreatePixelShader(blob.data(), blob.size(),
                nullptr, m_pixelShaderSolid.ReleaseAndGetAddressOf()));
    }

    // Create and initialize the vertex buffer
    {
        static const Vertex vertices[] =
        {
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

            { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

            { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
            { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
        };

        CD3D11_BUFFER_DESC bufferDesc(sizeof(Vertex) * _countof(vertices), D3D11_BIND_VERTEX_BUFFER);
        bufferDesc.StructureByteStride = sizeof(Vertex);

        D3D11_SUBRESOURCE_DATA initData = { vertices, 0, 0 };
        DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, &initData, m_vertexBuffer.ReleaseAndGetAddressOf()));
    }

    // Create and initialize the index buffer
    {
        static const uint16_t indices[] =
        {
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
        CD3D11_BUFFER_DESC bufferDesc(sizeof(uint16_t) * _countof(indices), D3D11_BIND_INDEX_BUFFER);
        bufferDesc.StructureByteStride = sizeof(uint16_t);

        D3D11_SUBRESOURCE_DATA initData = { indices, 0, 0 };
        DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, &initData, m_indexBuffer.ReleaseAndGetAddressOf()));
    }

    // Create the constant buffer
    {
        CD3D11_BUFFER_DESC bufferDesc(sizeof(ConstantBuffer), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
        DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));
    }

    // Initialize the world matrix
    XMStoreFloat4x4(&m_worldMatrix, XMMatrixIdentity());

    // Initialize the view matrix
    static const XMVECTORF32 c_eye = { 0.0f, 4.0f, -10.0f, 0.0f };
    static const XMVECTORF32 c_at = { 0.0f, 1.0f, 0.0f, 0.0f };
    static const XMVECTORF32 c_up = { 0.0f, 1.0f, 0.0f, 0.0 };
    XMStoreFloat4x4(&m_viewMatrix, XMMatrixLookAtLH(c_eye, c_at, c_up));

    // Initialize the lighting parameters
    m_lightDirs[0] = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
    m_lightDirs[1] = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);

    m_lightColors[0] = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    m_lightColors[1] = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);

    // Initialize the scene output color
    m_outputColor = XMFLOAT4(0, 0, 0, 0);
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::CreateWindowSizeDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting::CreateWindowSizeDependentResources()" << std::endl;
    //*/

    // Initialize the projection matrix
    auto size = p_deviceResources->GetOutputSize();
    XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, float(size.right) / float(size.bottom), 0.01f, 100.0f);

    XMStoreFloat4x4(&m_projectionMatrix, projection);
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting::ResetResources()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting::ResetRessources()" << std::endl;
    //*/

    m_inputLayout.Reset();
    m_vertexBuffer.Reset();
    m_indexBuffer.Reset();
    m_constantBuffer.Reset();
    m_vertexShader.Reset();
    m_pixelShader.Reset();
    m_pixelShaderSolid.Reset();
}

} // namespace nxn
