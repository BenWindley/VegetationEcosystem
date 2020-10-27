#pragma once

#include "Transform.h"
#include "Renderable.h"

class CylinderSegment : public Renderable, public Transform
{
public:
	CylinderSegment() = default;
	~CylinderSegment() = default;

	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, float* branchWidth);
	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

	void SetSegment(CylinderSegment* nextSegment);

	std::vector<Vegetation_Ecosystem::VertexPositionColor> m_vertices;

private:
	CylinderSegment* m_nextSegment;
	float* m_branchWidth;
};

