/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleTriangle_11
 *
 *
 * @file src/AppStateSimpleTriangle_11.h
 * @author Olivier Falconnet
 * @date 20211003 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_TRIANGLE
#define NXN_APP_STATE_SIMPLE_TRIANGLE


#include "AppState_11.h"

namespace nxn {


class AppStateSimpleTriangle_11 : public AppState_11 {

public:
	                                                    AppStateSimpleTriangle_11           (App_11* p_app);
	                                                    ~AppStateSimpleTriangle_11          ();

	virtual void                                        Init                                ();
	virtual void                                        Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual AppState_11*                                Update                              (   DX::DeviceResources_11* p_deviceResources,
                                                                                                DX::StepTimer const& p_timer,
                                                                                                bool & p_gamePadConnected,
                                                                                                DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                        Draw                                (DX::DeviceResources_11* p_deviceResources);
    virtual void                                        Clear                               (DX::DeviceResources_11* p_deviceResources);

protected:
	virtual void                                        HandleCleanup                       ();

private:
    virtual void                                        CreateDeviceDependentResources      (DX::DeviceResources_11* p_deviceResources);
    virtual void                                        CreateWindowSizeDependentResources  (DX::DeviceResources_11* p_deviceResources);
    virtual void                                        ResetResources                      ();

    // SimpleTriangle objects
            Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_spInputLayout;
            Microsoft::WRL::ComPtr<ID3D11Buffer>        m_spVertexBuffer;
            Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_spVertexShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_spPixelShader;

}; // class AppStateSimpleTriangle_11

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_TRIANGLE
