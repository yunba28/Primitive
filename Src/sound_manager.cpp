#include "sound_manager.hpp"
#include "DxLib.h"
#include <filesystem>

SoundManager* SoundManager::unique = nullptr;

SoundManager::SoundManager()
	: soundMap()
	, rootPath()
	, volumeRate(1.f)
{

}

SoundManager& SoundManager::Get()
{
	return *SoundManager::unique;
}

void SoundManager::Initialize()
{
	if (!SoundManager::unique)
		SoundManager::unique = new SoundManager;

	auto path = std::filesystem::current_path();
	path += "\\Data\\";
	SoundManager::Get().rootPath = path.string();
}

void SoundManager::Finalize()
{
	InitSoundMem();

	if (SoundManager::unique)
		delete SoundManager::unique;
}

int SoundManager::Load(const Eagle::String& _fileName, int bufMax)
{
	if (SoundManager::IsExist(_fileName))
	{
		return SoundManager::Get().soundMap.At(_fileName);
	}
	auto& sm = SoundManager::Get();
	Eagle::String path = sm.rootPath + _fileName;
	int handle = LoadSoundMem(path.c_str(), bufMax);
	if (handle < 0)
	{
		return -1;
	}
	sm.soundMap[_fileName] = handle;
	return handle;
}

int SoundManager::Play(const Eagle::String& _fileName, int _soundType, float _volume, bool _moveTop)
{
	if (!SoundManager::IsExist(_fileName))
	{
		return -1;
	}
	int vol = Eagle::Math::Clamp(static_cast<int>(_volume * SoundManager::Get().volumeRate * 255.f), 0, 255);
	int handle = SoundManager::Get().soundMap.At(_fileName);
	ChangeVolumeSoundMem(vol, handle);
	return PlaySoundMem(handle, _soundType, _moveTop);
}

int SoundManager::Stop(const Eagle::String& _fileName)
{
	if (!SoundManager::IsExist(_fileName))
	{
		return -1;
	}
	int handle = SoundManager::Get().soundMap.At(_fileName);
	return StopSoundMem(handle);
}

void SoundManager::SetVolumeRate(float _volRate)
{
	SoundManager::Get().volumeRate = Eagle::Math::Clamp01(_volRate);
}

bool SoundManager::IsPlaying(const Eagle::String& _fileName)
{
	if (!SoundManager::IsExist(_fileName))
	{
		return false;
	}
	int handle = SoundManager::Get().soundMap.At(_fileName);
	return CheckSoundMem(handle);
}

bool SoundManager::IsExist(const Eagle::String& _fileName)
{
	return SoundManager::Get().soundMap.Find(_fileName) != SoundManager::Get().soundMap.end();
}
