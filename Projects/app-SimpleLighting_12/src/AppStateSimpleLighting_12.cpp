#include "pch.h"
#include "AppStateSimpleLighting_12.h"

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
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleLighting_12::AppStateSimpleLighting_12(App_12 *p_app) :
	AppState_12(p_app)
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::ctor()" << std::endl;
}

//-----------------------------------------------------------------------------

AppStateSimpleLighting_12::~AppStateSimpleLighting_12()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::Init() 
{
    GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::Init()" << std::endl;

	AppState_12::Init();
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::Reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::Reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState_12* AppStateSimpleLighting_12::Update(
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
    (void)p_mouseButtons;

    double elapsedSeconds = p_timer.GetElapsedSeconds();
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::Update(" << elapsedSeconds << ")" << std::endl;
	//*/

    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(p_timer.GetElapsedSeconds());

    // Update the rotation constant
    m_curRotationAngleRad += elapsedTime / 3.f;
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

    PIXEndEvent();

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::Draw(DX::DeviceResources_12* p_deviceResources)
{
    // Check to see if the GPU is keeping up
    int frameIdx = p_deviceResources->GetCurrentFrameIndex();
    int numBackBuffers = p_deviceResources->GetBackBufferCount();
    uint64_t completedValue = m_fence->GetCompletedValue();
    if ((frameIdx > completedValue) // if frame index is reset to zero it may temporarily be smaller than the last GPU signal
        && (frameIdx - completedValue > numBackBuffers))
    {
        // GPU not caught up, wait for at least one available frame
        DX::ThrowIfFailed(m_fence->SetEventOnCompletion(frameIdx - numBackBuffers, m_fenceEvent.Get()));
        WaitForSingleObjectEx(m_fenceEvent.Get(), INFINITE, FALSE);
    }

    // Prepare the command list to render a new frame.
    p_deviceResources->Prepare();
    Clear(p_deviceResources);

    auto commandList = p_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // Index into the available constant buffers based on the number
    // of draw calls. We've allocated enough for a known number of
    // draw calls per frame times the number of back buffers
    unsigned int constantBufferIndex = c_numDrawCalls * (frameIdx % numBackBuffers);

    // Set the root signature and pipeline state for the command list
    commandList->SetGraphicsRootSignature(m_rootSignature.Get());
    commandList->SetPipelineState(m_lambertPipelineState.Get());

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

    // Set the constants for the first draw call
    memcpy(&m_mappedConstantData[constantBufferIndex].constants, &sceneParameters, sizeof(ConstantBuffer));

    // Bind the constants to the shader
    auto baseGpuAddress = m_constantDataGpuAddr + sizeof(PaddedConstantBuffer) * constantBufferIndex;
    commandList->SetGraphicsRootConstantBufferView(c_rootParameterCB, baseGpuAddress);

    // Set up the input assembler
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    commandList->IASetIndexBuffer(&m_indexBufferView);

    // Draw the Lambert lit cube
    commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
    baseGpuAddress += sizeof(PaddedConstantBuffer);
    ++constantBufferIndex;

    // Render each light
    commandList->SetPipelineState(m_solidColorPipelineState.Get());

    for (int m = 0; m < 2; ++m)
    {
        XMMATRIX lightMatrix = XMMatrixTranslationFromVector(5.0f * sceneParameters.lightDir[m]);
        XMMATRIX lightScaleMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
        lightMatrix = lightScaleMatrix * lightMatrix;

        // Update the world variable to reflect the current light
        sceneParameters.worldMatrix = XMMatrixTranspose(lightMatrix);
        sceneParameters.outputColor = sceneParameters.lightColor[m];

        // Set the constants for the draw call
        memcpy(&m_mappedConstantData[constantBufferIndex].constants, &sceneParameters, sizeof(ConstantBuffer));

        // Bind the constants to the shader
        commandList->SetGraphicsRootConstantBufferView(c_rootParameterCB, baseGpuAddress);

        commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
        baseGpuAddress += sizeof(PaddedConstantBuffer);
        ++constantBufferIndex;
    }

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(p_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
    p_deviceResources->Present();

    // GPU will signal an increasing value each frame
    p_deviceResources->GetCommandQueue()->Signal(m_fence.Get(), frameIdx);

    PIXEndEvent(p_deviceResources->GetCommandQueue());

}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::Clear(DX::DeviceResources_12* p_deviceResources)
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

void AppStateSimpleLighting_12::HandleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleLighting_12::HandleCleanup()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::CreateDeviceDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting_12::CreateDeviceDependentResources()" << std::endl;

    auto device = p_deviceResources->GetD3DDevice();

    // Create a root signature with one constant buffer view
    {
        CD3DX12_ROOT_PARAMETER rp = {};
        rp.InitAsConstantBufferView(c_rootParameterCB, 0);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.Init(1, &rp, 0, nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
            | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS
            | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
            | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS);

        ComPtr<ID3DBlob> signature;
        ComPtr<ID3DBlob> error;
        HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
        if (FAILED(hr))
        {
            if (error)
            {
                OutputDebugStringA(reinterpret_cast<const char*>(error->GetBufferPointer()));
            }
            throw DX::com_exception(hr);
        }

        DX::ThrowIfFailed(
            device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
                IID_PPV_ARGS(m_rootSignature.ReleaseAndGetAddressOf())));
    }

    // Create the constant buffer memory and map the CPU and GPU addresses
    {
        const D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        size_t cbSize = c_numDrawCalls * p_deviceResources->GetBackBufferCount() * sizeof(PaddedConstantBuffer);

        const D3D12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(cbSize);
        DX::ThrowIfFailed(device->CreateCommittedResource(
            &uploadHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &constantBufferDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(m_perFrameConstants.ReleaseAndGetAddressOf())));

        DX::ThrowIfFailed(m_perFrameConstants->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedConstantData)));

        m_constantDataGpuAddr = m_perFrameConstants->GetGPUVirtualAddress();
    }

    // Load the shader blob for the vertex shader that will be shared by two pipeline state objects
    {
        auto triangleVSBlob = DX::ReadData(L"SimpleLighting_12_TriangleVS.cso");

        // Input element descriptor also shared by two pipeline state objects
        static const D3D12_INPUT_ELEMENT_DESC s_inputElementDesc[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        // Create the Pipelline State Object for the Lambert pixel shader
        {
            auto lambertPSBlob = DX::ReadData(L"SimpleLighting_12_LambertPS.cso");

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
            psoDesc.InputLayout = { s_inputElementDesc, _countof(s_inputElementDesc) };
            psoDesc.pRootSignature = m_rootSignature.Get();
            psoDesc.VS = { triangleVSBlob.data(), triangleVSBlob.size() };
            psoDesc.PS = { lambertPSBlob.data(), lambertPSBlob.size() };
            psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
            psoDesc.DSVFormat = p_deviceResources->GetDepthBufferFormat();
            psoDesc.SampleMask = UINT_MAX;
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.NumRenderTargets = 1;
            psoDesc.RTVFormats[0] = p_deviceResources->GetBackBufferFormat();
            psoDesc.SampleDesc.Count = 1;
            DX::ThrowIfFailed(
                device->CreateGraphicsPipelineState(&psoDesc,
                    IID_PPV_ARGS(m_lambertPipelineState.ReleaseAndGetAddressOf())));
        }

        // Create the Pipeline State Object for the solid color pixel shader
        {
            auto solidColorPSBlob = DX::ReadData(L"SimpleLighting_12_SolidColorPS.cso");

            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
            psoDesc.InputLayout = { s_inputElementDesc, _countof(s_inputElementDesc) };
            psoDesc.pRootSignature = m_rootSignature.Get();
            psoDesc.VS = { triangleVSBlob.data(), triangleVSBlob.size() };
            psoDesc.PS = { solidColorPSBlob.data(), solidColorPSBlob.size() };
            psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
            psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
            psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
            psoDesc.DSVFormat = p_deviceResources->GetDepthBufferFormat();
            psoDesc.SampleMask = UINT_MAX;
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            psoDesc.NumRenderTargets = 1;
            psoDesc.RTVFormats[0] = p_deviceResources->GetBackBufferFormat();
            psoDesc.SampleDesc.Count = 1;
            DX::ThrowIfFailed(
                device->CreateGraphicsPipelineState(&psoDesc,
                    IID_PPV_ARGS(m_solidColorPipelineState.ReleaseAndGetAddressOf())));
        }
    }

    // Create the vertex buffer
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

        // Note: using upload heaps to transfer static data like vert buffers is not 
        // recommended. Every time the GPU needs it, the upload heap will be marshalled 
        // over. Please read up on Default Heap usage. An upload heap is used here for 
        // code simplicity and because there are very few verts to actually transfer.
        const D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        const CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
        DX::ThrowIfFailed(
            device->CreateCommittedResource(&uploadHeapProperties,
                D3D12_HEAP_FLAG_NONE,
                &resourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(m_vertexBuffer.ReleaseAndGetAddressOf())));

        // Copy the data to the vertex buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
        DX::ThrowIfFailed(
            m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, vertices, sizeof(vertices));
        m_vertexBuffer->Unmap(0, nullptr);

        // Initialize the vertex buffer view.
        m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
        m_vertexBufferView.StrideInBytes = sizeof(Vertex);
        m_vertexBufferView.SizeInBytes = sizeof(vertices);
    }

    // Create the index buffer
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

        // See note above
        const D3D12_HEAP_PROPERTIES uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        const CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices));
        DX::ThrowIfFailed(
            device->CreateCommittedResource(&uploadHeapProperties,
                D3D12_HEAP_FLAG_NONE,
                &resourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(m_indexBuffer.ReleaseAndGetAddressOf())));

        // Copy the data to the index buffer.
        UINT8* pVertexDataBegin;
        CD3DX12_RANGE readRange(0, 0);		// We do not intend to read from this resource on the CPU.
        DX::ThrowIfFailed(
            m_indexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
        memcpy(pVertexDataBegin, indices, sizeof(indices));
        m_indexBuffer->Unmap(0, nullptr);

        // Initialize the index buffer view.
        m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
        m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
        m_indexBufferView.SizeInBytes = sizeof(indices);
    }

    // Wait until assets have been uploaded to the GPU.
    p_deviceResources->WaitForGpu();

    // Create a fence for synchronizing between the CPU and the GPU
    DX::ThrowIfFailed(device->CreateFence(p_deviceResources->GetCurrentFrameIndex(), D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(m_fence.ReleaseAndGetAddressOf())));

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

void AppStateSimpleLighting_12::CreateWindowSizeDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting_12::CreateWindowSizeDependentResources()" << std::endl;

    // Initialize the projection matrix
    auto size = p_deviceResources->GetOutputSize();
    XMMATRIX projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, float(size.right) / float(size.bottom), 0.01f, 100.0f);

    XMStoreFloat4x4(&m_projectionMatrix, projection);

    // The frame index will be reset to zero when the window size changes
    // So we need to tell the GPU to signal our fence starting with zero
    uint64_t currentIdx = p_deviceResources->GetCurrentFrameIndex();
    p_deviceResources->GetCommandQueue()->Signal(m_fence.Get(), currentIdx);
}

//-----------------------------------------------------------------------------

void AppStateSimpleLighting_12::ResetResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleLighting_12::ResetRessources()" << std::endl;

    m_rootSignature.Reset();
    m_lambertPipelineState.Reset();
    m_solidColorPipelineState.Reset();
    m_fence.Reset();
    m_vertexBuffer.Reset();
    m_indexBuffer.Reset();
    m_mappedConstantData = nullptr;
    m_constantDataGpuAddr = 0;
    m_perFrameConstants.Reset();
}

} // namespace nxn
