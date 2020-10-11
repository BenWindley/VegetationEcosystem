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
	DirectX::XMVECTOR GetRotation();
	DirectX::XMVECTOR GetLocalRotation();
	DirectX::XMVECTOR GetScale();
	DirectX::XMVECTOR GetLocalScale();

	DirectX::XMMATRIX GetTransposeMatrix();

	void SetPosition(DirectX::XMVECTOR position);
	void SetLocalPosition(DirectX::XMVECTOR position);
	void SetRotation(DirectX::XMVECTOR rotation);
	void SetLocalRotation(DirectX::XMVECTOR rotation);
	void SetScale(DirectX::XMVECTOR rotation);
	void SetLocalScale(DirectX::XMVECTOR rotation);


private:
	DirectX::XMVECTOR m_localPosition = DirectX::XMVECTOR{ 0, 0, 0, 1 };
	DirectX::XMVECTOR m_localRotation = DirectX::XMVECTOR{ 0, 0, 0, 1 };
	DirectX::XMVECTOR m_localScale =	DirectX::XMVECTOR{ 1, 1, 1, 1 };
};
