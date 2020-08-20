#ifndef LEGEND_STDAFX_H_
#define LEGEND_STDAFX_H_

#define _ENABLE_EXTENDED_ALIGNED_STORAGE

// clang-format off
#define NOMINMAX
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include <wrl/client.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>
#include <functional>
#include <map>

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include "src/util/typedef.h"
#include "src/libs/d3dx12.h"
#include "src/libs/imgui/imgui.h"

#include "src/util/debug.h"
//clang-format on


namespace legend {
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
}  // namespace legend

#endif  // !LEGEND_STDAFX_H_
