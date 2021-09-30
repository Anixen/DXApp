/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleTriangle
 *
 *
 * @file src/AppStateSimpleTriangle.h
 * @author Olivier Falconnet
 * @date 20210929 - File creation
 */

#pragma once

#include "AppState.h"


namespace nxn {


class AppStateSimpleTriangle : public AppState {

public:
	                                                        AppStateSimpleTriangle              (App* p_app);
	                                                        ~AppStateSimpleTriangle             ();

	virtual void                                            init                                ();
	virtual void                                            reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState*                                       update                              (DX::StepTimer const& p_timer);

	virtual void                                            draw                                (DX::DeviceResources* p_deviceResources);
    virtual void                                            clear                               (DX::DeviceResources* p_deviceResources);

protected:
	virtual void                                            handleCleanup                       ();

private:
    virtual void                                            createDeviceDependentResources      (DX::DeviceResources * p_deviceResources);
    virtual void                                            createWindowSizeDependentResources  ();
    virtual void                                            resetResources                      ();

            Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_spInputLayout;
            Microsoft::WRL::ComPtr<ID3D11Buffer>            m_spVertexBuffer;
            Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_spVertexShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_spPixelShader;

}; // class AppStateSimpleTriangle

} // namespace nxn
