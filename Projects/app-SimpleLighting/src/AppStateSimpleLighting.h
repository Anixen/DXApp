/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleLighting
 *
 *
 * @file src/AppStateSimpleLighting.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_LIGHTING
#define NXN_APP_STATE_SIMPLE_LIGHTING


#include "../../lib-DXApp/src/AppState.h"

namespace nxn {


class AppStateSimpleLighting : public AppState {

public:
	                                            AppStateSimpleLighting              (App* p_app);
	                                            ~AppStateSimpleLighting             ();

	virtual void                                Init                                ();
	virtual void                                Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState*                           Update                              (   DX::DeviceResources* p_deviceResources,
                                                                                        DX::StepTimer const& p_timer,
                                                                                        bool & p_ctrlConnected,
                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                Draw                                (DX::DeviceResources* p_deviceResources);
    virtual void                                Clear                               (DX::DeviceResources* p_deviceResources);

protected:
	virtual void                                HandleCleanup                       ();

private:
    virtual void                                CreateDeviceDependentResources      (DX::DeviceResources * p_deviceResources);
    virtual void                                CreateWindowSizeDependentResources  (DX::DeviceResources * p_deviceResources);
    virtual void                                ResetResources                      ();

    Microsoft::WRL::ComPtr<ID3D11InputLayout>   m_inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        m_indexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>        m_constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_pixelShaderSolid;

    float                                       m_curRotationAngleRad   = 0.f;

    // These computed values will be loaded into a ConstantBuffer during Render
    DirectX::XMFLOAT4X4                         m_worldMatrix;
    DirectX::XMFLOAT4X4                         m_viewMatrix;
    DirectX::XMFLOAT4X4                         m_projectionMatrix;
    DirectX::XMFLOAT4                           m_lightDirs[2];
    DirectX::XMFLOAT4                           m_lightColors[2];
    DirectX::XMFLOAT4                           m_outputColor;

}; // class AppStateSimpleLighting

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_LIGHTING
