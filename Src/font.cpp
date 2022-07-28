#include "font.hpp"
#include "DxLib.h"

Font::Font()
	: handle(-1)
	, size(16)
	, name("")
{
}

Font::~Font()
{
	if (handle != -1)
	{
		DeleteFontToHandle(handle);
	}
}

Font* Font::Create(Eagle::String fontName, unsigned int fontSize)
{
	Font* font = new Font;

	if (!CreateFontImpl(*font, fontName, fontSize))
	{
		delete font;
		return nullptr;
	}

	return font;
}

void Font::Destroy(Font* font)
{
	if (!font)
		return;

	if (font->handle == -1)
		return;

	DxLib::DeleteFontToHandle(font->handle);

	font->handle = -1;

	delete font;

	font = nullptr;
}

Font* Font::Duplicate(const Font& font)
{
	return new Font(font);
}

void Font::Recreate(Eagle::String fontName, unsigned int fontSize)
{
	if (handle > 0)
	{
		DxLib::DeleteFontToHandle(handle);
		handle = -1;
	}

	CreateFontImpl(*this, fontName, fontSize);
}

int Font::GetHandle() const noexcept
{
	return handle;
}

unsigned int Font::GetFontSize() const noexcept
{
	return size;
}

Eagle::Name Font::GetFontName() const noexcept
{
	return name;
}

bool Font::CreateFontImpl(Font& font, Eagle::String fontName, unsigned int fontSize) noexcept
{
	font.handle = DxLib::CreateFontToHandle(fontName.c_str(), fontSize, 1, DX_FONTTYPE_ANTIALIASING_8X8);

	//ÉtÉHÉìÉgÇÃê∂ê¨Ç…é∏îs
	if (font.handle == -1)
	{
		return false;
	}

	font.size = fontSize;
	font.name = fontName;

	return true;
}

void DrawString(const Eagle::Vector3& pos, const char* text, unsigned color, DrawPosition dPos)
{
	const float halfLength = static_cast<float>(DxLib::GetDrawStringWidth(text, static_cast<int>(std::strlen(text))) / 2);
	const float halfHeight = static_cast<float>(DxLib::GetFontSize() / 2);

	if (dPos == DrawPosition::Center)
		DxLib::DrawStringF(pos.x - halfLength, pos.y - halfHeight, text, color);
	else if (dPos == DrawPosition::Left)
		DxLib::DrawStringF(pos.x, pos.y - halfHeight, text, color);
	else if (dPos == DrawPosition::Right)
		DxLib::DrawStringF(pos.x - halfLength * 2.f, pos.y - halfHeight, text, color);
}

void DrawString(const Eagle::Vector3& pos, const char* text, int fontHandle, unsigned color, DrawPosition dPos)
{
	const float halfLength = static_cast<float>(DxLib::GetDrawStringWidthToHandle(text, static_cast<int>(std::strlen(text)), fontHandle) / 2);
	const float halfHeight = static_cast<float>(DxLib::GetFontSizeToHandle(fontHandle) / 2);

	if (dPos == DrawPosition::Center)
		DxLib::DrawStringFToHandle(pos.x - halfLength, pos.y - halfHeight, text, color, fontHandle);
	else if (dPos == DrawPosition::Left)
		DxLib::DrawStringFToHandle(pos.x, pos.y - halfHeight, text, color, fontHandle);
	else if (dPos == DrawPosition::Right)
		DxLib::DrawStringFToHandle(pos.x - halfLength * 2.f, pos.y - halfHeight, text, color, fontHandle);
}

void DrawString(const Eagle::Vector3& pos, const Eagle::String& text, unsigned color, DrawPosition dPos)
{
	DrawString(pos, text.c_str(), color, dPos);
}

void DrawString(const Eagle::Vector3& pos, const Eagle::String& text, int fontHandle, unsigned color, DrawPosition dPos)
{
	DrawString(pos, text.c_str(), fontHandle, color, dPos);
}