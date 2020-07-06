#pragma once
#include <vector>
#include <memory>

#include <DirectXMath.h>

class Transform
{
public:
	Transform() = default;
	~Transform() = default;

public:
	Transform* m_parent;

	DirectX::XMVECTOR GetPosition();
	DirectX::XMVECTOR GetLocalPosition();
	DirectX::XMMATRIX GetRotation();
	DirectX::XMMATRIX GetLocalRotation();

	void SetPosition(DirectX::XMVECTOR position);
	void SetLocalPosition(DirectX::XMVECTOR position);
	void SetRotation(DirectX::XMMATRIX rotation);
	void SetLocalRotation(DirectX::XMMATRIX rotation);

private:
	DirectX::XMVECTOR m_localPosition = DirectX::XMVECTOR();
	DirectX::XMMATRIX m_localRotation = DirectX::XMMATRIX();
};
