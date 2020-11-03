#pragma once


#define MAX_AGE 20.0f
#define MAX_THREADS 4
#define TROPISM_FACTOR 0.15f
#define LIGHTRAYS 10
#define PHOTOTROPISM_FACTOR 1.0f
#define SPATIALTROPISM_FACTOR 0.5f
#define SPATIAL_FALLOFF 10
#define GRAVITROPISM_FACTOR 0.0f
#define MAX_COMPLEXITY 10

#define SPREAD_MAIN 15
#define SPREAD_BRANCH 60
#define SPREAD_RANDOM 5

#define WIDTH_MAIN 0.9
#define WIDTH_BRANCH 0.7

#define CYLINDER_SEGMENTS 10
#define LEAF_QUANTITY 5
#define LEAF_SIZE 7.0f
#define LEAF_DISPLACEMENT 8

#define UPKEEP_COEFFICIENT 0.00001f

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