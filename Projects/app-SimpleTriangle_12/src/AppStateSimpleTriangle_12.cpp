#include "pch.h"
#include "AppStateSimpleTriangle_12.h"

#include "ATGColors.h"
#include "ReadData.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

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
AppStateSimpleTriangle_12::AppStateSimpleTriangle_12(App_12 *p_app) :
	AppState_12(p_app)
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateSimpleTriangle_12::~AppStateSimpleTriangle_12()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::Init() 
{
    GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_12::Init()" << std::endl;

	AppState_12::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::Reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_12::Reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState_12* AppStateSimpleTriangle_12::Update(
    DX::DeviceResources_12* p_deviceResources,
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
		<< "AppStateSimpleTriangle_12::Update(" << elapsedSeconds << ")" << std::endl;
	//*/

    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

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

    auto mouse = p_mouse->GetState();
    mouse;

    PIXEndEvent();

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::Draw(DX::DeviceResources_12* p_deviceResources)
{
    // Prepare the command list to render a new frame.
    p_deviceResources->Prepare();
    Clear(p_deviceResources);

    auto commandList = p_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetPipelineState(m_pipelineState.Get());

    // Set necessary state.
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);

    // Draw triangle.
    commandList->DrawInstanced(3, 1, 0, 0);

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(p_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
    p_deviceResources->Present();
    m_graphicsMemory->Commit(p_deviceResources->GetCommandQueue());
    PIXEndEvent(p_deviceResources->GetCommandQueue());
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::Clear(DX::DeviceResources_12* p_deviceResources)
{
    auto commandList = p_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto rtvDescriptor = p_deviceResources->GetRenderTargetView();
    auto dsvDescriptor = p_deviceResources->GetDepthStencilView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);

    // Use linear clear color for gamma-correct rendering.
    commandList->ClearRenderTargetView(rtvDescriptor, ATG::ColorsLinear::Background, 0, nullptr);

    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto viewport = p_deviceResources->GetScreenViewport();
    auto scissorRect = p_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::HandleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleTriangle_12::HandleCleanup()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::CreateDeviceDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_12::CreateDeviceDependentResources()" << std::endl;

    auto device = p_deviceResources->GetD3DDevice();

    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    // Create an empty root signature.
    {
        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
        rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        DX::ThrowIfFailed(
            D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
        DX::ThrowIfFailed(
            device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf())));
    }

    // Create the pipeline state, which includes loading shaders.
    auto vertexShaderBlob = DX::ReadData(L"SimpleTriangle_12_VertexShader.cso");

    auto pixelShaderBlob = DX::ReadData(L"SimpleTriangle_12_PixelShader.cso");

    static const D3D12_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
    {
        { "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,  0 },
        { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA , 0 },
    };

    // Describe and create the graphics pipeline state object (PSO).
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { s_inputElementDesc, _countof(s_inputElementDesc) };
    psoDesc.pRootSignature = m_rootSignature.Get();
    psoDesc.VS = { vertexShaderBlob.data(), vertexShaderBlob.size() };
    psoDesc.PS = { pixelShaderBlob.data(), pixelShaderBlob.size() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.DSVFormat = p_deviceResources->GetDepthBufferFormat();
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = p_deviceResources->GetBackBufferFormat();
    psoDesc.SampleDesc.Count = 1;
    DX::ThrowIfFailed(
        device->CreateGraphicsPipelineState(&psoDesc,
            IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf())));

    // Create vertex buffer.
    {
        static const Vertex s_vertexData[3] =
        {
            { { 0.0f,   0.5f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top / Red
            { { 0.5f,  -0.5f,  0.5f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right / Green
            { { -0.5f, -0.5f,  0.5f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } }   // Left / Blue
        };

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
        auto resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(s_vertexData));

        DX::ThrowIfFailed(
            device->CreateCommittedResource(&heapProps,
                D3D12_HEAP_FLAG_NONE,
                &resDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf())));

        // Copy the triangle data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
        DX::ThrowIfFailed(
            m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, s_vertexData, sizeof(s_vertexData));
        m_vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        m_vertexBufferView.SizeInBytes = sizeof(s_vertexData);
    }

    // Wait until assets have been uploaded to the GPU.
    p_deviceResources->WaitForGpu();
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::CreateWindowSizeDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    (void)p_deviceResources;

    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_12::CreateWindowSizeDependentResources()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleTriangle_12::ResetResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleTriangle_12::ResetRessources()" << std::endl;

    m_vertexBuffer.Reset();
    m_pipelineState.Reset();
    m_rootSignature.Reset();
    m_graphicsMemory.reset();
}

} // namespace nxn
