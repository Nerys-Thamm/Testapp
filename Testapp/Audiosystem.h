#pragma once
#include <fmod.hpp>
#include "GameObject.h"
#include <map>
#include <filesystem>
#include "Config.h"

class Audiosystem:
	public CGameObject
{
public:
	void AddSound(std::string _filename, std::string _name);
	void AddSound(std::string _filename, std::string _name, int _channel);
	void AddSoundFromYoutube(std::string _URL, std::string _name);
	void AddSoundFromYoutube(std::string _URL, std::string _name, int _channel);
	void RemoveSound(std::string _name);
	void PlaySound(std::string _name, float _volume = 1.0f, bool _loop = false);
	void PauseSound(std::string _name);
	static Audiosystem* GetInstance();
private:
	Audiosystem();
	static Audiosystem* m_instance;
	virtual void Update(float _fDeltaTime);
	FMOD::System* m_audioSystem = nullptr;
	FMOD::ChannelGroup* m_channelGroup = nullptr;
	std::map<std::string, FMOD::Sound*> m_sounds;
	std::map<FMOD::Sound*, FMOD::Channel*> m_channelBindings;
};

