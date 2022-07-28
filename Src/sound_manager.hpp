#pragma once
#include "eagle.hpp"

class SoundManager {
private:
	SoundManager();
	~SoundManager() = default;

	static SoundManager& Get();
	static SoundManager* unique;
public:
	static void Initialize();
	static void Finalize();

	static int Load(const Eagle::String& _fileName, int bufMax = 3);
	static int Play(const Eagle::String& _fileName, int _soundType, float _volume, bool _moveTop = true);
	static int Stop(const Eagle::String& _fileName);
	static void SetVolumeRate(float _volRate);
	static bool IsPlaying(const Eagle::String& _fileName);
	static bool IsExist(const Eagle::String& _fileName);

private:
	Eagle::HashMap<Eagle::String, int> soundMap;
	Eagle::String rootPath;
	float volumeRate;
};