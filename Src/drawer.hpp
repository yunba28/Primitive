#pragma once
#include "game_utility.hpp"
#include "eagle/include/material.hpp"

//円を描画するためのコンポーネント
class CircleDrawer : public Eagle::Material {
public:
	CircleDrawer();
	CircleDrawer(float _radius, Eagle::Color _color);
	~CircleDrawer() = default;

	void SetOffset(float _offsetX, float _offsetY);
	void SetRadius(float _radius);
	void SetThickness(float _thickness);
	void SetFill(bool flag);

	float GetRadius()const noexcept;

private:
	Eagle::Vector2 offset;
	float radius;
	float thickness;
	bool fill;

	void Render()override;
};

//辺が軸に平行な矩形の描画
class BoxDrawer : public Eagle::Material {
public:
	BoxDrawer();
	BoxDrawer(float _width, float _height, Eagle::Color _color);
	~BoxDrawer() = default;

	void SetOffset(float _offsetX, float _offsetY);

	void SetSize(float _width, float _height);

	void SetThickness(float _thickness);

	void SetFill(bool flag);

	Eagle::Vector2 GetSize()const noexcept;

private:
	Eagle::Vector2 offset;
	Eagle::Vector2 extents1;
	Eagle::Vector2 extents2;
	float thickness;
	bool fill;

	void Render()override;
};

class CapsuleDrawer : public Eagle::Material {
public:
	CapsuleDrawer();
	~CapsuleDrawer();

	void SetOffset(float _offsetX, float _offsetY);

	void SetAxis(Eagle::Vector2 axis);

	void SetHeight(float height);

	void SetRadius(float radius);

	void SetState(Eagle::CapsuleCollider2D* collider);

	const Eagle::Vector2& GetExtents()const noexcept;
	const float GetHeight()const noexcept;
	const float GetRadius()const noexcept;

private:
	Eagle::Vector2 offset;
	Eagle::Vector2 extents;
	float height;
	float radius;

	void Render()override;

	Eagle::Vector2 GetTop(const Eagle::Quaternion& rot)const noexcept;

	Eagle::Vector2 GetBottom(const Eagle::Quaternion& rot)const noexcept;
};