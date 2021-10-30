/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleLighting_12
 *
 *
 * @file src/AppStateSimpleLighting_12.h
 * @author Olivier Falconnet
 * @date 20211025 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_LIGHTING_12
#define NXN_APP_STATE_SIMPLE_LIGHTING_12


#include "AppState_12.h"

namespace nxn {


class AppStateSimpleLighting_12 : public AppState_12 {

public:
	                                                            AppStateSimpleLighting_12           (App_12* p_app);
	                                                            ~AppStateSimpleLighting_12          ();

	virtual         void                                        Init                                ();
	virtual         void                                        Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual         AppState_12*                                Update                              (   DX::DeviceResources_12* p_deviceResources,
                                                                                                        DX::StepTimer const& p_timer,
                                                                                                        bool & p_gamePadConnected,
                                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual         void                                        Draw                                (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        Clear                               (DX::DeviceResources_12* p_deviceResources);

protected:
	virtual         void                                        HandleCleanup                       ();

private:
    virtual         void                                        CreateDeviceDependentResources      (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        CreateWindowSizeDependentResources  (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        ResetResources                      ();


    struct ConstantBuffer
    {
        DirectX::XMMATRIX worldMatrix;
        DirectX::XMMATRIX viewMatrix;
        DirectX::XMMATRIX projectionMatrix;
        DirectX::XMVECTOR lightDir[2];
        DirectX::XMVECTOR lightColor[2];
        DirectX::XMVECTOR outputColor;
    };

    // We'll allocate space for several of these and they will need to be padded for alignment.
    static_assert(sizeof(ConstantBuffer) == 272, "Checking the size here.");

    // D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT < 272 < 2 * D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT
    // Create a union with the correct size and enough room for one ConstantBuffer
    union PaddedConstantBuffer
    {
        ConstantBuffer constants;
        uint8_t bytes[2 * D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT];
    };

    // Check the exact size of the PaddedConstantBuffer to make sure it will align properly
    static_assert(sizeof(PaddedConstantBuffer) == 2 * D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT, "PaddedConstantBuffer is not aligned properly");


    // SimpleLighting_12 objects
    // DirectXTK objects.
                    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
                    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_lambertPipelineState;
                    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_solidColorPipelineState;
                    Microsoft::WRL::ComPtr<ID3D12Resource>      m_vertexBuffer;
                    Microsoft::WRL::ComPtr<ID3D12Resource>      m_indexBuffer;
                    Microsoft::WRL::ComPtr<ID3D12Resource>      m_perFrameConstants;
                    PaddedConstantBuffer*                       m_mappedConstantData;
                    D3D12_GPU_VIRTUAL_ADDRESS                   m_constantDataGpuAddr;
                    D3D12_VERTEX_BUFFER_VIEW                    m_vertexBufferView;
                    D3D12_INDEX_BUFFER_VIEW                     m_indexBufferView;

    // In this simple sample, we know that there are three draw calls
    // and we will update the scene constants for each draw call.
    static  const   unsigned int                                c_numDrawCalls = 3;

    // A synchronization fence and an event. These members will be used
    // to synchronize the CPU with the GPU so that there will be no
    // contention for the constant buffers. 
                    Microsoft::WRL::ComPtr<ID3D12Fence>         m_fence;
                    Microsoft::WRL::Wrappers::Event             m_fenceEvent;

    // Index in the root parameter table
    static  const   UINT                                        c_rootParameterCB = 0;

    // Scene constants, updated per-frame
                    float                                       m_curRotationAngleRad;

    // These computed values will be loaded into a ConstantBuffer
    // during Render
                    DirectX::XMFLOAT4X4                         m_worldMatrix;
                    DirectX::XMFLOAT4X4                         m_viewMatrix;
                    DirectX::XMFLOAT4X4                         m_projectionMatrix;
                    DirectX::XMFLOAT4                           m_lightDirs[2];
                    DirectX::XMFLOAT4                           m_lightColors[2];
                    DirectX::XMFLOAT4                           m_outputColor;

}; // class AppStateSimpleLighting_12

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_LIGHTING_12
