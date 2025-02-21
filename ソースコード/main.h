//=================================================================================================
//
// ���C���Ǘ� [main.h]
//
//=================================================================================================
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dxgi.lib")

//#define SCREEN_WIDTH	(1920)
//#define SCREEN_HEIGHT	(1080)

#define SCREEN_WIDTH	(960)
#define SCREEN_HEIGHT	(540)

HWND GetWindow();