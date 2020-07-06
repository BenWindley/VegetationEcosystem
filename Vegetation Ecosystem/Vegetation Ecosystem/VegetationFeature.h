#pragma once
#include <memory>

#include "Transform.h"

class VegetationFeature : public Transform
{
public:
	VegetationFeature() = default;
	VegetationFeature(Transform* parent);
	virtual ~VegetationFeature() = default;

public:
	virtual void Update();
	virtual void Render();

	bool GetFate();
	bool GetRemove();

protected:
	Transform* m_parent;
	bool m_fate = true;
	bool m_remove = false;
};