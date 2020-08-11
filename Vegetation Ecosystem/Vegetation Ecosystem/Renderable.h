#pragma once

#include "..\Common\DeviceResources.h"
#include "Content/ShaderStructures.h"

#include <vector>

class Renderable
{
public:
	Renderable() = default;
	~Renderable() = default;

	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);

protected:
	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Vegetation_Ecosystem::VertexPositionColor vertices[], uint32 vertexCount, unsigned short indices[], uint32 indexCount);

	bool m_loadingComplete = false;
	uint32 m_indexCount;

	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_vertexBuffer;

	DX::DeviceResources* m_deviceResources;
	ID3D11InputLayout* m_inputLayout;
	ID3D11VertexShader* m_vShader;
	ID3D11PixelShader* m_pShader;
	ID3D11Buffer* m_constantBuffer;
};
