#ifndef LEGEND_STDAFX_H_
#define LEGEND_STDAFX_H_

#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include <array>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "src/libs/stb_image.h"

// clang-format off
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
