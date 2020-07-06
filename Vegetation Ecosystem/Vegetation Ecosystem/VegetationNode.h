#pragma once
#include <vector>
#include <memory>

#include "VegetationFeature.h"

class VegetationNode : public Transform
{
public:
	VegetationNode() = default;
	virtual ~VegetationNode() = default;

public:
	virtual void Start(Transform* parent);
	virtual void Start(VegetationNode* parent);
	virtual void Update(float growth);
	virtual void Render();

	void CreateNewNode(VegetationFeature growthBud);

	bool GetRemove();

private:
	VegetationNode* m_parentNode;
	std::vector<VegetationFeature>	m_vegetationFeatures;
	std::vector<VegetationNode> m_childNodes;

	bool m_remove;
};
