#pragma once

#define MAX_AGE 5.0f
#define MAX_THREADS 5
#define TROPISM_FACTOR 0.1f
#define LIGHTRAYS 15
#define PHOTOTROPISM_FACTOR 1.0f
#define PHOTOTROPISM_BIAS 5.0f
#define SPATIALTROPISM_FACTOR 0.0f
#define GRAVITROPISM_FACTOR 0.0f
#define MAX_COMPLEXITY 10

#define SPREAD_MAIN 50
#define SPREAD_BRANCH 60
#define SPREAD_SIDE 90
#define SPREAD_RANDOM 10

#define WIDTH_STRAIGHT 0.95
#define WIDTH_MAIN 0.90
#define WIDTH_BRANCH 0.85
#define WIDTH_SIDE 0.7
#define MIN_BRANCH_WIDTH 0.05

#define CYLINDER_SEGMENTS 10
#define LEAF_QUANTITY 5
#define LEAF_SIZE 7.0f
#define LEAF_DISPLACEMENT 8

#define UPKEEP_COEFFICIENT 1e-10F

#define PLANT_SPACING 0.0f

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
