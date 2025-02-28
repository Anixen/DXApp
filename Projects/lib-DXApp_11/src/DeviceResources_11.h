/**
 * A wrapper for the Direct3D 11 device and swapchain.
 *
 * @class DeviceRessources_11
 *
 * @file src/DeviceRessources_11.h
 * @author Chuck Walbourn
 * @date 20170809 - Initial Release
 *
 * @date 20211015 - Included to lib-DXApp_11 project
 */

#ifndef DX_DEVICE_RESSOURCES_11
#define DX_DEVICE_RESSOURCES_11


namespace DX
{
    // Provides an interface for an application that owns DeviceResources_11 to be notified of the device being lost or created.
    interface IDeviceNotify
    {
        virtual void OnDeviceLost() = 0;
        virtual void OnDeviceRestored() = 0;

    protected:
        ~IDeviceNotify() = default;
    };

    // Controls all the DirectX device resources.
    class DeviceResources_11
    {
    public:
        static const unsigned int c_FlipPresent = 0x1;
        static const unsigned int c_AllowTearing = 0x2;
        static const unsigned int c_EnableHDR = 0x4;

        DeviceResources_11(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
            DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
            UINT backBufferCount = 2,
            D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_10_0,
            unsigned int flags = c_FlipPresent) noexcept;

        void CreateDeviceResources();
        void CreateWindowSizeDependentResources();
        void SetWindow(HWND window, int width, int height);
        bool WindowSizeChanged(int width, int height);
        void HandleDeviceLost();
        void RegisterDeviceNotify(IDeviceNotify* deviceNotify) { m_deviceNotify = deviceNotify; }
        void Present();

        // Device Accessors.
        RECT GetOutputSize() const { return m_outputSize; }

        // Direct3D Accessors.
        ID3D11Device1*          GetD3DDevice() const { return m_d3dDevice.Get(); }
        ID3D11DeviceContext1*   GetD3DDeviceContext() const { return m_d3dContext.Get(); }
        IDXGISwapChain1*        GetSwapChain() const { return m_swapChain.Get(); }
        D3D_FEATURE_LEVEL       GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
        ID3D11Texture2D*        GetRenderTarget() const { return m_renderTarget.Get(); }
        ID3D11Texture2D*        GetDepthStencil() const { return m_depthStencil.Get(); }
        ID3D11RenderTargetView*	GetRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
        ID3D11DepthStencilView* GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
        DXGI_FORMAT             GetBackBufferFormat() const { return m_backBufferFormat; }
        DXGI_FORMAT             GetDepthBufferFormat() const { return m_depthBufferFormat; }
        D3D11_VIEWPORT          GetScreenViewport() const { return m_screenViewport; }
        UINT                    GetBackBufferCount() const { return m_backBufferCount; }
        DXGI_COLOR_SPACE_TYPE   GetColorSpace() const { return m_colorSpace; }
        unsigned int            GetDeviceOptions() const { return m_options; }

        // Performance events
        void PIXBeginEvent(_In_z_ const wchar_t* name)
        {
            m_d3dAnnotation->BeginEvent(name);
        }

        void PIXEndEvent()
        {
            m_d3dAnnotation->EndEvent();
        }

        void PIXSetMarker(_In_z_ const wchar_t* name)
        {
            m_d3dAnnotation->SetMarker(name);
        }

    private:
        void CreateFactory();
        void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);
        void UpdateColorSpace();

        // Direct3D objects.
        Microsoft::WRL::ComPtr<IDXGIFactory2>               m_dxgiFactory;
        Microsoft::WRL::ComPtr<ID3D11Device1>               m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>        m_d3dContext;
        Microsoft::WRL::ComPtr<IDXGISwapChain1>             m_swapChain;
        Microsoft::WRL::ComPtr<ID3DUserDefinedAnnotation>   m_d3dAnnotation;

        // Direct3D rendering objects. Required for 3D.
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_renderTarget;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencil;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_d3dRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_d3dDepthStencilView;
        D3D11_VIEWPORT                                  m_screenViewport;

        // Direct3D properties.
        DXGI_FORMAT                                     m_backBufferFormat;
        DXGI_FORMAT                                     m_depthBufferFormat;
        UINT                                            m_backBufferCount;
        D3D_FEATURE_LEVEL                               m_d3dMinFeatureLevel;

        // Cached device properties.
        HWND                                            m_window;
        D3D_FEATURE_LEVEL                               m_d3dFeatureLevel;
        RECT                                            m_outputSize;

        // HDR Support
        DXGI_COLOR_SPACE_TYPE                           m_colorSpace;

        // DeviceResources_11 options (see flags above)
        unsigned int                                    m_options;

        // The IDeviceNotify can be held directly as it owns the DeviceResources_11.
        IDeviceNotify*                                  m_deviceNotify;
    };
}


#endif // !DX_DEVICE_RESSOURCES_11

/*
 * The MIT License(MIT)
 *
 * Copyright(c) 2017 - 2021 Microsoft Corp
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files(the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions :
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */