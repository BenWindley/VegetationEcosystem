#pragma once


#define MAX_AGE 10.0f
#define MAX_THREADS 4
#define TROPISM_FACTOR 0.2f
#define LIGHTRAYS 30
#define PHOTOTROPISM_FACTOR 1.0f
#define SPATIALTROPISM_FACTOR 0.2f
#define SPATIAL_FALLOFF 10
#define GRAVITROPISM_FACTOR 0.1f
#define MAX_DEPTH 100

#define SPREAD_MAIN 20
#define SPREAD_BRANCH 45
#define SPREAD_RANDOM 2

#define WIDTH_MAIN 0.8
#define WIDTH_BRANCH 0.7

#define CYLINDER_SEGMENTS 10
#define LEAF_QUANTITY 5
#define LEAF_SIZE 3.0f
#define LEAF_DISPLACEMENT 5

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