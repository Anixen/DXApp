/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleMSAA
 *
 *
 * @file src/AppStateSimpleMSAA.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_MSAA
#define NXN_APP_STATE_SIMPLE_MSAA


#include "../../lib-DXApp/src/AppState.h"

namespace nxn {

class AppStateSimpleMSAA : public AppState {

public:
	                                                                    AppStateSimpleMSAA                  (App* p_app);
	                                                                    ~AppStateSimpleMSAA                 ();

	virtual             void                                            Init                                ();
	virtual             void                                            Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual             AppState*                                       Update                              (   DX::DeviceResources* p_deviceResources,
                                                                                                                        DX::StepTimer const& p_timer,
                                                                                                                        bool & p_ctrlConnected,
                                                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual             void                                            Draw                                (DX::DeviceResources* p_deviceResources);
    virtual             void                                            Clear                               (DX::DeviceResources* p_deviceResources);

protected:
	virtual             void                                            HandleCleanup                       ();

private:
    virtual             void                                            CreateDeviceDependentResources      (DX::DeviceResources * p_deviceResources);
    virtual             void                                            CreateWindowSizeDependentResources  (DX::DeviceResources * p_deviceResources);
    virtual             void                                            ResetResources                      ();

     
    // MSAA resources.
                        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_msaaRenderTarget;

                        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_msaaRenderTargetView;
                        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_msaaDepthStencilView;

            unsigned    int                                             m_sampleCount           = 0;
                        bool                                            m_msaa                  = false; 

    // DirectXTK objects.
                        std::unique_ptr<DirectX::SpriteBatch>           m_batch;
                        std::unique_ptr<DirectX::SpriteFont>            m_smallFont;
                        std::unique_ptr<DirectX::SpriteFont>            m_ctrlFont;

                        std::unique_ptr<DirectX::CommonStates>          m_states;
                        std::unique_ptr<DirectX::Model>                 m_model;
                        std::unique_ptr<DirectX::EffectFactory>         m_fxFactory;

                        DirectX::SimpleMath::Matrix                     m_world;
                        DirectX::SimpleMath::Matrix                     m_view;
                        DirectX::SimpleMath::Matrix                     m_proj;

}; // class AppStateSimpleMSAA

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_MSAA
