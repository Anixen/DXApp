/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleLighting_11
 *
 *
 * @file src/AppStateSimpleLighting_11.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_LIGHTING_11
#define NXN_APP_STATE_SIMPLE_LIGHTING_11


#include "AppState_11.h"

namespace nxn {


class AppStateSimpleLighting_11 : public AppState_11 {

public:
	                                                    AppStateSimpleLighting_11           (App_11* p_app);
	                                                     ~AppStateSimpleLighting_11         ();

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

    // SimpleLighting objects
            Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
            Microsoft::WRL::ComPtr<ID3D11Buffer>        m_vertexBuffer;
            Microsoft::WRL::ComPtr<ID3D11Buffer>        m_indexBuffer;
            Microsoft::WRL::ComPtr<ID3D11Buffer>        m_constantBuffer;

            Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vertexShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShaderSolid;

    // Scene constants, updated per-frame
            float                                       m_curRotationAngleRad   = 0.f;

    // These computed values will be loaded into a ConstantBuffer during Render
            DirectX::XMFLOAT4X4                         m_worldMatrix;
            DirectX::XMFLOAT4X4                         m_viewMatrix;
            DirectX::XMFLOAT4X4                         m_projectionMatrix;
            DirectX::XMFLOAT4                           m_lightDirs[2];
            DirectX::XMFLOAT4                           m_lightColors[2];
            DirectX::XMFLOAT4                           m_outputColor;

}; // class AppStateSimpleLighting_11

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_LIGHTING_11
