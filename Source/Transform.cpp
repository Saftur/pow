#include "stdafx.h"
#include "Transform.h"

Vector2D Transform::camTranslation = { 0, 0 };
bool Transform::camIsDirty = false;

Transform::Transform(float x, float y) :
		Component("Transform"), translation(x, y), rotation(0), scale(0, 0), isDirty(true), followCamera(true)
{
}

Component * Transform::Clone() const
{
	return new Transform(*this);
}

Matrix2D & Transform::GetMatrix()
{
	if (isDirty || (followCamera && camIsDirty)) {
		matrix = Matrix2D::TranslationMatrix(translation.X(), translation.Y()) *
				 Matrix2D::RotationMatrixRadians(rotation) *
				 Matrix2D::ScalingMatrix(scale.X(), scale.Y());
		isDirty = false;
	}
	if (followCamera && camIsDirty)
        matrix = Matrix2D().TranslationMatrix(camTranslation.X(), camTranslation.Y()) * matrix;
	return matrix;
}

const Vector2D & Transform::GetTranslation() const
{
	return translation;
}

const Vector2D Transform::GetWorldTranslation() const
{
	return parent ? translation + parent->GetWorldTranslation() : translation;
}

float Transform::GetRotation() const
{
	return rotation;
}

float Transform::GetWorldRotation() const
{
	return parent ? rotation + parent->GetRotation() : rotation;
}

const Vector2D & Transform::GetScale() const
{
	return scale;
}

const Vector2D Transform::GetWorldScale() const
{
	return parent ? scale + parent->GetScale() : scale;
}

void Transform::SetTranslation(const Vector2D & translation_)
{
	this->translation = translation_;
	isDirty = true;
}

void Transform::SetWorldTranslation(const Vector2D & translation_)
{
	SetTranslation(parent ? translation - parent->GetWorldTranslation() : translation);
}

void Transform::SetRotation(float rotation_)
{
	this->rotation = rotation_;
	isDirty = true;
}

void Transform::SetWorldRotation(float rotation_)
{
	SetRotation(parent ? rotation - parent->GetWorldRotation() : rotation);
}

void Transform::SetScale(const Vector2D & scale_)
{
	this->scale = scale_;
	isDirty = true;
}

void Transform::SetWorldScale(const Vector2D & scale_)
{
	SetScale(parent ? scale - parent->GetWorldScale() : scale);
}

void Transform::SetParent(Transform * transform)
{
	parent = transform;
}

void Transform::SetFollowCamera(bool fc)
{
       followCamera = fc;
}

void Transform::SetCamTranslation(const Vector2D & translation)
{
       camTranslation = translation;
       camIsDirty = true;
}

Vector2D Transform::GetCamTranslation()
{
       return camTranslation;
}

void Transform::SetCamIsDirty(bool isDirty)
{
       camIsDirty = isDirty;
}
