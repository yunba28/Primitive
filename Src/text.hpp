#pragma once
#include "eagle.hpp"
#include "font.hpp"

//[summary]
//ï“èWâ¬î\Ç»ï∂èÕ
class Text : public Eagle::Material{
public:
	Text();
	Text(const Eagle::String& _text);
	~Text();

	void SetText(const Eagle::String& _text);
	void SetFont(const Eagle::String& _fontName, unsigned int _fontSize);
	void SetFont(const Font& _font);
	void SetDrawPosition(DrawPosition _drawPos);

	int GetDrawLength()const noexcept;

	std::size_t GetLength()const noexcept;

private:
	Eagle::String text;
	UNIQUE(Font) font;
	DrawPosition drawPosition;

	void Render()override;
};