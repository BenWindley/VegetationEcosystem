#include "pch.h"
#include "Renderable.h"

#include <DirectXMath.h>
#include "Common/WICTextureLoader.h"

using namespace DirectX;
using namespace Windows::Foundation;

void Renderable::Init(DX::DeviceResources* deviceResources,
	Vegetation_Ecosystem::RendererResources* rendererResources,
	Vegetation_Ecosystem::VertexPositionColor vertices[],
	uint32 vertexCount,
	unsigned short indices[],
	uint32 indexCount)
{
	Renderable::Init(deviceResources, rendererResources, vertices, vertexCount, indices, indexCount, L"Assets\\Blank.png");
}

void Renderable::Init(
	DX::DeviceResources* deviceResources, 
	Vegetation_Ecosystem::RendererResources* rendererResources,
	Vegetation_Ecosystem::VertexPositionColor vertices[], 
	uint32 vertexCount, 
	unsigned short indices[],
	uint32 indexCount, 
	std::wstring texName)
{
	static std::vector<Texture> textures;

	bool exists = false;

	for (auto& t : textures)
	{
		if (t.name == texName)
		{
			exists = true;

			m_texture = t.m_texture;
			m_textureResourceView = t.m_textureResourceView;
			m_samplerState = t.m_samplerState;
		}
	}

	if (!exists)
	{
		auto result = DirectX::CreateWICTextureFromFile(deviceResources->GetD3DDevice(), deviceResources->GetD3DDeviceContext(), texName.c_str(), &m_texture, &m_textureResourceView);

		textures.push_back({ texName, m_texture, m_textureResourceView, m_samplerState });

		D3D11_SHADER_RESOURCE_VIEW_DESC textureResourceViewDesc;
		textureResourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		textureResourceViewDesc.Texture2D.MostDetailedMip = 0;
		textureResourceViewDesc.Texture2D.MipLevels = -1;

		deviceResources->GetD3DDevice()->CreateShaderResourceView(m_texture, &textureResourceViewDesc, &m_textureResourceView);

		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

		deviceResources->GetD3DDevice()->CreateSamplerState(&sampler_desc, &m_samplerState);
	}

	CreateResources(deviceResources, rendererResources, vertices, vertexCount, indices, indexCount);
}

void Renderable::CreateResources(DX::DeviceResources* deviceResources, Vegetation_Ecosystem::RendererResources* rendererResources, Vegetation_Ecosystem::VertexPositionColor vertices[], uint32 vertexCount, unsigned short indices[], uint32 indexCount)
{
	m_deviceResources = deviceResources;
	m_inputLayout = rendererResources->inputLayout.Get();
	m_vShader = rendererResources->vertexShader.Get();
	m_pShader = rendererResources->pixelShader.Get();
	m_constantBuffer = rendererResources->constantBuffer.Get();

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(Vegetation_Ecosystem::VertexPositionColor) * vertexCount, D3D11_BIND_VERTEX_BUFFER);

	m_deviceResources->GetD3DDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
	);

	m_indexCount = indexCount;

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * indexCount, D3D11_BIND_INDEX_BUFFER);

	m_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
	);

	m_loadingComplete = true;
}

void Renderable::Render(Vegetation_Ecosystem::ModelViewProjectionConstantBuffer constantBufferData)
{
	if (!this) return;
	if (!m_loadingComplete) return;

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(
		m_constantBuffer,
		0,
		NULL,
		&constantBufferData,
		0,
		0,
		0
	);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(Vegetation_Ecosystem::VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		&m_vertexBuffer,
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_indexBuffer,
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout);

	// Attach our vertex shader.
	context->VSSetShader(
		m_vShader,
		nullptr,
		0
	);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(
		0,
		1,
		&m_constantBuffer,
		nullptr,
		nullptr
	);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pShader,
		nullptr,
		0
	);

	context->PSSetShaderResources(
		0, 
		1, 
		&m_textureResourceView
	);

	context->PSSetSamplers(
		0,
		1,
		&m_samplerState
	);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}
