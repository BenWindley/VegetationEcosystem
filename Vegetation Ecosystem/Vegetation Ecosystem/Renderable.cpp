#include "pch.h"
#include "Renderable.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace Windows::Foundation;

void Renderable::Init(DX::DeviceResources* deviceResources, 
	Vegetation_Ecosystem::RendererResources* rendererResources, 
	Vegetation_Ecosystem::VertexPositionColor vertices[], 
	uint32 vertexCount,
	unsigned short indices[],
	uint32 indexCount)
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

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
	);
}
