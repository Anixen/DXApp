/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleTexture
 *
 *
 * @file src/AppStateSimpleTexture.h
 * @author Olivier Falconnet
 * @date 20210929 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_TEXTURE
#define NXN_APP_STATE_SIMPLE_TEXTURE


#include "../../lib-DXApp/src/AppState.h"

namespace nxn {


class AppStateSimpleTexture : public AppState {

public:
	                                                    AppStateSimpleTexture               (App* p_app);
	                                                    ~AppStateSimpleTexture              ();

	virtual void                                        Init                                ();
	virtual void                                        Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState*                                   Update                              (   DX::StepTimer const& p_timer,
                                                                                                DirectX::GamePad* p_gamePad, DirectX::Keyboard* p_keyboard, DirectX::Mouse* p_mouse);

	virtual void                                        Draw                                (DX::DeviceResources* p_deviceResources);
    virtual void                                        Clear                               (DX::DeviceResources* p_deviceResources);

protected:
	virtual void                                        HandleCleanup                       ();

private:
    virtual void                                        CreateDeviceDependentResources      (DX::DeviceResources * p_deviceResources);
    virtual void                                        CreateWindowSizeDependentResources  ();
    virtual void                                        ResetResources                      ();

    Microsoft::WRL::ComPtr<ID3D11InputLayout>           m_spInputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_spVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>                m_spIndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader>          m_spVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>           m_spPixelShader;
    Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_spSampler;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_spTexture;

}; // class AppStateSimpleTexture

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_TEXTURE
