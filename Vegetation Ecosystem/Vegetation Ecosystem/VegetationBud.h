#pragma once
#include "VegetationFeature.h"

class VegetationBud : public VegetationFeature
{
public:
	VegetationBud(Transform* parent, bool terminalBud) : VegetationFeature(parent), m_terminalBud(terminalBud){};
	~VegetationBud() = default;

public:
	void Update()override;
	void Render()override;

private:
	bool m_terminalBud;
};
