/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleTexture_12
 *
 *
 * @file src/AppStateSimpleTexture_12.h
 * @author Olivier Falconnet
 * @date 20211023 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_TEXTURE_12
#define NXN_APP_STATE_SIMPLE_TEXTURE_12


#include "AppState_12.h"

namespace nxn {


class AppStateSimpleTexture_12 : public AppState_12 {

public:
	                                                        AppStateSimpleTexture_12            (App_12* p_app);
	                                                        ~AppStateSimpleTexture_12           ();

	virtual void                                            Init                                ();
	virtual void                                            Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual AppState_12*                                    Update                              (   DX::DeviceResources_12* p_deviceResources,
                                                                                                    DX::StepTimer const& p_timer,
                                                                                                    bool & p_gamePadConnected,
                                                                                                    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                            Draw                                (DX::DeviceResources_12* p_deviceResources);
    virtual void                                            Clear                               (DX::DeviceResources_12* p_deviceResources);

protected:
	virtual void                                            HandleCleanup                       ();

private:
    virtual void                                            CreateDeviceDependentResources      (DX::DeviceResources_12* p_deviceResources);
    virtual void                                            CreateWindowSizeDependentResources  (DX::DeviceResources_12* p_deviceResources);
    virtual void                                            ResetResources                      ();

    // SimpleTexture_12 objects
    // DirectXTK objects.
            std::unique_ptr<DirectX::GraphicsMemory>        m_graphicsMemory;

    // Direct3D 12 objects
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>    m_srvHeap;
            Microsoft::WRL::ComPtr<ID3D12RootSignature>     m_rootSignature;
            Microsoft::WRL::ComPtr<ID3D12PipelineState>     m_pipelineState;
            Microsoft::WRL::ComPtr<ID3D12Resource>          m_vertexBuffer;
            Microsoft::WRL::ComPtr<ID3D12Resource>          m_indexBuffer;
            Microsoft::WRL::ComPtr<ID3D12Resource>          m_texture;
            D3D12_VERTEX_BUFFER_VIEW                        m_vertexBufferView;
            D3D12_INDEX_BUFFER_VIEW                         m_indexBufferView;

}; // class AppStateSimpleTexture_12

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_TEXTURE_12
