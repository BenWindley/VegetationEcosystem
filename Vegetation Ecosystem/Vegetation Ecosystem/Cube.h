#pragma once

#include "..\Common\DeviceResources.h"
#include "Content/ShaderStructures.h"
#include "Transform.h"
#include "Renderable.h"

class Cube : public Transform, public Renderable
{
public:
	Cube() = default;
	~Cube() = default;

	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

private:
};
