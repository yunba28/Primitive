#include "drawer.hpp"

CircleDrawer::CircleDrawer()
	: Material()
	, offset(0.f)
	, radius(5.f)
	, thickness(1.f)
	, fill(true)
{
}

CircleDrawer::CircleDrawer(float _radius, Eagle::Color _color)
	: Material()
	, offset(0.f)
	, radius(_radius)
	, thickness(1.f)
	, fill(true)
{
	SetColor(_color);
}

void CircleDrawer::SetOffset(float _offsetX, float _offsetY)
{
	offset.Set(_offsetX,_offsetY);
}

void CircleDrawer::SetRadius(float _radius)
{
	radius = _radius;
}

void CircleDrawer::SetThickness(float _thickness)
{
	thickness = _thickness;
}

void CircleDrawer::SetFill(bool flag)
{
	fill = flag;
}

float CircleDrawer::GetRadius() const noexcept
{
	return radius;
}

void CircleDrawer::Render()
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition()) + offset;
	Eagle::Vector2 scl = Eagle::ToVec2(GetTransform()->GetScale());

	if (scl == Eagle::Vector2::zero)
		return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));

	DrawCircleAA(pos.x, pos.y, radius * (scl.x + scl.y) * 0.5f, 12, GetColor(), fill, thickness * (scl.x + scl.y) * 0.5f);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

BoxDrawer::BoxDrawer()
	: Material()
	, offset(0.f)
	, extents1(5.f,5.f)
	, extents2(-5.f, 5.f)
	, thickness(1.f)
	, fill(true)
{
}

BoxDrawer::BoxDrawer(float _width, float _height, Eagle::Color _color)
	: Material()
	, offset(0.f)
	, extents1()
	, extents2()
	, thickness(1.f)
	, fill(true)
{
	SetSize(_width, _height);
	SetColor(_color);
}

void BoxDrawer::SetOffset(float _offsetX, float _offsetY)
{
	offset.Set(_offsetX, _offsetY);
}

void BoxDrawer::SetSize(float _width, float _height)
{
	extents1 = Eagle::Vector2(_width, _height).Abs() * 0.5f;
	extents2 = Eagle::Vector2(_width, _height).Abs() * 0.5f;
	extents2.x *= -1.f;
}

void BoxDrawer::SetThickness(float _thickness)
{
	thickness = _thickness;
}

void BoxDrawer::SetFill(bool flag)
{
	fill = flag;
}

Eagle::Vector2 BoxDrawer::GetSize() const noexcept
{
	return extents1 * 2.f;
}

void BoxDrawer::Render()
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition());
	Eagle::Vector2 scl = Eagle::ToVec2(GetTransform()->GetScale());

	if (scl == Eagle::Vector2::zero)
		return;

	//Eagle::Vector2 min, max;
	//min = pos - (extents1 * scl) + offset;
	//max = pos + (extents1 * scl) + offset;
	//
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));
	//
	//DrawBoxAA(min.x, min.y, max.x, max.y, GetColor(), fill, thickness * (scl.x + scl.y) * 0.5f);
	//
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	Eagle::Quaternion rot = GetTransform()->GetWorldRotation();
	Eagle::Vector2 ext[2];
	ext[0] = Eagle::ToVec2(rot.Rotate(Eagle::ToVec3(extents1)));
	ext[1] = Eagle::ToVec2(rot.Rotate(Eagle::ToVec3(extents2)));
	Eagle::Vector2 p[4];
	p[0] = pos + (ext[0] * scl) + offset;
	p[1] = pos - (ext[1] * scl) + offset;
	p[2] = pos - (ext[0] * scl) + offset;
	p[3] = pos + (ext[1] * scl) + offset;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));

	DrawQuadrangleAA(
		p[0].x, p[0].y,
		p[1].x, p[1].y,
		p[2].x, p[2].y,
		p[3].x, p[3].y,
		GetColor(),
		fill,
		thickness * (scl.x + scl.y) * 0.5f
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

CapsuleDrawer::CapsuleDrawer()
	: Material()
	, offset(0.f)
	, extents(0,-5)
	, height(10.f)
	, radius(5.f)
{
}

CapsuleDrawer::~CapsuleDrawer()
{
}

void CapsuleDrawer::SetOffset(float _offsetX, float _offsetY)
{
	offset.Set(_offsetX, _offsetY);
}

void CapsuleDrawer::SetAxis(Eagle::Vector2 axis)
{
	const float halfHeight = height * 0.5f;
	extents = axis.Normalize() * halfHeight;
}

void CapsuleDrawer::SetHeight(float height)
{
	this->height = height;
	const float halfHeight = height * 0.5f;
	extents = extents.Normalize() * halfHeight;
}

void CapsuleDrawer::SetRadius(float radius)
{
	this->radius = radius;
}

void CapsuleDrawer::SetState(Eagle::CapsuleCollider2D* collider)
{
	extents = collider->GetExtents();
	height = extents.Magnitude() * 2.f;
	radius = collider->GetRadius();
}

const Eagle::Vector2& CapsuleDrawer::GetExtents() const noexcept
{
	return extents;
}

const float CapsuleDrawer::GetHeight() const noexcept
{
	return height;
}

const float CapsuleDrawer::GetRadius() const noexcept
{
	return radius;
}

void CapsuleDrawer::Render()
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition());
	Eagle::Vector2 scl = Eagle::ToVec2(GetTransform()->GetScale());

	if (scl == Eagle::Vector2::zero)
		return;

	const Eagle::Quaternion rot = GetTransform()->GetWorldRotation();

	Eagle::Vector2 pos1, pos2;
	pos1 = GetTop(rot);
	pos2 = GetBottom(rot);

	float aveScl = ((scl.x + scl.y) * 0.5f);

	float sRadius = radius * aveScl;

	int posNum = static_cast<int>(radius) * static_cast<int>(aveScl);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));

	DrawLineAA(pos1.x, pos1.y, pos2.x, pos2.y, GetColor(), sRadius * 2.05f);
	DrawCircleAA(pos1.x, pos1.y, sRadius, posNum, GetColor(), true);
	DrawCircleAA(pos2.x, pos2.y, sRadius, posNum, GetColor(), true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

Eagle::Vector2 CapsuleDrawer::GetTop(const Eagle::Quaternion& rot) const noexcept
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition());
	Eagle::Vector2 scl = Eagle::ToVec2(GetTransform()->GetScale());

	Eagle::Vector2 ext = Eagle::ToVec2(rot.Rotate(Eagle::ToVec3(extents)));

	return pos + (ext * scl) + offset;
}

Eagle::Vector2 CapsuleDrawer::GetBottom(const Eagle::Quaternion& rot) const noexcept
{
	Eagle::Vector2 pos = Eagle::ToVec2(GetTransform()->GetWorldPosition());
	Eagle::Vector2 scl = Eagle::ToVec2(GetTransform()->GetScale());

	Eagle::Vector2 ext = Eagle::ToVec2(rot.Rotate(Eagle::ToVec3(extents)));

	return pos - (ext * scl) + offset;
}
