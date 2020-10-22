#pragma once


#define MAX_AGE 10.0f
#define MAX_THREADS 4
#define TROPISM_FACTOR 0.5f
#define PHOTOTROPISM_FACTOR 0.0f
#define SPATIALTROPISM_FACTOR 0.0f
#define GRAVITROPISM_FACTOR 0.0f
#define MAX_DEPTH 10
#define SPREAD_MAIN 15
#define SPREAD_BRANCH 15
#define SPATIAL_FALLOFF 100


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