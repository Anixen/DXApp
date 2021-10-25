/**
 * Header for standard system include files.
 *
 * Advanced Technology Group (ATG)
 * Copyright (C) Microsoft Corporation. All rights reserved.
 *
 * @file src/pch.h
 * @author Chuck Walbourn
 * @date 20170809 - Initial Release
 *
 * @author Olivier Falconnet
 * @date 20211025 - Included to app-SimpleBezier project
 * @date 20211025 - Reworked dependencies
 */

#ifndef PCH_SIMPLE_BEZIER_12
#define PCH_SIMPLE_BEZIER_12


// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

#include <wrl/client.h>
#include <wrl/event.h>

#include <d3d12.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>

#include "d3dx12.h"

#include <filesystem>

// To use graphics and CPU markup events with the latest version of PIX, change this to include <pix3.h> 
// then add the NuGet package WinPixEventRuntime to the project. 
#include <pix.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "SimpleMath.h"

#include "StepTimer.h"

namespace DX
{
    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) noexcept : result(hr) {}

        const char* what() const override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // Helper utility converts D3D API failures into exceptions.
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw com_exception(hr);
        }
    }
}


#endif // !PCH_SIMPLE_BEZIER_12

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