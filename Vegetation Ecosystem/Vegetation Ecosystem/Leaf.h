#pragma once

#include "Transform.h"
#include "Renderable.h"

class Leaf : public Renderable, public Transform
{
public:
	Leaf() = default;
	~Leaf() = default;

	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Transform* currentTransform, DirectX::XMVECTOR rotation, DirectX::XMVECTOR offset, float size);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);
};

