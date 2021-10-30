/**
 * 
 * An application state is responsible for holding relevant information, and providing methods needed for the main loop.
 *
 * @class AppStateSimpleCompute_11
 *
 *
 * @file src/AppStateSimpleCompute_11.h
 * @author Olivier Falconnet
 * @date 20211005 - File creation
 */

#ifndef NXN_APP_STATE_SIMPLE_COMPUTE_11
#define NXN_APP_STATE_SIMPLE_COMPUTE_11


#include "AppState_11.h"
#include "ControllerHelp.h"

namespace nxn {

class SmoothedFPS {

public:
                SmoothedFPS(uint32_t frameInterval = 100)
    {
        Initialize(frameInterval);
    }

    void        Initialize(uint32_t frameInterval = 100)
    {
        m_frameInterval = frameInterval;
        m_timeAccumulator = 0.0f;
        m_frameAccumulator = 0;
        m_smoothedFPS = 0.0f;
    }

    void        Tick(float DeltaTime)
    {
        m_timeAccumulator += DeltaTime;
        ++m_frameAccumulator;

        if (m_frameAccumulator >= m_frameInterval)
        {
            m_smoothedFPS = (float)m_frameInterval / m_timeAccumulator;
            m_timeAccumulator = 0.0f;
            m_frameAccumulator = 0;
        }
    }

    float       GetFPS() const { return m_smoothedFPS; }

private:
    float       m_smoothedFPS;
    float       m_timeAccumulator;
    uint32_t    m_frameAccumulator;
    uint32_t    m_frameInterval;

};


class AppStateSimpleCompute_11 : public AppState_11 {

public:
	                                                            AppStateSimpleCompute_11            (App_11* p_app);
	                                                            ~AppStateSimpleCompute_11           ();

	virtual void                                                Init                                ();
	virtual void                                                Reinit                              ();  // Called to reset the state. Re-Initializes a state without reallocating everything inside.

    /**
     * @param {StepTimer const&}   p_timer  The StepTimer that keeping track elapsed duration since last update
     *
     * @return {AppState*} A pointer to a new app state, or nullptr if no change required
     */
	virtual AppState_11*                                        Update                              (   DX::DeviceResources_11* p_deviceResources,
                                                                                                        DX::StepTimer const& p_timer,
                                                                                                        bool & p_gamePadConnected,
                                                                                                        DirectX::GamePad* p_gamePad,    DirectX::GamePad::ButtonStateTracker & p_gamePadButtons,
                                                                                                        DirectX::Keyboard* p_keyboard,  DirectX::Keyboard::KeyboardStateTracker & p_keyboardButtons,
                                                                                                        DirectX::Mouse* p_mouse,        DirectX::Mouse::ButtonStateTracker & p_mouseButtons);

	virtual void                                                Draw                                (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                Clear                               (DX::DeviceResources_11* p_deviceResources);

protected:
	virtual void                                                HandleCleanup                       ();

private:
    virtual void                                                CreateDeviceDependentResources      (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                CreateWindowSizeDependentResources  (DX::DeviceResources_11* p_deviceResources);
    virtual void                                                ResetResources                      ();

            void                                                ResetWindow                         ();
            void                                                UpdateFractalData                   (DX::DeviceResources_11* p_deviceResources);

    // SimpleCompute_11 objects
    // on-screen help
            std::unique_ptr<ATG::Help>                          m_help;
            bool                                                m_showHelp;

    // Compute data
            SmoothedFPS                                         m_renderFPS;

            uint64_t*                                           m_fractalTimestamps;
            DirectX::XMFLOAT4                                   m_window;
            bool                                                m_windowUpdated;
            uint32_t                                            m_fractalMaxIterations;

            Microsoft::WRL::ComPtr<ID3D11Buffer>                m_cbFractal;
            Microsoft::WRL::ComPtr<ID3D11ComputeShader>	        m_csFractal;
            Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_fractalTexture;
            Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>   m_fractalUAV;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_fractalSRV;
            Microsoft::WRL::ComPtr<ID3D11Texture2D>             m_fractalColorMap;
            Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_fractalColorMapSRV;
            Microsoft::WRL::ComPtr<ID3D11SamplerState>          m_fractalBilinearSampler;

            // DirectXTK objects.
            std::unique_ptr<DirectX::SpriteBatch>               m_spriteBatch;
            std::unique_ptr<DirectX::SpriteFont>                m_font;
            std::unique_ptr<DirectX::SpriteFont>                m_ctrlFont;

    struct CB_FractalCS
    {
        DirectX::XMFLOAT4 MaxThreadIter;
        DirectX::XMFLOAT4 Window;
    };

}; // class AppStateSimpleCompute_11

} // namespace nxn


#endif // !NXN_APP_STATE_SIMPLE_COMPUTE_11
