#include "button.hpp"

#include "eagle/tween.hpp"

#include "text.hpp"
#include "drawer.hpp"

#include "sound_manager.hpp"

Button::Button()
	: Behavior(ID::Update)
	, box(nullptr)
	, text(nullptr)
	, collider(nullptr)
	, onPressed(nullptr)
	, oldMousePos()
	, color{}
	, hoverActiveFlag(0b11)
	, isFocus(false)
{
}

Button::~Button()
{
}

void Button::SetOnPressed(std::function<void(void)> func)
{
	onPressed = func;
}

void Button::SetStayColor(Eagle::Color _color)
{
	color.stay = _color;
	if (!isFocus)
		text->SetColor(_color);
}

void Button::SetHoverColor(Eagle::Color _color)
{
	color.hover = _color;
	if (isFocus)
		text->SetColor(_color);
}

void Button::SetUseFadeColor(bool flag)
{
	(flag) 
		? Eagle::BitOn(hoverActiveFlag, 0) 
		: Eagle::BitOff(hoverActiveFlag, 0);
}

void Button::SetUseFadeScale(bool flag)
{
	(flag)
		? Eagle::BitOn(hoverActiveFlag, 1)
		: Eagle::BitOff(hoverActiveFlag, 1);
}

void Button::OnFocus()
{
	if (isFocus)
		return;

	isFocus = true;

	if (hoverActiveFlag & 0b10)
	{
		GetTransform()->DoScale(Eagle::Vector3(1.03f), 0.1f);
	}

	if (hoverActiveFlag & 0b01)
	{
		box->DoColor(color.hover, 0.1f);
		text->DoColor(color.hover, 0.1f);
	}

	SoundManager::Play("se_select10.mp3", DX_PLAYTYPE_BACK, 0.75f);
}

void Button::OffFocus()
{
	if (!isFocus)
		return;

	isFocus = false;

	if (hoverActiveFlag & 0b10)
	{
		GetTransform()->DoScale(Eagle::Vector3::one, 0.1f);
	}

	if (hoverActiveFlag & 0b01)
	{
		box->DoColor(color.stay, 0.1f);
		text->DoColor(color.stay, 0.1f);
	}
}

Text* Button::GetText() const noexcept
{
	return text;
}

BoxDrawer* Button::GetBoxDrawer() const noexcept
{
	return box;
}

void Button::Awake()
{
	collider = GetGameObject()->AttachComponent<Eagle::BoxCollider2D>();
	
	if (!onPressed)
		onPressed = []() {};
}

void Button::Start()
{
	if(!text)
		text = GetGameObject()->GetComponent<Text>();
	if(!box)
		box = GetGameObject()->GetComponent<BoxDrawer>();

	Eagle::Vector2 size = box->GetSize();
	collider->SetSize(size.x, size.y);
}

void Button::Update()
{
	const Eagle::Rectf rect = collider->GetRect();
	const Eagle::Vector2 LT(rect.left, rect.top);
	const Eagle::Vector2 RB(rect.right, rect.bottom);

	const Eagle::Vector2 mPos = Input::GetMousePosition();

	//マウスポインタが動いていたら処理
	if (mPos != oldMousePos)
	{
		Eagle::Math::IsBetween(mPos, LT, RB) ? OnFocus() : OffFocus();
	}

	oldMousePos = mPos;

	//そもそも押されてないから処理しない
	if (!(Input::GetDown(Input::Key::Return) || Input::GetDown(Input::Mouse::Left) || Input::GetDown(Input::Pad::A)))
		return;

	if (isFocus)
	{
		OnPress();
	}
}

void Button::OnPress()
{
	auto& tween = GetTransform()->DoScale(Eagle::Vector3(1.1f), 0.2f);
	tween.SetEase(Eagle::Ease::Quint::In);
	tween.SetOnComplete([&]()
		{
			GetTransform()->DoScale(Eagle::Vector3::one, 0.2f).SetEase(Eagle::Ease::Quint::Out)
				.SetOnComplete(onPressed);
		}
	);
	SoundManager::Play("se_pikon5.mp3", DX_PLAYTYPE_BACK, 0.75f);
}

Button* MakeButton(
	Eagle::String objectName,
	Eagle::String drawText,
	Eagle::String fontName, 
	unsigned int fontSize, 
	float boxWidth, 
	float boxHeight)
{
	auto obj = Eagle::GameObject::Create(objectName);
	auto btn = obj->AttachComponent<Button>();
	btn->box = obj->AttachComponent<BoxDrawer>(boxWidth, boxHeight, 0xffffff);
	btn->box->SetFill(false);
	btn->text = obj->AttachComponent<Text>(drawText);
	btn->text->SetFont(fontName, fontSize);
	return btn;
}
