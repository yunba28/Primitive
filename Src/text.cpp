#include "text.hpp"
#include "DxLib.h"

Text::Text()
	: text("")
	, font(nullptr)
	, drawPosition(DrawPosition::Center)
{
}

Text::Text(const Eagle::String& _text)
	: text(_text)
	, font(nullptr)
	, drawPosition(DrawPosition::Center)
{
}

Text::~Text()
{
}

void Text::SetText(const Eagle::String& _text)
{
	text = _text;
}

void Text::SetFont(const Eagle::String& _fontName, unsigned int _fontSize)
{
	if (font)
	{
		Font::Destroy(font.release());
	}

	font.reset(Font::Create(_fontName, _fontSize));
}

void Text::SetFont(const Font& _font)
{
	font.reset(Font::Duplicate(_font));
}

void Text::SetDrawPosition(DrawPosition _drawPos)
{
	drawPosition = _drawPos;
}

int Text::GetDrawLength() const noexcept
{
	return (font)
		? GetDrawStringWidthToHandle(text.c_str(), static_cast<int>(text.length()), font->GetHandle())
		: GetDrawStringWidth(text.c_str(), static_cast<int>(text.length()));
}

std::size_t Text::GetLength() const noexcept
{
	return text.length();
}

void Text::Render()
{
	Eagle::Vector3 pos = GetTransform()->GetWorldPosition();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(GetTransparency() * 255.f));
	(!font)
		? DrawString(pos, text, GetColor(), drawPosition)
		: DrawString(pos, text, font->GetHandle(), GetColor(), drawPosition);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
