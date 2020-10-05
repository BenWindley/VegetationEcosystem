#pragma once
#include <memory>

#include "Content/ShaderStructures.h"
#include "..\Common\DeviceResources.h"

#include "Transform.h"
#include "Cube.h"

class VegetationFeature : public Transform
{
public:
	VegetationFeature() = default;
	VegetationFeature(Transform* parent);
	virtual ~VegetationFeature() = default;

public:
	virtual void Start(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources);
	virtual void Update();
	virtual void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	bool GetFate();
	bool GetRemove();

protected:
	DX::DeviceResources* m_deviceResources;
	Vegetation_Ecosystem::RendererResources* m_rendererResources;

	Transform* m_parent;
	//Cube* m_cube;

	bool m_fate = true;
	bool m_remove = false;
};
