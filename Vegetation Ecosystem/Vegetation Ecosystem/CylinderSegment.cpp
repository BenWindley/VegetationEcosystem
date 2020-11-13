#include "pch.h"
#include "CylinderSegment.h"

using namespace DirectX;
using namespace Windows::Foundation;

void CylinderSegment::Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, float branchWidth, CylinderSegment* previous, Transform* currentTransform, bool branchEnd, bool branching)
{
    std::vector<XMFLOAT2> circleVertices;

    float radius = branchEnd ? 0 : branchWidth;

    for (int i = 0; i <= CYLINDER_SEGMENTS; ++i)
    {
        float sectorAngle = i * XM_2PI / CYLINDER_SEGMENTS;
        circleVertices.push_back({ radius * cosf(sectorAngle), radius * sinf(sectorAngle)});
    }

    Transform ringTransform;
    ringTransform.m_parent = currentTransform;

    XMVECTOR sampleScale;
    XMVECTOR sampleRotation;
    XMVECTOR samplePosition;

    for (int i = 0; i <= CYLINDER_SEGMENTS; ++i)
    {
        ringTransform.SetLocalPosition({ circleVertices[i].x, 0, circleVertices[i].y });

        XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(ringTransform.GetTransposeMatrix()));

        m_vertices.push_back({ DirectX::XMFLOAT3(samplePosition.m128_f32[0], samplePosition.m128_f32[1], samplePosition.m128_f32[2]), { 1, 1, 1 }, {(float) i / CYLINDER_SEGMENTS, 1} });

        if (branching || previous == nullptr)
        {
            m_vertices.push_back(m_vertices.back());
            m_vertices.back().uv.y = 0;
        }
        else
        {
            m_vertices.push_back(previous->m_vertices[i * 2]);
            m_vertices.back().uv.y = 0;
        }
    }

    for (unsigned short i = 0; i < CYLINDER_SEGMENTS * 2; i+=2)
    {
        m_indices.push_back(i + 3);
        m_indices.push_back(i + 0);
        m_indices.push_back(i + 1);

        m_indices.push_back(i + 2);
        m_indices.push_back(i + 0);
        m_indices.push_back(i + 3);
    }

	Renderable::Init(deviceResources, rendererResources, &m_vertices[0], m_vertices.size(), &m_indices[0], m_indices.size(), L"Assets\\Tree.png");
}

void CylinderSegment::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
    if (!this) return;

    XMStoreFloat4x4(&constantBufferData.model, GetTransposeMatrix());

    Renderable::Render(constantBufferData);
}