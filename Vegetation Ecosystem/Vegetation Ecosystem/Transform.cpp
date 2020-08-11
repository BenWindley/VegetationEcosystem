#include "pch.h"
#include "Transform.h"

using namespace DirectX;

XMVECTOR Transform::GetPosition()
{
	return m_parent ? XMVectorAdd(m_localPosition, m_parent->GetLocalPosition()) : m_localPosition;
}

XMVECTOR Transform::GetLocalPosition()
{
	return m_localPosition;
}

XMVECTOR Transform::GetRotation()
{
	return m_parent ? XMVectorAddAngles(m_localRotation, m_parent->GetLocalRotation()) : m_localRotation;
}

XMVECTOR Transform::GetLocalRotation()
{
	return m_localRotation;
}

XMVECTOR Transform::GetScale()
{
	return m_parent ? XMVectorMultiply(m_localScale, m_parent->GetLocalScale()) : m_localScale;
}

XMVECTOR Transform::GetLocalScale()
{
	return m_localScale;
}

DirectX::XMMATRIX Transform::GetMatrix()
{
	auto selfMatrix = XMMatrixTranspose(XMMatrixAffineTransformation(GetLocalScale(), { 0 }, XMQuaternionRotationRollPitchYawFromVector(GetLocalRotation()), GetLocalPosition()));

	return m_parent ? XMMatrixMultiply(m_parent->GetMatrix(), selfMatrix) : selfMatrix;
}

void Transform::SetPosition(XMVECTOR position)
{
	m_localPosition = m_parent ? XMVectorSubtract(position, m_parent->GetPosition()) : position;
}

void Transform::SetLocalPosition(XMVECTOR position)
{
	m_localPosition = position;
}

void Transform::SetRotation(XMVECTOR rotation)
{
	m_localRotation = m_parent ? XMVectorSubtractAngles(rotation, m_parent->GetLocalRotation()) : rotation;
}

void Transform::SetLocalRotation(XMVECTOR rotation)
{
	m_localRotation = rotation;
}

void Transform::SetScale(XMVECTOR scale)
{
	m_localScale = m_parent ? XMVectorDivide(scale, m_parent->GetLocalScale()) : scale;
}

void Transform::SetLocalScale(XMVECTOR scale)
{
	m_localScale = scale;
}
