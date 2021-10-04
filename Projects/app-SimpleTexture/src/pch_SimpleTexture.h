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
 * @date 20210921 - Included to ModelViewer project, updated file description
 */

#ifndef PCH_SIMPLE_TEXTURE
#define PCH_SIMPLE_TEXTURE


#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0601
#include <SDKDDKVer.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>

#if defined(NTDDI_WIN10_RS2)
#include <dxgi1_6.h>
#else
#include <dxgi1_5.h>
#endif

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wincodec.h>

#include <algorithm>
#include <exception>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <string>
#include <filesystem>

#include <stdio.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "GamePad.h"
#include "Keyboard.h"
#include "Mouse.h"

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

#include <cmath>
#include <cstdint>
#include <exception>

#include <streambuf>
#include <ostream>

#include <string>
#include <filesystem>
#include <time.h>
#include <sstream>
#include <iostream>


#endif // !PCH_SIMPLE_TEXTURE

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