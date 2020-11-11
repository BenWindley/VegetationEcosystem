#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"

#include "Cube.h"
#include "Vegetation.h"

namespace Vegetation_Ecosystem
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() { return m_tracking; }

		std::wstring GetComputeProgress();


	private:
		void Rotate(float radians);

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for geometry.
		RendererResources m_rendererResources;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer m_constantBufferData;

		std::vector<Vegetation*> m_trees;

		// Variables used with the rendering loop.
		float	m_degreesPerSecond;
		bool	m_tracking;
		float	m_totalTime;

		int m_calculateBiomass = 0;
	};
}
