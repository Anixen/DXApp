/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleInstancing_11
 *
 *
 * @file src/AppStateSimpleInstancing_11.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_INSTANCING_11
#define NXN_APP_STATE_SIMPLE_INSTANCING_11


#include "AppState_11.h"
#include "Shaders/SimpleInstancing_11_Shared.h"

namespace nxn {

class AppStateSimpleInstancing_11 : public AppState_11 {

public:
	                                                                AppStateSimpleInstancing_11         (App_11* p_app);
	                                                                ~AppStateSimpleInstancing_11        ();

	virtual void                                                    Init                                ();
	virtual void                                                    Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState_11*                                            Update                              (   DX::DeviceResources_11* p_deviceResources,
                                                                                                            DX::StepTimer const& p_timer,
                                                                                                            bool & p_gamePadConnected,
                                                                                                            DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                            DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                            DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                                    Draw                                (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                    Clear                               (DX::DeviceResources_11* p_deviceResources);

protected:
	virtual void                                                    HandleCleanup                       ();

private:
    virtual void                                                    CreateDeviceDependentResources      (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                    CreateWindowSizeDependentResources  (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                    ResetResources                      ();

            void                                                    ReplaceBufferContents               (DX::DeviceResources_11* p_deviceResources, ID3D11Buffer* buffer, size_t bufferSize, const void* data);
            void                                                    ResetSimulation();

            float                                                   FloatRand                           (float lowerBound = -1.0f, float upperBound = 1.0f);

    // SimpleInstancing_11 objects

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

            std::unique_ptr<DirectX::SpriteBatch>                   m_batch;
            std::unique_ptr<DirectX::SpriteFont>                    m_smallFont;
            std::unique_ptr<DirectX::SpriteFont>                    m_ctrlFont;

            Microsoft::WRL::ComPtr<ID3D11InputLayout>               m_inputLayout;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_vertexBuffer;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_indexBuffer;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_instanceData;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_boxColors;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_vertexConstants;
            Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_pixelConstants;
            Microsoft::WRL::ComPtr<ID3D11VertexShader>              m_vertexShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>               m_pixelShader;

    struct aligned_deleter { void operator()(void* p) { _aligned_free(p); } };

            std::unique_ptr<Instance[]>                             m_CPUInstanceData;
            std::unique_ptr<DirectX::XMVECTOR[], aligned_deleter>   m_rotationQuaternions;
            std::unique_ptr<DirectX::XMVECTOR[], aligned_deleter>   m_velocities;
            uint32_t                                                m_usedInstanceCount;

            DirectX::XMFLOAT4X4                                     m_proj;
            Lights                                                  m_lights    = {};
            float                                                   m_pitch     = 0.f;
            float                                                   m_yaw       = 0.f;      

            std::default_random_engine                              m_randomEngine;

}; // class AppStateSimpleInstancing_11

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_INSTANCING_11
