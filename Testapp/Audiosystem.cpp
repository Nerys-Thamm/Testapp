#include "Audiosystem.h"

Audiosystem::Audiosystem()
{
	if (FMOD::System_Create(&m_audioSystem) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Audio System failed to create." << std::endl;
	}
	if (m_audioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Audio System failed to create." << std::endl;
	}
	m_audioSystem->createChannelGroup("Main", &m_channelGroup);
}

void Audiosystem::AddSound(std::string _filename, std::string _name)
{
	std::string path = cfAUDIOPATH + _filename;
	FMOD::Sound* newsound;
	
	if (m_audioSystem->createSound(path.c_str(), FMOD_DEFAULT, 0, &newsound) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Failed to load sound using createSound(...)" << std::endl;
	}
	else
	{
		m_sounds.emplace(_name, newsound);
		
	}
}

void Audiosystem::AddSound(std::string _filename, std::string _name, int _channel)
{
	std::string path = cfAUDIOPATH + _filename;
	FMOD::Sound* newsound;

	if (m_audioSystem->createSound(path.c_str(), FMOD_DEFAULT, 0, &newsound) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Failed to load sound using createSound(...)" << std::endl;
	}
	else
	{
		m_sounds.emplace(_filename, newsound);
		
	}
}

void Audiosystem::AddSoundFromYoutube(std::string _URL, std::string _name)
{
	if (!std::filesystem::exists(cfAUDIOPATH + _name + ".mp3"))
	{
		system(("DL " + _URL + " " + _name).c_str());
	}
	std::string name = _name + ".mp3";
	AddSound(name, _name);
}

void Audiosystem::AddSoundFromYoutube(std::string _URL, std::string _name, int _channel)
{
	if (!std::filesystem::exists(cfAUDIOPATH + _name + ".mp3"))
	{
		system(("DL " + _URL + " " + _name).c_str());
	}
	std::string name = _name + ".mp3";
	AddSound(name, _name, _channel);
}

void Audiosystem::RemoveSound(std::string _name)
{
	m_sounds.at(_name)->release();
	m_channelBindings.erase(m_channelBindings.find(m_sounds.at(_name)));
	m_sounds.erase(m_sounds.find(_name));
}

void Audiosystem::PlaySound(std::string _name, float _volume, bool _loop)
{
	FMOD::Channel* channel;
	
	
	if (m_audioSystem->playSound(m_sounds.at(_name), m_channelGroup, false, &channel) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Failed to Play sound [Playback Error]" << std::endl;
	}
	
	m_channelBindings.emplace(m_sounds.at(_name), channel);
	channel->setVolume(_volume);
	channel->setLoopCount(_loop ? 100 : 1);
	channel->setChannelGroup(m_channelGroup);
}

void Audiosystem::PauseSound(std::string _name)
{
	
}

Audiosystem* Audiosystem::m_instance = nullptr;

Audiosystem* Audiosystem::GetInstance()
{
	if (m_instance == nullptr) { m_instance = new Audiosystem(); }
	return m_instance;
}

void Audiosystem::Update(float _fDeltaTime)
{
	m_audioSystem->update();
}


