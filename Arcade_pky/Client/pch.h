#pragma once

#pragma region Standard Libraries
#include <algorithm>
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#pragma endregion Standard Libraries

#define NOMINMAX

#pragma region DirectX
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "wrl.h"
using namespace Microsoft::WRL;
#pragma endregion

#include "Core/Defines.h"

#pragma region Vector
#include "Core/Vector2.h"
#include "Core/Vector3.h"
#include "Core/Vector4.h"
#pragma endregion Vector

#include "Core/Color.h"
#include "Core/Matrix.h"
#include "Core/Ptr.h"

#define _HAS_COLLISION_MODULE
#define _HAS_TILEMAP_MODULE

