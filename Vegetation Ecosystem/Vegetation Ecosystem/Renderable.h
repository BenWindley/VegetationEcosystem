#pragma once

#include "..\Common\DeviceResources.h"
#include "Content/ShaderStructures.h"

#include <string>
#include <vector>

class Renderable
{
public:
	Renderable() = default;
	~Renderable() = default;

	void Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData);
	std::vector<Vegetation_Ecosystem::VertexPositionColor> GetVertices() { return m_vertices; };
	std::vector<unsigned short> GetIndices() { return m_indices; };
	int GetTextureID() { return m_textureID; };

protected:
	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Vegetation_Ecosystem::VertexPositionColor vertices[], uint32 vertexCount, unsigned short indices[], uint32 indexCount, std::wstring texName);
	void Init(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Vegetation_Ecosystem::VertexPositionColor vertices[], uint32 vertexCount, unsigned short indices[], uint32 indexCount);

	void CreateResources(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Vegetation_Ecosystem::VertexPositionColor vertices[], uint32 vertexCount, unsigned short indices[], uint32 indexCount);

	bool m_loadingComplete = false;
	uint32 m_indexCount;

	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_vertexBuffer;

	std::vector<Vegetation_Ecosystem::VertexPositionColor> m_vertices;
	std::vector<unsigned short> m_indices;
	int m_textureID;

	DX::DeviceResources* m_deviceResources;
	ID3D11InputLayout* m_inputLayout;
	ID3D11VertexShader* m_vShader;
	ID3D11PixelShader* m_pShader;
	ID3D11Buffer* m_constantBuffer;

	ID3D11Resource* m_texture = nullptr;
	ID3D11ShaderResourceView* m_textureResourceView = nullptr;
	ID3D11SamplerState* m_samplerState = nullptr;

private:
	struct Texture
	{
		int id;
		std::wstring name;
		ID3D11Resource* m_texture;
		ID3D11ShaderResourceView* m_textureResourceView;
		ID3D11SamplerState* m_samplerState;
	};
};
