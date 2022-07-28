#pragma once
#include "eagle.hpp"

//[summary]
//フォント情報を保持する構造体
class Font {
private:
	Font();

public:
	~Font();

	//Fontを生成する
	static Font* Create(Eagle::String fontName, unsigned int fontSize);
	//Fontを破棄する
	static void Destroy(Font* font);
	//Fontを複製
	static Font* Duplicate(const Font& font);

	//以前のフォントを破棄して新たなフォントを作成する
	void Recreate(Eagle::String fontName, unsigned int fontSize);

	//Fontハンドルを取得
	int GetHandle()const noexcept;

	//Fontのサイズを取得
	unsigned int GetFontSize()const noexcept;

	//Fontの名前を取得
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