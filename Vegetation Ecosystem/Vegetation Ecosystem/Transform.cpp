#include "pch.h"
#include "Transform.h"

DirectX::XMVECTOR Transform::GetPosition()
{
	return m_parent ? DirectX::XMVectorAdd(m_localPosition, m_parent->GetLocalPosition()) : m_localPosition;
}

DirectX::XMVECTOR Transform::GetLocalPosition()
{
	return m_localPosition;
}

DirectX::XMMATRIX Transform::GetRotation()
{
	return m_parent ? m_localRotation + m_parent->GetLocalRotation() : m_localRotation;
}

DirectX::XMMATRIX Transform::GetLocalRotation()
{
	return m_localRotation;
}

void Transform::SetPosition(DirectX::XMVECTOR position)
{
	m_localPosition = m_parent ? DirectX::XMVectorSubtract(position, m_parent->GetPosition()) : position;
}

void Transform::SetLocalPosition(DirectX::XMVECTOR position)
{
	m_localPosition = position;
}

void Transform::SetRotation(DirectX::XMMATRIX rotation)
{
	m_localRotation = m_parent ? rotation - m_parent->GetLocalRotation() : rotation;
}

void Transform::SetLocalRotation(DirectX::XMMATRIX rotation)
{
	m_localRotation = rotation;
}
