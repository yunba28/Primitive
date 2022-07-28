#pragma once
#include "text.hpp"

class TextEx : public Eagle::Behavior {
public:
	struct CharData {
		Text* text = nullptr;
	};
public:
	TextEx();
	TextEx(const Eagle::String& _text, int _byteStep = 1);
	~TextEx();

	void SetText(const Eagle::String& _text, int _byteStep = 1);
	void SetFont(const Eagle::String& _fontName, unsigned int _fontSize);
	void SetCharSpace(float _charSpace);

	Eagle::DynamicArray<CharData>& GetCharaDataList();

private:
	Eagle::DynamicArray<CharData> text;
	UNIQUE(Font) font;
	float charSpace;

	void LateUpdate()override;

	void AlignedPosition();
};