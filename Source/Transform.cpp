#include "stdafx.h"
#include "Transform.h"

/*Vector2D Transform::camTranslation = { 0, 0 };
Vector2D Transform::camScale = { 1, 1 };
bool Transform::camIsDirty = false;*/

Transform::Transform() : Transform(0.f, 0.f) {
}

Transform::Transform(float x, float y) :
		Component("Transform"), translation(x, y), rotation(0), scale(1, 1), isDirty(true), followCamera(true)
{
}

Transform::Transform(Vector2D pos, Vector2D scale, float rot) :
		Component("Transform"), translation(pos), scale(scale), rotation(rotation), isDirty(true), followCamera(true) {
}

Component * Transform::Clone() const
{
	return new Transform(*this);
}

Matrix2D & Transform::GetMatrix()
{
	if (isDirty/* || (followCamera && camIsDirty)*/) {
		matrix = Matrix2D::TranslationMatrix(translation.X(), translation.Y()) *
				 Matrix2D::RotationMatrixRadians(rotation) *
				 Matrix2D::ScalingMatrix(scale.X(), scale.Y());
		isDirty = false;
	}
	/*if (followCamera && camIsDirty)
        matrix = Matrix2D().TranslationMatrix(camTranslation.X(), camTranslation.Y()) * matrix;*/
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

/*const Vector2D Transform::GetScreenTranslation() const
{
	return GetWorldTranslation() + (followCamera ? -camTranslation : Vector2D(0, 0));
}*/

float Transform::GetRotation() const
{
	return rotation;
}

float Transform::GetWorldRotation() const
{
	return parent ? rotation + parent->GetRotation() : rotation;
}

/*float Transform::GetScreenRotation() const
{
	return GetWorldRotation();
}*/

const Vector2D & Transform::GetScale() const
{
	return scale;
}

const Vector2D Transform::GetWorldScale() const
{
	Vector2D scl = scale;
	if (parent) {
		scl.x *= parent->GetScale().x;
		scl.y *= parent->GetScale().y;
	}
	return scl;
}

/*const Vector2D Transform::GetScreenScale() const
{
	Vector2D scl = GetWorldScale();
	if (followCamera) {
		scl.x /= camScale.x;
		scl.y /= camScale.y;
	}
	return scl;
}*/

void Transform::SetTranslation(const Vector2D & translation_)
{
	this->translation = translation_;
	isDirty = true;
}

void Transform::SetWorldTranslation(const Vector2D & translation_)
{
	SetTranslation(parent ? translation_ - parent->GetWorldTranslation() : translation_);
}

/*void Transform::SetScreenTranslation(const Vector2D & translation_)
{
	SetWorldTranslation(translation_ - (followCamera ? -camTranslation : Vector2D(0, 0)));
}*/

void Transform::SetRotation(float rotation_)
{
	this->rotation = rotation_;
	isDirty = true;
}

void Transform::SetWorldRotation(float rotation_)
{
	SetRotation(parent ? rotation_ - parent->GetWorldRotation() : rotation_);
}

/*void Transform::SetScreenRotation(float rotation_)
{
	SetWorldRotation(rotation_);
}*/

void Transform::SetScale(const Vector2D & scale_)
{
	this->scale = scale_;
	isDirty = true;
}

void Transform::SetWorldScale(const Vector2D & scale_)
{
	Vector2D scl = scale_;
	if (parent) {
		scl.x /= parent->GetWorldScale().x;
		scl.y /= parent->GetWorldScale().y;
	}
	SetScale(scl);
	//SetScale(parent ? scale_ - parent->GetWorldScale() : scale_);
}

/*void Transform::SetScreenScale(const Vector2D & scale_)
{
	Vector2D scl = scale_;
	if (followCamera) {
		scl.x *= camScale.x;
		scl.y *= camScale.y;
	}
	SetWorldScale(scl);
}*/

void Transform::SetParent(Transform * transform)
{
	parent = transform;
}

void Transform::SetFollowCamera(bool fc)
{
       followCamera = fc;
}

/*void Transform::SetCamTranslation(const Vector2D & translation)
{
       camTranslation = translation;
       camIsDirty = true;
}*/

/*Vector2D Transform::GetCamTranslation()
{
       return camTranslation;
}*/

/*void Transform::SetCamIsDirty(bool isDirty)
{
       camIsDirty = isDirty;
}*/

void Transform::Load(rapidjson::Value& obj)
{
	using namespace rapidjson;

	Value* tmp;// = &(obj["Translation"]);

	if (obj.HasMember("Translation") && (tmp=&(obj["Translation"]))->IsArray() && tmp->Size() == 2)
		SetTranslation(Vector2D(tmp->GetArray()[0].GetFloat(), tmp->GetArray()[1].GetFloat()));

	if (obj.HasMember("Scale") && (tmp=&(obj["Scale"]))->IsArray() && tmp->Size() == 2)
		SetScale(Vector2D(tmp->GetArray()[0].GetFloat(), tmp->GetArray()[1].GetFloat()));

	if (obj.HasMember("Rotation") && (tmp=&(obj["Rotation"]))->IsFloat())
		SetRotation(tmp->GetFloat());
}
