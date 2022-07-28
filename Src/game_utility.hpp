#pragma once
#include <cstdlib>
#include "input.hpp"

//[summary]
//デバッグ中のみ実行されるクラス
class Debug {
private:
	Debug() = default;
	~Debug() = default;
public:

	template<typename ...Args>
	static void Log(const char* text, Args&& ...args)
	{
#if _DEBUG
		printfDx(text, std::forward<Args>(args)...);
#endif
	}

	template<typename ...Args>
	static void Log(int x, int y, unsigned int color, const char* text, Args&& ...args)
	{
#if _DEBUG
		DrawFormatString(x, y, color, text, std::forward<Args>(args)...);
#endif
	}

	static void Message(const char* text, const char* title, UINT messageType = MB_OK)
	{
#if _DEBUG
		MessageBox(NULL, _TEXT(text), _TEXT(title), messageType);
#endif
	}

	static void ErrorLog(const char* text, const char* title)
	{
#if _DEBUG
		MessageBox(NULL, _TEXT(text), _TEXT(title), MB_OK | MB_ICONERROR);
		std::exit(EXIT_FAILURE);
#endif
	}
};

enum class GameLevel {
	Easy = 0b0001,
	Normal = 0b0010,
	Hard = 0b0100,
	Debug = 0b1000
};

inline GameLevel operator|(GameLevel a, GameLevel b)
{
	return static_cast<GameLevel>(static_cast<int>(a) | static_cast<int>(b));
}

inline GameLevel operator&(GameLevel a, GameLevel b)
{
	return static_cast<GameLevel>(static_cast<int>(a) & static_cast<int>(b));
}

//[summary]
//ゲームの設定を保持
class GameConfig {
private:
	using Display = Eagle::Int2;
	using DisplayF = Eagle::Float2;

	GameConfig()
		: windowSize(800, 600)
		, resolution(800, 600)
		, titleName("The Ta.Chiki")
		, gameLevel(GameLevel::Normal)
	{

	}

	~GameConfig()
	{

	}

	static GameConfig* Instance()noexcept
	{
		return GameConfig::ptr;
	}
public:

	static void Initialize()noexcept
	{
		GameConfig::ptr = new GameConfig;
		Input::Start();
	}

	static void Finalize()noexcept
	{
		if (GameConfig::ptr)
			delete GameConfig::ptr;
		Input::End();
	}

	static void SetWindowSize(int w, int h)noexcept
	{
		Instance()->windowSize.Set(w, h);
	}

	static void SetResolution(int w, int h)noexcept
	{
		Instance()->resolution.Set(w, h);
	}

	static void SetTitleName(const char* name)noexcept
	{
		Instance()->titleName = name;
	}

	static void SetGameLevel(GameLevel level)noexcept
	{
		Instance()->gameLevel = level;
	}

	static Display GetWindowSize()noexcept
	{
		return Instance()->windowSize;
	}

	static DisplayF GetWindowSizeF()noexcept
	{
		return static_cast<DisplayF>(Instance()->windowSize);
	}

	static Display GetResolution()noexcept
	{
		return Instance()->resolution;
	}

	static DisplayF GetResolutionF()noexcept
	{
		return static_cast<DisplayF>(Instance()->resolution);
	}

	static const Eagle::String& GetTitleName()noexcept
	{
		return Instance()->titleName;
	}

	static GameLevel GetGameLevel()noexcept
	{
		return Instance()->gameLevel;
	}

	static bool IsEnableGameLevel(GameLevel level)noexcept
	{
		return static_cast<int>(Instance()->gameLevel & level) != 0;
	}

private:
	Display windowSize;
	Display resolution;
	Eagle::String titleName;
	GameLevel gameLevel;

	static GameConfig* ptr;
};

inline GameConfig* GameConfig::ptr = nullptr;

#define FIELD_MIN_X (float)(200.f)
#define FIELD_MIN_Y (float)(0.f  )

#define FIELD_MAX_X (float)(600.f)
#define FIELD_MAX_Y (float)(600.f)

//processを実行しない
#define PROCESS_OFF(process) if constexpr(0){process;}