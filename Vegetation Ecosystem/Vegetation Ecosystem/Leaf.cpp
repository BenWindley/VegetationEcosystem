#include "pch.h"
#include "Leaf.h"

using namespace DirectX;
using namespace Windows::Foundation;

void Leaf::Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Transform* currentTransform, DirectX::XMVECTOR rotation, DirectX::XMVECTOR offset)
{
    m_vertices.clear();
    m_indices.clear();

    Transform tempTransform;
    tempTransform.m_parent = currentTransform;

    XMVECTOR sampleScale;
    XMVECTOR sampleRotation;
    XMVECTOR samplePosition;

    tempTransform.SetRotation(rotation);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            if (x == 0 || y == 0) continue;

            Transform t;

            XMVECTOR pos = { LEAF_SIZE * x / 2.0f, 0, LEAF_SIZE * y / 2.0f };

            pos = XMVector3Rotate(pos, rotation);

            tempTransform.SetLocalPosition(XMVectorAdd(offset, pos));
            XMMatrixDecompose(&sampleScale, &sampleRotation, &samplePosition, XMMatrixTranspose(tempTransform.GetTransposeMatrix()));

            XMFLOAT2 uvs = { (x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f };

            m_vertices.push_back({ {samplePosition.m128_f32[0], samplePosition.m128_f32[1], samplePosition.m128_f32[2]}, {1.0f, 1.0f, 1.0f}, uvs });
        }
    }

    m_indices.push_back(0);
    m_indices.push_back(1);
    m_indices.push_back(2);

    m_indices.push_back(1);
    m_indices.push_back(3);
    m_indices.push_back(2);

    m_indices.push_back(1);
    m_indices.push_back(0);
    m_indices.push_back(2);

    m_indices.push_back(3);
    m_indices.push_back(1);
    m_indices.push_back(2);

    Renderable::Init(deviceResources, rendererResources, &m_vertices[0], m_vertices.size(), &m_indices[0], m_indices.size(), L"Assets\\Leaf.png");
}

void Leaf::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
    if (!this) return;

    XMStoreFloat4x4(&constantBufferData.model, GetTransposeMatrix());

    Renderable::Render(constantBufferData);
}
