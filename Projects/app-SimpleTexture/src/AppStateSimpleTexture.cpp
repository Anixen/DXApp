#include "pch_SimpleTexture.h"
#include "AppStateSimpleTexture.h"

#include "ATGColors.h"
#include "FindMedia.h"
#include "ReadData.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace
{
    struct Vertex
    {
        XMFLOAT4 position;
        XMFLOAT2 texcoord;
    };

    std::vector<uint8_t> LoadBGRAImage(const wchar_t* filename, uint32_t& width, uint32_t& height)
    {
        ComPtr<IWICImagingFactory> wicFactory;
        DX::ThrowIfFailed(CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory)));

        ComPtr<IWICBitmapDecoder> decoder;
        DX::ThrowIfFailed(wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf()));

        ComPtr<IWICBitmapFrameDecode> frame;
        DX::ThrowIfFailed(decoder->GetFrame(0, frame.GetAddressOf()));

        DX::ThrowIfFailed(frame->GetSize(&width, &height));

        WICPixelFormatGUID pixelFormat;
        DX::ThrowIfFailed(frame->GetPixelFormat(&pixelFormat));

        uint32_t rowPitch = width * sizeof(uint32_t);
        uint32_t imageSize = rowPitch * height;

        std::vector<uint8_t> image;
        image.resize(size_t(imageSize));

        if (memcmp(&pixelFormat, &GUID_WICPixelFormat32bppBGRA, sizeof(GUID)) == 0)
        {
            DX::ThrowIfFailed(frame->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data())));
        }
        else
        {
            ComPtr<IWICFormatConverter> formatConverter;
            DX::ThrowIfFailed(wicFactory->CreateFormatConverter(formatConverter.GetAddressOf()));

            BOOL canConvert = FALSE;
            DX::ThrowIfFailed(formatConverter->CanConvert(pixelFormat, GUID_WICPixelFormat32bppBGRA, &canConvert));
            if (!canConvert)
            {
                throw std::exception("CanConvert");
            }

            DX::ThrowIfFailed(formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA,
                WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeMedianCut));

            DX::ThrowIfFailed(formatConverter->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data())));
        }

        return image;
    }
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleTexture::AppStateSimpleTexture(App *p_app) :
	AppState(p_app)
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::ctor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppStateSimpleTexture::~AppStateSimpleTexture()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::dtor()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::Init() {
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::init()" << std::endl;
    //*/

	AppState::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::Reinit()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::reinit()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

AppState* AppStateSimpleTexture::Update(
    DX::DeviceResources* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_ctrlConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_ctrlConnected;

    (void)p_mouse;

    (void)p_gamePadButtons;
    (void)p_keyboardButtons;
    (void)p_mouseButtons;

    double elapsedSeconds = p_timer.GetElapsedSeconds();
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::update(" << elapsedSeconds << ")" << std::endl;
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

void AppStateSimpleTexture::Draw(DX::DeviceResources* p_deviceResources)
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

    context->IASetIndexBuffer(m_spIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Set shaders.
    context->VSSetShader(m_spVertexShader.Get(), nullptr, 0);
    context->GSSetShader(nullptr, nullptr, 0);
    context->PSSetShader(m_spPixelShader.Get(), nullptr, 0);

    // Set texture and sampler.
    auto sampler = m_spSampler.Get();
    context->PSSetSamplers(0, 1, &sampler);

    auto texture = m_spTexture.Get();
    context->PSSetShaderResources(0, 1, &texture);

    // Draw quad.
    context->DrawIndexed(6, 0, 0);

    p_deviceResources->PIXEndEvent();

    // Show the new frame.
    p_deviceResources->Present();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::Clear(DX::DeviceResources * p_deviceResources)
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

void AppStateSimpleTexture::HandleCleanup()
{
    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTexture::handleCleanup()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::CreateDeviceDependentResources(DX::DeviceResources * p_deviceResources)
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTexture::createDeviceDependentResources()" << std::endl;
    //*/

    auto device = p_deviceResources->GetD3DDevice();

    // Load and create shaders.
    auto vertexShaderBlob = DX::ReadData(L"SimpleTexture_VertexShader.cso");

    DX::ThrowIfFailed(
        device->CreateVertexShader(vertexShaderBlob.data(), vertexShaderBlob.size(),
            nullptr, m_spVertexShader.ReleaseAndGetAddressOf()));

    auto pixelShaderBlob = DX::ReadData(L"SimpleTexture_PixelShader.cso");

    DX::ThrowIfFailed(
        device->CreatePixelShader(pixelShaderBlob.data(), pixelShaderBlob.size(),
            nullptr, m_spPixelShader.ReleaseAndGetAddressOf()));

    // Create input layout.
    static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
    {
        { "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
        { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
    };

    DX::ThrowIfFailed(
        device->CreateInputLayout(s_inputElementDesc, _countof(s_inputElementDesc),
            vertexShaderBlob.data(), vertexShaderBlob.size(),
            m_spInputLayout.ReleaseAndGetAddressOf()));

    // Create vertex buffer.
    static const Vertex s_vertexData[4] =
    {
        { { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.f, 1.f } },
        { {  0.5f, -0.5f, 0.5f, 1.0f }, { 1.f, 1.f } },
        { {  0.5f,  0.5f, 0.5f, 1.0f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f, 0.5f, 1.0f }, { 0.f, 0.f } },
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

    // Create index buffer.
    static const uint16_t s_indexData[6] =
    {
        3,1,0,
        2,1,3,
    };

    initialData.pSysMem = s_indexData;

    bufferDesc.ByteWidth = sizeof(s_indexData);
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.StructureByteStride = sizeof(uint16_t);

    DX::ThrowIfFailed(
        device->CreateBuffer(&bufferDesc, &initialData,
            m_spIndexBuffer.ReleaseAndGetAddressOf()));

    // Create sampler.
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    DX::ThrowIfFailed(
        device->CreateSamplerState(&samplerDesc,
            m_spSampler.ReleaseAndGetAddressOf()));

    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc = {};
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // sunset.jpg is in sRGB colorspace
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    wchar_t buff[MAX_PATH];
    DX::FindMediaFile(buff, MAX_PATH, L"sunset.jpg");
    auto image = LoadBGRAImage(buff, txtDesc.Width, txtDesc.Height);

    initialData.pSysMem = image.data();
    initialData.SysMemPitch = txtDesc.Width * sizeof(uint32_t);

    ComPtr<ID3D11Texture2D> tex;
    DX::ThrowIfFailed(
        device->CreateTexture2D(&txtDesc, &initialData,
            tex.GetAddressOf()));

    DX::ThrowIfFailed(
        device->CreateShaderResourceView(tex.Get(),
            nullptr, m_spTexture.ReleaseAndGetAddressOf()));
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::CreateWindowSizeDependentResources(DX::DeviceResources * p_deviceResources)
{
    (void)p_deviceResources;

    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTexture::createWindwSizeDependentResources()" << std::endl;
    //*/
}

//-----------------------------------------------------------------------------

void AppStateSimpleTexture::ResetResources()
{
    /*
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTexture::resetRessources()" << std::endl;
    //*/

    m_spInputLayout .Reset();
    m_spVertexBuffer.Reset();
    m_spIndexBuffer .Reset();
    m_spVertexShader.Reset();
    m_spPixelShader .Reset();
    m_spSampler     .Reset();
    m_spTexture     .Reset();
}

} // namespace nxn
