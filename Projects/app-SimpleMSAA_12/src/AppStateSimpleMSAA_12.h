/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleMSAA_12
 *
 *
 * @file src/AppStateSimpleMSAA_12.h
 * @author Olivier Falconnet
 * @date 20211120 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_MSAA_12
#define NXN_APP_STATE_SIMPLE_MSAA_12


#include "AppState_12.h"

namespace nxn {

class AppStateSimpleMSAA_12 : public AppState_12 {

public:
	                                                                            AppStateSimpleMSAA_12               (App_12* p_app);
	                                                                            ~AppStateSimpleMSAA_12              ();

	virtual             void                                                    Init                                ();
	virtual             void                                                    Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual             AppState_12*                                            Update                              (   DX::DeviceResources_12* p_deviceResources,
                                                                                                                        DX::StepTimer const& p_timer,
                                                                                                                        bool & p_gamePadConnected,
                                                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual             void                                                    Draw                                (DX::DeviceResources_12* p_deviceResources);
    virtual             void                                                    Clear                               (DX::DeviceResources_12* p_deviceResources);

protected:
	virtual             void                                                    HandleCleanup                       ();

private:
    virtual             void                                                    CreateDeviceDependentResources      (DX::DeviceResources_12* p_deviceResources);
    virtual             void                                                    CreateWindowSizeDependentResources  (DX::DeviceResources_12* p_deviceResources);
    virtual             void                                                    ResetResources                      ();

    // SimpleMSAA_12 objects

    // MSAA resources.
                        Microsoft::WRL::ComPtr<ID3D12Resource>                  m_msaaRenderTarget;
                        Microsoft::WRL::ComPtr<ID3D12Resource>                  m_msaaDepthStencil;

                        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>            m_msaaRTVDescriptorHeap;
                        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>            m_msaaDSVDescriptorHeap;

            unsigned    int                                                     m_sampleCount               = 0;
                        bool                                                    m_msaa                      = false;

    // DirectXTK objects.
                        std::unique_ptr<DirectX::GraphicsMemory>                m_graphicsMemory;
                        std::unique_ptr<DirectX::SpriteBatch>                   m_batch;

                        std::unique_ptr<DirectX::DescriptorHeap>                m_resourceDescriptors;

    enum Descriptors
    {
        UIFont,
        CtrlFont,
        Count
    };

                        std::unique_ptr<DirectX::SpriteFont>                    m_smallFont;
                        std::unique_ptr<DirectX::SpriteFont>                    m_ctrlFont;

                        std::unique_ptr<DirectX::CommonStates>                  m_states;
                        std::unique_ptr<DirectX::Model>                         m_model;
                        std::unique_ptr<DirectX::EffectTextureFactory>          m_modelResources;
                        std::unique_ptr<DirectX::IEffectFactory>                m_fxFactory;

                        std::vector<std::shared_ptr<DirectX::IEffect>>          m_modelMSAA;
                        std::vector<std::shared_ptr<DirectX::IEffect>>          m_modelStandard;

                        DirectX::SimpleMath::Matrix                             m_world;
                        DirectX::SimpleMath::Matrix                             m_view;
                        DirectX::SimpleMath::Matrix                             m_proj;

}; // class AppStateSimpleMSAA_12

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_MSAA_12
