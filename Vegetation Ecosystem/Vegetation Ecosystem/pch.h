#pragma once


#define MAX_AGE 30.0f
#define MAX_THREADS 4
#define TROPISM_FACTOR 0.2f
#define PHOTOTROPISM_FACTOR 1.0f
#define SPATIALTROPISM_FACTOR 0.5f
#define GRAVITROPISM_FACTOR 0.0f
#define MAX_DEPTH 40

#define SPREAD_MAIN 25
#define SPREAD_BRANCH 45
#define SPATIAL_FALLOFF 10

#define CYLINDER_SEGMENTS 10

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