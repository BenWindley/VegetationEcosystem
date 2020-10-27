#include "pch.h"
#include "CylinderSegment.h"

using namespace DirectX;
using namespace Windows::Foundation;

void CylinderSegment::Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, float* branchWidth)
{
    m_branchWidth = branchWidth;

	std::vector<unsigned short> indices;

    float height = 1.0f;
    float radius = *m_branchWidth;

    std::vector<XMFLOAT2> circleVertices;

    for (int i = 0; i <= CYLINDER_SEGMENTS; ++i)
    {
        float sectorAngle = i * XM_2PI / CYLINDER_SEGMENTS;
        circleVertices.push_back({ radius * cosf(sectorAngle), radius * sinf(sectorAngle)});
    }

    for (int i = 0; i <= CYLINDER_SEGMENTS; ++i)
    {
        m_vertices.push_back({ DirectX::XMFLOAT3(circleVertices[i].x, 5, circleVertices[i].y), { 1, 1, 1 } });
        m_vertices.push_back( { DirectX::XMFLOAT3(circleVertices[i].x, 0, circleVertices[i].y), { 1, 1, 1 } });
    }

    for (unsigned short i = 0; i < CYLINDER_SEGMENTS * 2; i+=2)
    {
        indices.push_back(i + 3);
        indices.push_back(i + 0);
        indices.push_back(i + 1);

        indices.push_back(i + 2);
        indices.push_back(i + 0);
        indices.push_back(i + 3);
    }

	Renderable::Init(deviceResources, rendererResources, &m_vertices[0], m_vertices.size(), &indices[0], indices.size());
}

void CylinderSegment::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
    if (!this) return;

    XMStoreFloat4x4(&constantBufferData.model, GetTransposeMatrix());

    Renderable::Render(constantBufferData);
}

void CylinderSegment::SetSegment(CylinderSegment* nextSegment)
{
    m_nextSegment = nextSegment;
}
