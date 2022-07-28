#include "text_ex.hpp"
#include "game_utility.hpp"

TextEx::TextEx()
	: Behavior(ID::LateUpdate)
	, text()
	, font(nullptr)
	, charSpace(0.f)
{
}

TextEx::TextEx(const Eagle::String& _text, int _byteStep)
	: Behavior(ID::LateUpdate)
	, text()
	, font(nullptr)
	, charSpace(0.f)
{
	SetText(_text, _byteStep);
}

TextEx::~TextEx()
{
}

void TextEx::SetText(const Eagle::String& _text, int _byteStep)
{
	for (auto& data : text)
	{
		Eagle::GameObject::Destroy(data.text->GetGameObject());
	}
	int idx = 0;
	const int length = static_cast<int>(_text.length());
	for (idx; idx < length; idx += _byteStep)
	{
		CharData data;
		data.text = Eagle::GameObject::Create()->AttachComponent<Text>(_text.substr(idx, _byteStep));
		data.text->SetDrawPosition(DrawPosition::Left);
		data.text->GetTransform()->SetPosition(GetTransform()->GetWorldPosition());
		text.Add(data);
	}
	if (font)
	{
		for (auto& data : text)
		{
			data.text->SetFont(*font);
		}
	}
	AlignedPosition();
}

void TextEx::SetFont(const Eagle::String& _fontName, unsigned int _fontSize)
{
	if (font)
	{
		Font::Destroy(font.release());
	}

	font.reset(Font::Create(_fontName, _fontSize));

	for (auto& data : text)
	{
		data.text->SetFont(*font);
	}

	AlignedPosition();
}

void TextEx::SetCharSpace(float _charSpace)
{
	charSpace = _charSpace;
}

Eagle::DynamicArray<TextEx::CharData>& TextEx::GetCharaDataList()
{
	return text;
}

void TextEx::LateUpdate()
{
	AlignedPosition();
}

void TextEx::AlignedPosition()
{
	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();
	float offset = 0.f;
	float width = 0.f;
	for (auto& data : text)
	{
		width = data.text->GetDrawLength();
		Eagle::Vector3 charPos = pos;
		charPos.x += offset;
		charPos.y = data.text->GetTransform()->GetWorldPosition().y;
		data.text->GetTransform()->SetPosition(charPos);
		offset += width + charSpace;
	}
}
