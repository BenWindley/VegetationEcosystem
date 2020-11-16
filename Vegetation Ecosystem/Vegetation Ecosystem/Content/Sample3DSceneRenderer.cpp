#include "pch.h"
#include "Sample3DSceneRenderer.h"

#include "..\Common\DirectXHelper.h"

#include <random>
#include <cmath>
#include <string>
#include <codecvt>
#include <locale>

using namespace Vegetation_Ecosystem;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_degreesPerSecond(45),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 1.0f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	m_totalTime = timer.GetTotalSeconds();

	Species("Oak");

	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
	}

	std::vector<VegetationFeature*> allFeatures;

	for (auto& t : m_trees)
	{
		t->UpdateAllFeatures(&allFeatures);
	}

	for (auto& t : m_trees)
	{
		t->SetAllFeatures(allFeatures);
		t->UpdateLight(0.01f);
	}

	for (auto& t : m_trees)
	{
		t->Update(0.01f);
	}

	m_calculateBiomass++;

	if (m_trees.size() > 0 && (m_calculateBiomass % 100) == 0 && m_calculateBiomass <= 1000)
	{
		float totalBiomass = 0.0f;
		float minX = 0.0f, maxX = 0.0f, minY = 0.0f, maxY = 0.0f;

		for (auto& t : m_trees)
		{
			totalBiomass += t->GetBiomass();

			auto p = t->GetPosition();

			minX = std::fminf(minX, p.m128_f32[0]);
			maxX = std::fmaxf(maxX, p.m128_f32[0]);
			minY = std::fminf(minY, p.m128_f32[2]);
			maxY = std::fmaxf(maxY, p.m128_f32[2]);
		}

		float area = (maxX - minX) * (maxY - minY);

		OutputDebugString(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
			std::string("Average Biomass: ") + std::to_string(totalBiomass / m_trees.size()) + 
			std::string("\nPlant Density: ") + std::to_string(totalBiomass / area) + "\n").c_str()
		);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	return;

	for (auto& t : m_trees)
	{
		if (t)
			if (t->IsComplete())
				t->SetLocalRotation(DirectX::XMQuaternionRotationRollPitchYaw(0, radians, 0));
	}
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

std::wstring Vegetation_Ecosystem::Sample3DSceneRenderer::GetComputeProgress()
{
	return (m_trees.size() > 0) ? std::to_wstring(m_trees[0]->GetProgress() * 100) : L"0";
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	bool renderLeaves = (std::fmod(m_totalTime, 16) > 8);
	for (auto& t : m_trees)
	{
		t->Render(m_constantBufferData, renderLeaves);
	}
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_rendererResources.vertexShader
				)
			);

			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					&fileData[0],
					fileData.size(),
					&m_rendererResources.inputLayout
				)
			);
		}
	);

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_rendererResources.pixelShader
				)
			);

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateBuffer(
					&constantBufferDesc,
					nullptr,
					&m_rendererResources.constantBuffer
				)
			);
		}
	);

	// Once both shaders are loaded, create the mesh.
	auto createVegetationTask = (createPSTask && createVSTask).then([this]()
		{
			ID3D11BlendState* d3dBlendState;
			D3D11_BLEND_DESC omDesc;

			ZeroMemory(&omDesc, sizeof(D3D11_BLEND_DESC));
			omDesc.RenderTarget[0].BlendEnable = true;
			omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			m_deviceResources->GetD3DDevice()->CreateBlendState(&omDesc, &d3dBlendState);
			m_deviceResources->GetD3DDeviceContext()->OMSetBlendState(d3dBlendState, 0, 0xffffffff);

			std::random_device r;
			std::normal_distribution<float> position(0, PLANT_SPACING);
			//std::uniform_real_distribution<float> position(-PLANT_SPACING, PLANT_SPACING);

			for (int x = 0; x < 0; ++x)
			{
				m_trees.push_back(new Vegetation(Species("Oak")));
				m_trees.back()->SetLocalPosition({ position(r) , -1.0f, position(r) });
				m_trees.back()->SetScale({ 0.02f, 0.02f, 0.02f });
				m_trees.back()->Start(&*m_deviceResources, &m_rendererResources);
			}

			for (int x = 0; x < 1; ++x)
			{
				m_trees.push_back(new Vegetation(Species("Pine")));
				m_trees.back()->SetLocalPosition({ 0 , -1.0f, 0 });
				m_trees.back()->SetScale({ 0.02f, 0.02f, 0.02f });
				m_trees.back()->Start(&*m_deviceResources, &m_rendererResources);
			}

			for (int x = 0; x < 0; ++x)
			{
				m_trees.push_back(new Vegetation(Species("Shrub")));
				m_trees.back()->SetLocalPosition({ position(r) , -1.0f, position(r) });
				m_trees.back()->SetScale({ 0.02f, 0.02f, 0.02f });
				m_trees.back()->Start(&*m_deviceResources, &m_rendererResources);
			}
		}
	);
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_rendererResources.vertexShader.Reset();
	m_rendererResources.inputLayout.Reset();
	m_rendererResources.pixelShader.Reset();
	m_rendererResources.constantBuffer.Reset();
	m_rendererResources.vertexBuffer.Reset();
	m_rendererResources.indexBuffer.Reset();
}
