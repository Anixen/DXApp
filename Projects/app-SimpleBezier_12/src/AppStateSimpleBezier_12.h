/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleBezier_12
 *
 *
 * @file src/AppStateSimpleBezier_12.h
 * @author Olivier Falconnet
 * @date 20211025 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_BEZIER_12
#define NXN_APP_STATE_SIMPLE_BEZIER_12


#include "AppState_12.h"
#include "ControllerHelp.h"

namespace nxn {


class AppStateSimpleBezier_12 : public AppState_12 {

public:
	                                                            AppStateSimpleBezier_12             (App_12* p_app);
	                                                            ~AppStateSimpleBezier_12            ();

	virtual         void                                        Init                                ();
	virtual         void                                        Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
    virtual         AppState_12*                                Update                              (   DX::DeviceResources_12* p_deviceResources,
                                                                                                        DX::StepTimer const& p_timer,
                                                                                                        bool & p_gamePadConnected,
                                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual         void                                        Draw                                (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        Clear                               (DX::DeviceResources_12* p_deviceResources);

protected:
	virtual         void                                        HandleCleanup                       ();

private:
                    void                                        CreateShaders                       (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        CreateDeviceDependentResources      (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        CreateWindowSizeDependentResources  (DX::DeviceResources_12* p_deviceResources);
    virtual         void                                        ResetResources                      ();


    // SimpleBezier_12 objects
    // DirectXTK objects.
                    std::unique_ptr<DirectX::GraphicsMemory>    m_graphicsMemory;

    // Sample objects
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

                    static const size_t c_numPixelShaders = 2;
                    static const size_t c_numHullShaders = 3;

                    Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
                    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PSOs[c_numPixelShaders][c_numHullShaders];

                    std::unique_ptr<DirectX::DescriptorHeap>    m_resourceDescriptors;

                    D3D12_VERTEX_BUFFER_VIEW                    m_controlPointVBView;
                    Microsoft::WRL::ComPtr<ID3D12Resource>      m_controlPointVB;     // Control points for mesh
                    Microsoft::WRL::ComPtr<ID3D12Resource>      m_cbPerFrame;
                    ConstantBuffer*                             m_mappedConstantData;

    // Index in the root parameter table
                    static const UINT                           c_rootParameterCB = 0;

    // Control variables
                    float                                       m_subdivs;
                    bool                                        m_drawWires;
                    PartitionMode                               m_partitionMode;

                    DirectX::XMFLOAT4X4                         m_worldMatrix;
                    DirectX::XMFLOAT4X4                         m_viewMatrix;
                    DirectX::XMFLOAT4X4                         m_projectionMatrix;
                    DirectX::XMFLOAT3                           m_cameraEye;

    // Legend and help UI
                    std::unique_ptr<DirectX::DescriptorHeap>    m_fontDescriptors;
                    std::unique_ptr<DirectX::SpriteBatch>       m_batch;
                    std::unique_ptr<DirectX::SpriteFont>        m_smallFont;
                    std::unique_ptr<DirectX::SpriteFont>        m_ctrlFont;

                    std::unique_ptr<ATG::Help>                  m_help;
                    bool                                        m_showHelp;   

}; // class AppStateSimpleBezier_12

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_BEZIER
