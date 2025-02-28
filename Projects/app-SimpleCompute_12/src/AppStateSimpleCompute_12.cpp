#include "pch.h"
#include "AppStateSimpleCompute_12.h"

#include "ATGColors.h"
#include "FindMedia.h"
#include "ControllerFont.h"
#include "ReadData.h"

using namespace DirectX;

using Microsoft::WRL::ComPtr;

namespace
{
    const uint32_t s_numShaderThreads = 8;		// make sure to update value in shader if this changes

    const wchar_t* g_SampleTitle = L"SimpleCompute12";
    const wchar_t* g_SampleDescription = L"Demonstrates how to use the D3D12_COMMAND_LIST_TYPE_COMPUTE interface to submit asynchronous compute shader workloads";
    const ATG::HelpButtonAssignment g_HelpButtons[] = {
        { ATG::HelpID::MENU_BUTTON,         L"Show/Hide Help" },
        { ATG::HelpID::VIEW_BUTTON,         L"Exit" },
        { ATG::HelpID::LEFT_STICK,          L"Pan Viewport" },
        { ATG::HelpID::RIGHT_STICK,         L"Zoom Viewport" },
        { ATG::HelpID::RIGHT_TRIGGER,       L"Increase Zoom Speed" },
        { ATG::HelpID::A_BUTTON,            L"Toggle Async Compute" },
        { ATG::HelpID::Y_BUTTON,            L"Reset Viewport to Default" },
    };

    const D3D12_SAMPLER_DESC s_samplerType[] =
    {
        // MinMagMipPointUVWClamp
        {
            D3D12_FILTER_MIN_MAG_MIP_POINT,                 // Filter mode
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,               // U address clamping
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,               // V address clamping
            D3D12_TEXTURE_ADDRESS_MODE_CLAMP,               // W address clamping
            0.0F,                                           // Mip LOD bias
            0,                                              // Max Anisotropy - applies if using ANISOTROPIC filtering only
            D3D12_COMPARISON_FUNC_ALWAYS,                   // Comparison Func - always pass
            { 0.0F, 0.0F, 0.0F, 0.0F },                     // BorderColor float values - used if TEXTURE_ADDRESS_BORDER is set.
            0.0F,                                           // MinLOD
            D3D12_FLOAT32_MAX                               // MaxLOD
        },
    };

    //--------------------------------------------------------------------------------------
    // Inserts a resource transition operation in the command list
    //--------------------------------------------------------------------------------------
    void ResourceBarrier(_In_ ID3D12GraphicsCommandList* pCmdList, _In_ ID3D12Resource* pResource, D3D12_RESOURCE_STATES Before, D3D12_RESOURCE_STATES After, D3D12_RESOURCE_BARRIER_FLAGS Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
    {
        D3D12_RESOURCE_BARRIER barrierDesc = {};

        barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrierDesc.Flags = Flags;
        barrierDesc.Transition.pResource = pResource;
        barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrierDesc.Transition.StateBefore = Before;
        barrierDesc.Transition.StateAfter = After;

        pCmdList->ResourceBarrier(1, &barrierDesc);
    }

    struct CB_FractalCS
    {
        DirectX::XMFLOAT4 MaxThreadIter;
        DirectX::XMFLOAT4 Window;
    };
}

namespace nxn {


//-----------------------------------------------------------------------------
/**
 * @param {App*}    p_app  The address of the app to which belongs this state
 */
AppStateSimpleCompute_12::AppStateSimpleCompute_12(App_12 *p_app) 
    : AppState_12(p_app)
    , m_showHelp(false)
    , m_usingAsyncCompute(false)
    , m_renderIndex(0)
    , m_terminateThread(false)
    , m_fractalMaxIterations(300)
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::ctor()" << std::endl;

    m_help = std::make_unique<ATG::Help>(g_SampleTitle, g_SampleDescription, g_HelpButtons, _countof(g_HelpButtons));
}

//-----------------------------------------------------------------------------

AppStateSimpleCompute_12::~AppStateSimpleCompute_12()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::dtor()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::Init() 
{
    GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::Init()" << std::endl;

	AppState_12::Init();

    ResetWindow();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::Reinit()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::Reinit()" << std::endl;
}

//-----------------------------------------------------------------------------

AppState_12* AppStateSimpleCompute_12::Update(
    DX::DeviceResources_12* p_deviceResources,
    DX::StepTimer const& p_timer,
    bool & p_gamePadConnected,
    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons)
{
    (void)p_deviceResources;

    (void)p_mouse;
    (void)p_mouseButtons;

    double elapsedSeconds = p_timer.GetElapsedSeconds();
    (void)elapsedSeconds;

    /*
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::Update(" << elapsedSeconds << ")" << std::endl;
	//*/

    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(p_timer.GetElapsedSeconds());

    m_renderFPS.Tick(elapsedTime);

    auto pad = p_gamePad->GetState(0);
    p_gamePadConnected = pad.IsConnected();
    if (p_gamePadConnected)
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
                m_terminateThread = true;
            }

            if ((p_gamePadButtons.a == DirectX::GamePad::ButtonStateTracker::PRESSED))
            {
                m_usingAsyncCompute = !m_usingAsyncCompute;
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
                const float WindowScale = 1.0f + ThumbRightY * -0.25f * ScaleSpeed * elapsedTime;
                m_window.x *= WindowScale;
                m_window.y *= WindowScale;
                m_window.z += m_window.x * ThumbLeftX * elapsedTime * 0.5f;
                m_window.w += m_window.y * ThumbLeftY * elapsedTime * 0.5f;
                m_windowUpdated = true;
            }
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

        if (p_keyboardButtons.IsKeyPressed(Keyboard::Space))
        {
            m_usingAsyncCompute = !m_usingAsyncCompute;
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

            const float WindowScale = 1.0f + zoom * ScaleSpeed * elapsedTime;
            m_window.x *= WindowScale;
            m_window.y *= WindowScale;
            m_window.z += m_window.x * x * elapsedTime * 0.5f;
            m_window.w += m_window.y * y * elapsedTime * 0.5f;
            m_windowUpdated = true;
        }
    }

    PIXEndEvent();

	return nullptr;
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::Draw(DX::DeviceResources_12* p_deviceResources)
{
    // Prepare the command list to render a new frame.
    p_deviceResources->Prepare();
    Clear(p_deviceResources);

    auto commandList = p_deviceResources->GetCommandList();

    // Flip colors for which async compute buffer is being rendered
    PIXBeginEvent(commandList, m_renderIndex ? PIX_COLOR(0, 0, 255) : PIX_COLOR(0, 255, 0), L"Render");

    if (m_showHelp)
    {
        m_help->Render(commandList);
    }
    else
    {
        if (!m_usingAsyncCompute)									// the user has requested synchronous compute
        {															// add the compute work to the main command list
            if (m_windowUpdated)
            {
                UpdateFractalData(p_deviceResources);

                ID3D12DescriptorHeap* pHeaps[] = { m_SRVDescriptorHeap->Heap(), m_samplerDescriptorHeap->Heap() };
                commandList->SetDescriptorHeaps(_countof(pHeaps), pHeaps);

                EnsureResourceState(ComputeIndex(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, p_deviceResources);
                m_resourceState[ComputeIndex()] = ResourceState_Computing;

                commandList->SetComputeRootSignature(m_computeRootSignature.Get());

                commandList->SetComputeRootConstantBufferView(e_rootParameterCB, m_renderHeap.GpuAddress());
                commandList->SetComputeRootDescriptorTable(e_rootParameterSampler, m_samplerDescriptorHeap->GetGpuHandle(0));
                commandList->SetComputeRootDescriptorTable(e_rootParameterSRV, m_SRVDescriptorHeap->GetGpuHandle(e_iSRV + 2));	// sampler texture, gradient
                commandList->SetComputeRootDescriptorTable(e_rootParameterUAV, m_SRVDescriptorHeap->GetGpuHandle(e_iUAV + ComputeIndex()));

                commandList->SetPipelineState(m_computePSO.Get());
                commandList->Dispatch(m_ThreadGroupX, m_ThreadGroupY, 1);

                m_resourceState[ComputeIndex()] = ResourceState_Computed;
                SwapRenderComputeIndex();
            }
        }
        else
        {
            if (m_resourceState[ComputeIndex()] == ResourceState_Computed)			// async has finished with an update, so swap out the buffers
            {
                m_renderResourceFenceValue++;
                EnsureResourceState(RenderIndex(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, p_deviceResources);
                m_resourceState[RenderIndex()] = ResourceState_Switching;
                SwapRenderComputeIndex();
            }
            else if (m_resourceState[ComputeIndex()] == ResourceState_Switching)	// the compute buffer has finished being swapped from a pixel shader view to an unordered access view
            {																		// it's now ready for the async compute thread to use
                m_resourceState[ComputeIndex()] = ResourceState_ReadyCompute;
            }
            else if (m_resourceState[ComputeIndex()] == ResourceState_ReadyCompute)	// the async compute thread hasn't kicked off and starting using the compute buffer
            {
                // do nothing, still waiting on async compute to actually do work
            }
            else if (m_windowUpdated)												// need to kick off a new async compute, the user has changed the view area with the controller
            {
                assert((m_resourceState[RenderIndex()] == ResourceState_ReadyCompute) || (m_resourceState[RenderIndex()] == ResourceState_Rendered));
                m_renderResourceFenceValue++;
                EnsureResourceState(RenderIndex(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, p_deviceResources);
                m_resourceState[RenderIndex()] = ResourceState_Switching;
                SwapRenderComputeIndex();
            }
        }
        {
            assert((m_resourceState[RenderIndex()] == ResourceState_Computed) || (m_resourceState[RenderIndex()] == ResourceState_Rendered));
            RECT outputSize = p_deviceResources->GetOutputSize();

            m_resourceState[RenderIndex()] = ResourceState_Rendering;
            EnsureResourceState(RenderIndex(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, p_deviceResources);

            ID3D12DescriptorHeap* pHeaps[] = { m_SRVDescriptorHeap->Heap(), m_samplerDescriptorHeap->Heap() };
            commandList->SetDescriptorHeaps(_countof(pHeaps), pHeaps);

            m_spriteBatch->Begin(commandList);
            XMUINT2 texSize(outputSize.right, outputSize.bottom);
            XMFLOAT2 texLoc(0, 0);
            auto textureSRV = m_SRVDescriptorHeap->GetGpuHandle(e_iSRV + RenderIndex());
            m_spriteBatch->Draw(textureSRV, texSize, texLoc);
            m_spriteBatch->End();

            pHeaps[0] = m_resourceDescriptors->Heap();
            commandList->SetDescriptorHeaps(1, pHeaps);

            m_spriteBatch->Begin(commandList);

            {
                RECT safeRect = SimpleMath::Viewport::ComputeTitleSafeArea(outputSize.right, outputSize.bottom);
                XMFLOAT2 pos(float(safeRect.left), float(safeRect.top));

                wchar_t outputString[256] = {};
                swprintf_s(outputString, 256, L"Simple Compute Context %0.2f fps", m_renderFPS.GetFPS());

                m_font->DrawString(m_spriteBatch.get(), outputString, pos);
                pos.y += m_font->GetLineSpacing();
                if (m_usingAsyncCompute)
                {
                    swprintf_s(outputString, 256, L"Asynchronous compute %0.2f fps", m_computeFPS.GetFPS());
                    m_font->DrawString(m_spriteBatch.get(), outputString, pos);
                }
                else
                {
                    swprintf_s(outputString, 256, L"Synchronous compute %0.2f fps", m_renderFPS.GetFPS());
                    m_font->DrawString(m_spriteBatch.get(), outputString, pos);
                }

                const wchar_t* legend = m_app->IsGamePadConnected()
                    ? L"[A] Toggle asynchronous vs. synchronous   [View] Exit   [Menu] Help"
                    : L"WASD: Pan viewport   PageUp/Down: Zoom viewport    Space: Toggle async   Esc: Exit";

                DX::DrawControllerString(m_spriteBatch.get(), m_font.get(), m_ctrlFont.get(),
                    legend,
                    XMFLOAT2(float(safeRect.left), float(safeRect.bottom) - m_font->GetLineSpacing()));
            }
            m_spriteBatch->End();
        }
    }

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(p_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
    p_deviceResources->Present();

    // insert a fence for the frame, this allows the compute thread to grab the compute buffer as soon as the view is changed to unordered access
    auto commandQueue = p_deviceResources->GetCommandQueue();
    commandQueue->Signal(m_renderResourceFence.Get(), m_renderResourceFenceValue);

    m_graphicsMemory->Commit(p_deviceResources->GetCommandQueue());
    m_resourceState[RenderIndex()] = ResourceState_Rendered;

    PIXEndEvent(commandQueue);
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::Clear(DX::DeviceResources_12* p_deviceResources)
{
    auto commandList = p_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto rtvDescriptor = p_deviceResources->GetRenderTargetView();

    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, nullptr);
    commandList->ClearRenderTargetView(rtvDescriptor, ATG::Colors::Background, 0, nullptr);

    // Set the viewport and scissor rect.
    auto viewport = p_deviceResources->GetScreenViewport();
    auto scissorRect = p_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::HandleCleanup()
{
	GetLogStream(SeverityInfo)
		<< "AppStateSimpleCompute_12::HandleCleanup()" << std::endl;
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::CreateDeviceDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute_12::CreateDeviceDependentResources()" << std::endl;

    auto device = p_deviceResources->GetD3DDevice();
    auto commandList = p_deviceResources->GetCommandList();
    commandList->Reset(p_deviceResources->GetCommandAllocator(), nullptr);
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    m_resourceState[0] = m_resourceState[1] = ResourceState_ReadyCompute;

    m_resourceDescriptors = std::make_unique<DescriptorHeap>(device, Descriptors::Count);

    // create compute fence and event
    m_computeFenceEvent.Attach(CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS));
    if (!m_computeFenceEvent.IsValid())
    {
        throw std::exception("CreateEvent");
    }

    DX::ThrowIfFailed(
        device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_computeFence.ReleaseAndGetAddressOf())));
    m_computeFence->SetName(L"Compute");
    m_computeFenceValue = 1;

    DX::ThrowIfFailed(
        device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_renderResourceFence.ReleaseAndGetAddressOf())));
    m_renderResourceFence->SetName(L"Resource");
    m_renderResourceFenceValue = 1;

    // Initialize resource and descriptor heaps
    m_renderHeap = GraphicsMemory::Get().Allocate((size_t)(4 * 1024));
    m_computeHeap = GraphicsMemory::Get().Allocate((size_t)(4 * 1024));

    // sampler setup
    {
        m_samplerDescriptorHeap = std::make_unique<DescriptorHeap>(device,
            D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
            D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
            1);
        device->CreateSampler(s_samplerType, m_samplerDescriptorHeap->GetCpuHandle(0));
    }

    m_SRVDescriptorHeap = std::make_unique<DescriptorHeap>(device, e_iHeapEnd);

    // create fractal texture and views
    const D3D12_HEAP_PROPERTIES defaultHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    const D3D12_RESOURCE_DESC texDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 1920, 1080, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
    m_resourceStateFractalTexture[0] = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    DX::ThrowIfFailed(
        device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &texDesc,
            m_resourceStateFractalTexture[0],
            nullptr,
            IID_PPV_ARGS(m_fractalTexture[0].ReleaseAndGetAddressOf())));
    m_fractalTexture[0]->SetName(L"Fractal Texture 0");

    m_resourceStateFractalTexture[1] = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    DX::ThrowIfFailed(
        device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &texDesc,
            m_resourceStateFractalTexture[1],
            nullptr,
            IID_PPV_ARGS(m_fractalTexture[1].ReleaseAndGetAddressOf())));
    m_fractalTexture[1]->SetName(L"Fractal Texture 1");

    m_ThreadGroupX = static_cast<uint32_t>(texDesc.Width) / s_numShaderThreads;
    m_ThreadGroupY = texDesc.Height / s_numShaderThreads;

    // create gradient textures
    const uint32_t GradientTexels[] = { 0xFF000040, 0xFF000080, 0xFF0000C0, 0xFF0000FF, 0xFF0040FF, 0xFF0080FF, 0xFF00C0FF, 0xFF00FFFF };
    const uint32_t RainbowTexels[] = { 0xFF0000FF, 0xFF0080FF, 0xFF00FFFF, 0xFF00FF00, 0xFFFFFF00, 0xFFFF0000, 0xFF800000, 0xFFFF00FF };
    static_assert(sizeof(RainbowTexels) == sizeof(GradientTexels), "Mismatched size");

    const D3D12_RESOURCE_DESC gradientTexDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, 8, 1, 1, 1);
    DX::ThrowIfFailed(
        device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &gradientTexDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(m_fractalColorMap[0].ReleaseAndGetAddressOf())));
    m_fractalColorMap[0]->SetName(L"Fractal Color Map 0");

    DX::ThrowIfFailed(
        device->CreateCommittedResource(
            &defaultHeapProperties,
            D3D12_HEAP_FLAG_NONE,
            &gradientTexDesc,
            D3D12_RESOURCE_STATE_COMMON,
            nullptr,
            IID_PPV_ARGS(m_fractalColorMap[1].ReleaseAndGetAddressOf())));
    m_fractalColorMap[1]->SetName(L"Fractal Color Map 1");

    Microsoft::WRL::ComPtr<ID3D12Resource> colorMapIntermediate[2];
    {
        CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);

        D3D12_RESOURCE_ALLOCATION_INFO info = {};
        info.SizeInBytes = 1024;
        info.Alignment = 0;
        const D3D12_RESOURCE_DESC tempBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(info);
        DX::ThrowIfFailed(
            device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &tempBufferDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(colorMapIntermediate[0].ReleaseAndGetAddressOf())));

        DX::ThrowIfFailed(
            device->CreateCommittedResource(
                &heapProps,
                D3D12_HEAP_FLAG_NONE,
                &tempBufferDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(colorMapIntermediate[1].ReleaseAndGetAddressOf())));

        CD3DX12_SUBRESOURCE_FOOTPRINT descSubresource(gradientTexDesc, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
        ResourceBarrier(commandList, m_fractalColorMap[0].Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

        D3D12_SUBRESOURCE_DATA textureData = {};
        textureData.pData = GradientTexels;
        textureData.RowPitch = static_cast<LONG_PTR>(gradientTexDesc.Width * sizeof(uint32_t));
        textureData.SlicePitch = 1;
        UpdateSubresources(commandList, m_fractalColorMap[0].Get(), colorMapIntermediate[0].Get(), 0, 0, 1, &textureData);

        ResourceBarrier(commandList, m_fractalColorMap[0].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        textureData.pData = RainbowTexels;
        ResourceBarrier(commandList, m_fractalColorMap[1].Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
        UpdateSubresources(commandList, m_fractalColorMap[1].Get(), colorMapIntermediate[1].Get(), 0, 0, 1, &textureData);
        ResourceBarrier(commandList, m_fractalColorMap[1].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
    }

    // create uav
    device->CreateUnorderedAccessView(m_fractalTexture[0].Get(), nullptr, nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iUAV));
    device->CreateUnorderedAccessView(m_fractalTexture[1].Get(), nullptr, nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iUAV + 1));

    // create srv
    device->CreateShaderResourceView(m_fractalTexture[0].Get(), nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iSRV));
    device->CreateShaderResourceView(m_fractalTexture[1].Get(), nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iSRV + 1));
    device->CreateShaderResourceView(m_fractalColorMap[0].Get(), nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iSRV + 2));
    device->CreateShaderResourceView(m_fractalColorMap[1].Get(), nullptr, m_SRVDescriptorHeap->GetCpuHandle(e_iSRV + 3));

    // load fractal shader
    auto computeShaderBlob = DX::ReadData(L"SimpleCompute_12_FractalCS.cso");

    // Define root table layout
    {
        CD3DX12_DESCRIPTOR_RANGE descRange[e_numRootParameters];
        descRange[e_rootParameterSampler].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0); // s0
        descRange[e_rootParameterSRV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0
        descRange[e_rootParameterUAV].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0); // u0

        CD3DX12_ROOT_PARAMETER rootParameters[e_numRootParameters];
        rootParameters[e_rootParameterCB].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
        rootParameters[e_rootParameterSampler].InitAsDescriptorTable(1, &descRange[e_rootParameterSampler], D3D12_SHADER_VISIBILITY_ALL);
        rootParameters[e_rootParameterSRV].InitAsDescriptorTable(1, &descRange[e_rootParameterSRV], D3D12_SHADER_VISIBILITY_ALL);
        rootParameters[e_rootParameterUAV].InitAsDescriptorTable(1, &descRange[e_rootParameterUAV], D3D12_SHADER_VISIBILITY_ALL);

        CD3DX12_ROOT_SIGNATURE_DESC rootSignature(_countof(rootParameters), rootParameters);

        ComPtr<ID3DBlob> serializedSignature;
        DX::ThrowIfFailed(
            D3D12SerializeRootSignature(&rootSignature, D3D_ROOT_SIGNATURE_VERSION_1, serializedSignature.GetAddressOf(), nullptr));

        // Create the root signature
        DX::ThrowIfFailed(
            device->CreateRootSignature(
                0,
                serializedSignature->GetBufferPointer(),
                serializedSignature->GetBufferSize(),
                IID_PPV_ARGS(m_computeRootSignature.ReleaseAndGetAddressOf())));

        m_computeRootSignature->SetName(L"Compute RS");
    }

    // Create compute pipeline state
    D3D12_COMPUTE_PIPELINE_STATE_DESC descComputePSO = {};
    descComputePSO.pRootSignature = m_computeRootSignature.Get();
    descComputePSO.CS.pShaderBytecode = computeShaderBlob.data();
    descComputePSO.CS.BytecodeLength = computeShaderBlob.size();

    DX::ThrowIfFailed(
        device->CreateComputePipelineState(&descComputePSO, IID_PPV_ARGS(m_computePSO.ReleaseAndGetAddressOf())));
    m_computePSO->SetName(L"Compute PSO");

    // Create compute allocator, command queue and command list
    D3D12_COMMAND_QUEUE_DESC descCommandQueue = { D3D12_COMMAND_LIST_TYPE_COMPUTE, 0, D3D12_COMMAND_QUEUE_FLAG_NONE };
    DX::ThrowIfFailed(
        device->CreateCommandQueue(&descCommandQueue, IID_PPV_ARGS(m_computeCommandQueue.ReleaseAndGetAddressOf())));

    DX::ThrowIfFailed(
        device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(m_computeAllocator.ReleaseAndGetAddressOf())));

    DX::ThrowIfFailed(
        device->CreateCommandList(
            0,
            D3D12_COMMAND_LIST_TYPE_COMPUTE,
            m_computeAllocator.Get(),
            m_computePSO.Get(),
            IID_PPV_ARGS(m_computeCommandList.ReleaseAndGetAddressOf())));

    commandList->Close();
    p_deviceResources->GetCommandQueue()->ExecuteCommandLists(1, CommandListCast(&commandList));

    // Wait until assets have been uploaded to the GPU.
    p_deviceResources->WaitForGpu();

    ResourceUploadBatch resourceUpload(device);

    resourceUpload.Begin();

    {
        RenderTargetState rtState(p_deviceResources->GetBackBufferFormat(), p_deviceResources->GetDepthBufferFormat());
        SpriteBatchPipelineStateDescription pd(rtState);

        m_spriteBatch = std::make_unique<SpriteBatch>(device, resourceUpload, pd);
    }

    wchar_t strFilePath[MAX_PATH] = {};
    DX::FindMediaFile(strFilePath, MAX_PATH, L"SegoeUI_18.spritefont");
    m_font = std::make_unique<SpriteFont>(device, resourceUpload,
        strFilePath,
        m_resourceDescriptors->GetCpuHandle(Descriptors::TextFont),
        m_resourceDescriptors->GetGpuHandle(Descriptors::TextFont));

    DX::FindMediaFile(strFilePath, MAX_PATH, L"XboxOneControllerLegendSmall.spritefont");
    m_ctrlFont = std::make_unique<SpriteFont>(device, resourceUpload,
        strFilePath,
        m_resourceDescriptors->GetCpuHandle(Descriptors::ControllerFont),
        m_resourceDescriptors->GetGpuHandle(Descriptors::ControllerFont));

    {
        RenderTargetState rtState(p_deviceResources->GetBackBufferFormat(), p_deviceResources->GetDepthBufferFormat());

        m_help->RestoreDevice(device, resourceUpload, rtState);
    }

    auto finish = resourceUpload.End(p_deviceResources->GetCommandQueue());
    finish.wait();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::CreateWindowSizeDependentResources(DX::DeviceResources_12* p_deviceResources)
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute_12::CreateWindowSizeDependentResources()" << std::endl;

    auto size = p_deviceResources->GetOutputSize();
    m_help->SetWindow(size);

    auto viewport = p_deviceResources->GetScreenViewport();
    m_spriteBatch->SetViewport(viewport);
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::ResetResources()
{
    GetLogStream(SeverityInfo)
        << "AppStateSimpleCompute_12::ResetRessources()" << std::endl;

    m_spriteBatch.reset();
    m_font.reset();
    m_ctrlFont.reset();
    m_resourceDescriptors.reset();

    m_computePSO.Reset();
    m_computeRootSignature.Reset();
    m_computeAllocator.Reset();
    m_computeCommandQueue.Reset();
    m_computeCommandList.Reset();

    m_computeFenceEvent.Close();

    m_computeFence.Reset();
    m_renderResourceFence.Reset();

    m_fractalColorMap[0].Reset();
    m_fractalColorMap[1].Reset();
    m_fractalTexture[0].Reset();
    m_fractalTexture[1].Reset();

    m_renderHeap.Reset();
    m_computeHeap.Reset();

    m_SRVDescriptorHeap.reset();
    m_samplerDescriptorHeap.reset();

    m_help->ReleaseDevice();

    m_graphicsMemory.reset();
}

//-----------------------------------------------------------------------------

void AppStateSimpleCompute_12::ResetWindow()
{
    m_window = XMFLOAT4(4.0f, 2.25f, -0.65f, 0.0f);
    m_windowUpdated = true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name: UpdateFractalData
// Desc: Updates the dynamic constant buffer with fractal data
//-----------------------------------------------------------------------------
void AppStateSimpleCompute_12::UpdateFractalData(DX::DeviceResources_12* p_deviceResources)
{
    (void*)p_deviceResources;

    const D3D12_RESOURCE_DESC texDesc = m_fractalTexture[0]->GetDesc();

    SharedGraphicsResource *pUploadHeap = m_usingAsyncCompute ? &m_computeHeap : &m_renderHeap;

    auto pCBFractalData = reinterpret_cast<CB_FractalCS*> (pUploadHeap->Memory());

    pCBFractalData->MaxThreadIter = XMFLOAT4(static_cast<float>(texDesc.Width), static_cast<float>(texDesc.Height), static_cast<float>(m_fractalMaxIterations), 0);
    pCBFractalData->Window = m_window;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Name: EnsureResourceState
// Desc: Ensures the fractal texture is in the desired resource state
//------------------------------------------------------------------------------
_Use_decl_annotations_
bool AppStateSimpleCompute_12::EnsureResourceState( 
    uint32_t index, D3D12_RESOURCE_STATES afterState, 
    DX::DeviceResources_12* p_deviceResources)
{
    if (m_resourceStateFractalTexture[index] != afterState)
    {
        auto commandList = p_deviceResources->GetCommandList();

        ResourceBarrier(commandList, m_fractalTexture[index].Get(), m_resourceStateFractalTexture[index], afterState);
        m_resourceStateFractalTexture[index] = afterState;
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void AppStateSimpleCompute_12::AsyncComputeThreadProc(DX::DeviceResources_12* p_deviceResources)
{
    LARGE_INTEGER PerfFreq;
    QueryPerformanceFrequency(&PerfFreq);

    LARGE_INTEGER LastFrameTime;
    QueryPerformanceCounter(&LastFrameTime);

    while (!m_terminateThread)
    {
        LARGE_INTEGER CurrentFrameTime;
        QueryPerformanceCounter(&CurrentFrameTime);
        double DeltaTime = (double)(CurrentFrameTime.QuadPart - LastFrameTime.QuadPart) / (double)PerfFreq.QuadPart;
        LastFrameTime = CurrentFrameTime;

        if (m_usingAsyncCompute)
        {
            if (m_windowUpdated)
            {
                while (true)
                {
                    if (m_resourceState[ComputeIndex()] == ResourceState_Switching)						// render kicked off a resource switch to unordered,
                    {																					// check the fence for completed for quickest turn around
                        if (m_renderResourceFence->GetCompletedValue() >= m_renderResourceFenceValue)	// render might also check first and switch the state to ready compute
                        {
                            m_resourceState[ComputeIndex()] = ResourceState_ReadyCompute;
                            break;
                        }
                    }
                    if (m_resourceState[ComputeIndex()] == ResourceState_ReadyCompute)					// render detected compute buffer switched to unordered access first
                    {
                        break;
                    }
                    if (!m_usingAsyncCompute)															// user has request synchronous compute
                    {
                        break;
                    }
                }
                if (!m_usingAsyncCompute)																// user has request synchronous compute
                {
                    continue;
                }

                m_computeFPS.Tick(static_cast<FLOAT>(DeltaTime));

                UpdateFractalData(p_deviceResources);

                // setup the asynchronous compute command list, use a unique command list
                PIXBeginEvent(m_computeCommandList.Get(), !m_renderIndex ? PIX_COLOR(0, 0, 255) : PIX_COLOR(0, 255, 0), "Compute");

                ID3D12DescriptorHeap* pHeaps[] = { m_SRVDescriptorHeap->Heap(), m_samplerDescriptorHeap->Heap() };
                m_computeCommandList->SetDescriptorHeaps(_countof(pHeaps), pHeaps);

                m_computeCommandList->SetComputeRootSignature(m_computeRootSignature.Get());

                m_computeCommandList->SetComputeRootConstantBufferView(e_rootParameterCB, m_computeHeap.GpuAddress());
                m_computeCommandList->SetComputeRootDescriptorTable(e_rootParameterSampler, m_samplerDescriptorHeap->GetGpuHandle(0));
                m_computeCommandList->SetComputeRootDescriptorTable(e_rootParameterSRV, m_SRVDescriptorHeap->GetGpuHandle(e_iSRV + 3));					// rainbow sampler
                m_computeCommandList->SetComputeRootDescriptorTable(e_rootParameterUAV, m_SRVDescriptorHeap->GetGpuHandle(e_iUAV + ComputeIndex()));

                m_computeCommandList->SetPipelineState(m_computePSO.Get());
                m_computeCommandList->Dispatch(m_ThreadGroupX, m_ThreadGroupY, 1);

                PIXEndEvent(m_computeCommandList.Get());

                // close and execute the command list
                m_computeCommandList->Close();
                ID3D12CommandList *tempList = m_computeCommandList.Get();
                m_computeCommandQueue->ExecuteCommandLists(1, &tempList);

                const uint64_t fence = m_computeFenceValue++;
                m_computeCommandQueue->Signal(m_computeFence.Get(), fence);
                if (m_computeFence->GetCompletedValue() < fence)								// block until async compute has completed using a fence
                {
                    m_computeFence->SetEventOnCompletion(fence, m_computeFenceEvent.Get());
                    WaitForSingleObject(m_computeFenceEvent.Get(), INFINITE);
                }
                m_resourceState[ComputeIndex()] = ResourceState_Computed;						// signal the buffer is now ready for render thread to use

                m_computeAllocator->Reset();
                m_computeCommandList->Reset(m_computeAllocator.Get(), m_computePSO.Get());
            }
            else
            {
                SwitchToThread();
            }
        }
        else
        {
            SwitchToThread();
        }
    }
}

} // namespace nxn
