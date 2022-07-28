#pragma once
#include "eagle.hpp"

//[summary]
//�t�H���g����ێ�����\����
class Font {
private:
	Font();

public:
	~Font();

	//Font�𐶐�����
	static Font* Create(Eagle::String fontName, unsigned int fontSize);
	//Font��j������
	static void Destroy(Font* font);
	//Font�𕡐�
	static Font* Duplicate(const Font& font);

	//�ȑO�̃t�H���g��j�����ĐV���ȃt�H���g���쐬����
	void Recreate(Eagle::String fontName, unsigned int fontSize);

	//Font�n���h�����擾
	int GetHandle()const noexcept;

	//Font�̃T�C�Y���擾
	unsigned int GetFontSize()const noexcept;

	//Font�̖��O���擾
	Eagle::Name GetFontName()const noexcept;

private:
	int handle;
	unsigned int size;
	Eagle::String name;

	friend class std::unique_ptr<Font>;
	friend class std::shared_ptr<Font>;

private:
	static bool CreateFontImpl(Font& font, Eagle::String fontName, unsigned int fontSize)noexcept;
};

enum class DrawPosition {
	Center,
	Left,
	Right
};

void DrawString(const Eagle::Vector3& pos, const char* text, unsigned color, DrawPosition dPos = DrawPosition::Center);
void DrawString(const Eagle::Vector3& pos, const char* text, int fontHandle, unsigned color, DrawPosition dPos = DrawPosition::Center);
void DrawString(const Eagle::Vector3& pos, const Eagle::String& text, unsigned color, DrawPosition dPos = DrawPosition::Center);
void DrawString(const Eagle::Vector3& pos, const Eagle::String& text, int fontHandle, unsigned color, DrawPosition dPos = DrawPosition::Center);