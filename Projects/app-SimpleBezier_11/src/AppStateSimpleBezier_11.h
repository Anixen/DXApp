/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleBezier_11
 *
 *
 * @file src/AppStateSimpleBezier_11.h
 * @author Olivier Falconnet
 * @date 20211004 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_BEZIER
#define NXN_APP_STATE_SIMPLE_BEZIER


#include "AppState_11.h"
#include "ControllerHelp.h"

namespace nxn {


class AppStateSimpleBezier_11 : public AppState_11 {

public:
	                                                        AppStateSimpleBezier_11             (App_11* p_app);
	                                                        ~AppStateSimpleBezier_11            ();

	virtual void                                            Init                                ();
	virtual void                                            Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState_11*                                    Update                              (   DX::DeviceResources_11* p_deviceResources,
                                                                                                    DX::StepTimer const& p_timer,
                                                                                                    bool & p_gamePadConnected,
                                                                                                    DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                    DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                    DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                            Draw                                (DX::DeviceResources_11* p_deviceResources);
    virtual void                                            Clear                               (DX::DeviceResources_11* p_deviceResources);

protected:
	virtual void                                            HandleCleanup                       ();

private:
            void                                            CreateShaders                       (DX::DeviceResources_11* p_deviceResources);
    virtual void                                            CreateDeviceDependentResources      (DX::DeviceResources_11* p_deviceResources);
    virtual void                                            CreateWindowSizeDependentResources  (DX::DeviceResources_11* p_deviceResources);
    virtual void                                            ResetResources                      ();

    // SimpleBezier objects
    struct ConstantBuffer
    {
        DirectX::XMFLOAT4X4 viewProjectionMatrix;
        DirectX::XMFLOAT3   cameraWorldPos;
        float               tessellationFactor;
    };

    enum class PartitionMode
    {
        PartitionInteger,
        PartitionFractionalEven,
        PartitionFractionalOdd
    };

            std::unique_ptr<DirectX::CommonStates>          m_states;

            Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
            Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
            Microsoft::WRL::ComPtr<ID3D11HullShader>        m_hullShaderInteger;
            Microsoft::WRL::ComPtr<ID3D11HullShader>        m_hullShaderFracEven;
            Microsoft::WRL::ComPtr<ID3D11HullShader>        m_hullShaderFracOdd;
            Microsoft::WRL::ComPtr<ID3D11DomainShader>      m_domainShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixelShader;
            Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_solidColorPS;

            Microsoft::WRL::ComPtr<ID3D11Buffer>            m_controlPointVB;     // Control points for mesh
            Microsoft::WRL::ComPtr<ID3D11Buffer>            m_cbPerFrame;

            Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerStateSolid;
            Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerStateWireframe;

    // Control variables
            float                                           m_subdivs;
            bool                                            m_drawWires;
            PartitionMode                                   m_partitionMode     = PartitionMode::PartitionInteger;

            DirectX::XMFLOAT4X4                             m_worldMatrix;
            DirectX::XMFLOAT4X4                             m_viewMatrix;
            DirectX::XMFLOAT4X4                             m_projectionMatrix;
            DirectX::XMFLOAT3                               m_cameraEye;

    // Legend and help UI
            std::unique_ptr<DirectX::SpriteBatch>           m_batch;
            std::unique_ptr<DirectX::SpriteFont>            m_smallFont;
            std::unique_ptr<DirectX::SpriteFont>            m_ctrlFont;

            std::unique_ptr<ATG::Help>                      m_help;
            bool                                            m_showHelp          = false;

}; // class AppStateSimpleBezier_11

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_BEZIER
