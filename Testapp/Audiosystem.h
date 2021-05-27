#pragma once
#include <fmod.hpp>
#include "GameObject.h"
#include <map>
class Audiosystem
{
public:
	Audiosystem();
	void AddSound(std::string _filename, std::string _name);
	void RemoveSound(std::string _name);
	void PlaySound(std::string _name);
	void PauseSound(std::string _name);
private:
	FMOD::System* m_audioSystem;
	std::map<FMOD::Sound*, std::string> m_sounds;
};

