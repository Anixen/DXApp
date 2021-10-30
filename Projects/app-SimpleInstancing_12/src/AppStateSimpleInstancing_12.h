/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleInstancing_12
 *
 *
 * @file src/AppStateSimpleInstancing_12.h
 * @author Olivier Falconnet
 * @date 20211027 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_INSTANCING_12
#define NXN_APP_STATE_SIMPLE_INSTANCING_12


#include "AppState_12.h"
#include "Shaders/SimpleInstancing_12_Shared.h"

namespace nxn {

class AppStateSimpleInstancing_12 : public AppState_12 {

public:
	                                                                        AppStateSimpleInstancing_12         (App_12* p_app);
	                                                                        ~AppStateSimpleInstancing_12        ();

	virtual         void                                                    Init                                ();
	virtual         void                                                    Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual         AppState_12*                                            Update                              (   DX::DeviceResources_12* p_deviceResources,
                                                                                                                    DX::StepTimer const& p_timer,
                                                                                                                    bool & p_gamePadConnected,
                                                                                                                    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                                    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                                    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual         void                                                    Draw                                (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                                    Clear                               (DX::DeviceResources_12* p_deviceResources);

protected:
	virtual         void                                                    HandleCleanup                       ();

private:
    virtual         void                                                    CreateDeviceDependentResources      (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                                    CreateWindowSizeDependentResources  (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                                    ResetResources                      ();

                    void                                                    ResetSimulation                     ();
                    float                                                   FloatRand                           (float lowerBound = -1.0f, float upperBound = 1.0f);

    // SimpleInstancing_12 objects

    enum Descriptors
    {
        TextFont,
        ControllerFont,
        Count
    };

    // Instance vertex definition
    struct Instance
    {
        DirectX::XMFLOAT4 quaternion;
        DirectX::XMFLOAT4 positionAndScale;
    };

    // Light data structure (maps to constant buffer in pixel shader)
    struct Lights
    {
        DirectX::XMFLOAT4 directional;
        DirectX::XMFLOAT4 pointPositions[c_pointLightCount];
        DirectX::XMFLOAT4 pointColors[c_pointLightCount];
    };

    // DirectXTK objects.
                    std::unique_ptr<DirectX::GraphicsMemory>                m_graphicsMemory;

                    std::unique_ptr<DirectX::DescriptorHeap>                m_resourceDescriptors;
                    std::unique_ptr<DirectX::SpriteBatch>                   m_batch;
                    std::unique_ptr<DirectX::SpriteFont>                    m_smallFont;
                    std::unique_ptr<DirectX::SpriteFont>                    m_ctrlFont;

    // Direct3D 12 pipeline objects.
                    Microsoft::WRL::ComPtr<ID3D12RootSignature>             m_rootSignature;
                    Microsoft::WRL::ComPtr<ID3D12PipelineState>             m_pipelineState;

    // Direct3D 12 resources.
                    Microsoft::WRL::ComPtr<ID3D12Resource>                  m_vertexBuffer;
                    D3D12_VERTEX_BUFFER_VIEW                                m_vertexBufferView[3];
                    Microsoft::WRL::ComPtr<ID3D12Resource>                  m_indexBuffer;
                    D3D12_INDEX_BUFFER_VIEW                                 m_indexBufferView;
                    Microsoft::WRL::ComPtr<ID3D12Resource>                  m_boxColors;

                    Microsoft::WRL::ComPtr<ID3D12Resource>                  m_instanceData;
                    uint8_t*                                                m_mappedInstanceData;
                    D3D12_GPU_VIRTUAL_ADDRESS                               m_instanceDataGpuAddr;

                    // A synchronization fence and an event. These members will be used
                    // to synchronize the CPU with the GPU so that there will be no
                    // contention for the instance data. 
                    Microsoft::WRL::ComPtr<ID3D12Fence>                     m_fence;
                    Microsoft::WRL::Wrappers::Event                         m_fenceEvent;

    struct aligned_deleter { void operator()(void* p) { _aligned_free(p); } };

                    std::unique_ptr<Instance[]>                             m_CPUInstanceData;
                    std::unique_ptr<DirectX::XMVECTOR[], aligned_deleter>   m_rotationQuaternions;
                    std::unique_ptr<DirectX::XMVECTOR[], aligned_deleter>   m_velocities;
                    uint32_t                                                m_usedInstanceCount;

                    DirectX::XMFLOAT4X4                                     m_proj;
                    DirectX::XMFLOAT4X4                                     m_clip;
                    Lights                                                  m_lights;
                    float                                                   m_pitch;
                    float                                                   m_yaw;

                    std::default_random_engine                              m_randomEngine;

}; // class AppStateSimpleInstancing_12

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_INSTANCING_12
