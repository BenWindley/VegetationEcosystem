#pragma once

#include "Transform.h"
#include "Renderable.h"

class CylinderSegment : public Renderable, public Transform
{
public:
	CylinderSegment() = default;
	~CylinderSegment() = default;

	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, float branchWidth, CylinderSegment* previous, Transform* currentTransform, bool branchEnd, bool branching = false);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);
};

