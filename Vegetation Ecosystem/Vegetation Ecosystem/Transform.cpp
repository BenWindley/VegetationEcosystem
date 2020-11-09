#include "pch.h"
#include "Transform.h"

using namespace DirectX;

XMVECTOR Transform::GetPosition()
{
	return m_parent ? XMVectorAdd(m_localPosition, m_parent->GetPosition()) : m_localPosition;
}

XMVECTOR Transform::GetLocalPosition()
{
	return m_localPosition;
}

XMVECTOR Transform::GetRotation()
{
	return m_parent ? DirectX::XMQuaternionMultiply(m_parent->GetRotation(), m_localRotation) : m_localRotation;
}

XMVECTOR Transform::GetLocalRotation()
{
	return m_localRotation;
}

XMVECTOR Transform::GetScale()
{
	return m_parent ? XMVectorMultiply(m_localScale, m_parent->GetScale()) : m_localScale;
}

XMVECTOR Transform::GetLocalScale()
{
	return m_localScale;
}

DirectX::XMMATRIX Transform::GetTransposeMatrix()
{
	auto selfMatrix = XMMatrixTranspose(XMMatrixAffineTransformation(GetLocalScale(), { 0 }, GetLocalRotation(), GetLocalPosition()));

	return m_parent ? XMMatrixMultiply(m_parent->GetTransposeMatrix(), selfMatrix) : selfMatrix;
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
	m_localRotation = m_parent ? DirectX::XMQuaternionMultiply(DirectX::XMQuaternionInverse(m_parent->GetRotation()), rotation) : rotation;
}

void Transform::SetLocalRotation(XMVECTOR rotation)
{
	m_localRotation = rotation;
}

void Transform::SetScale(XMVECTOR scale)
{
	m_localScale = m_parent ? XMVectorDivide(scale, m_parent->GetScale()) : scale;
}

void Transform::SetLocalScale(XMVECTOR scale)
{
	m_localScale = scale;
}
