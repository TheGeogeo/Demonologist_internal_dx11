#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

#include <d3dcompiler.h>
#include <cstring>
#include <tchar.h>
#include <memory>
#include <thread>
#include <functional>
#include <array>
#include <dwmapi.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <cstdint>
#include <cmath>
#include <type_traits>
#include <initializer_list>
#include <algorithm>
#include <windows.h>
#include <random>
#include <iomanip>
#include <map>

#include "SDK/SDK.hpp"
#include "singleton.h"
#include "utilfunction.h"
#include "cfg.h"
#include "MenuGUI.h"
#include "CheatMain.h"
#include "hotkey.h"
#include "esp.h"
#include "mainoffset.h"