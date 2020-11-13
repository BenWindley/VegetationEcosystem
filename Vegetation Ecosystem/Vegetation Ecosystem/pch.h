#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>

#define MAX_AGE 5.0f
#define MAX_THREADS 5

#define LIGHTRAYS 15
#define MAX_COMPLEXITY 20

#define CYLINDER_SEGMENTS 10
#define LEAF_QUANTITY 5
#define LEAF_SIZE 7.0f
#define LEAF_DISPLACEMENT 8

#define UPKEEP_COEFFICIENT 1e-10F

#define PLANT_SPACING 0.5f
