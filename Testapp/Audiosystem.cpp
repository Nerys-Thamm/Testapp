#include "Audiosystem.h"

// ********************************************************************************
/// <summary>
/// Constructor, only accessible to self as it uses the singleton pattern
/// </summary>
/// <returns></returns>
// ********************************************************************************
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

// ********************************************************************************
/// <summary>
/// Adds a sound to the map of sounds
/// </summary>
/// <param name="_filename">The filename of the sound</param>
/// <param name="_name">The name of the sound, used as a key</param>
// ********************************************************************************
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

// ********************************************************************************
/// <summary>
/// Uses a commandline utility to download a sound from youtube if it doesnt already
/// exist.
/// </summary>
/// <param name="_URL">The youtube URL</param>
/// <param name="_name">The name of the sound, used as a key</param>
// ********************************************************************************
void Audiosystem::AddSoundFromYoutube(std::string _URL, std::string _name)
{
	if (!std::filesystem::exists(cfAUDIOPATH + _name + ".mp3")) //Check if the sound file already exists
	{
		system(("DL " + _URL + " " + _name).c_str()); //If not then run a batch script that downloads and extracts the audio data from youtube and converts to mp3 using FFMPEG
	}
	std::string name = _name + ".mp3";
	AddSound(name, _name); //Add the downloaded sound
}

// ********************************************************************************
/// <summary>
/// Removes a sound
/// </summary>
/// <param name="_name">The name of the sound to remove</param>
// ********************************************************************************
void Audiosystem::RemoveSound(std::string _name)
{
	m_sounds.at(_name)->release(); //Free the sound from memory

	//Erase the sound from the maps
	m_channelBindings.erase(m_channelBindings.find(m_sounds.at(_name)));
	m_sounds.erase(m_sounds.find(_name));
}

// ********************************************************************************
/// <summary>
/// Plays a sound
/// </summary>
/// <param name="_name">The name of the sound to play</param>
/// <param name="_volume">The volume of the sound</param>
/// <param name="_loop">Whether or not to loop the sound</param>
// ********************************************************************************
void Audiosystem::PlaySound(std::string _name, float _volume, bool _loop)
{
	FMOD::Channel* channel;

	//Attempt to play the sound
	if (m_audioSystem->playSound(m_sounds.at(_name), m_channelGroup, false, &channel) != FMOD_OK)
	{
		std::cout << "FMOD ERROR: Failed to Play sound [Playback Error]" << std::endl;
	}

	//Add the channel to the map
	m_channelBindings.emplace(m_sounds.at(_name), channel);

	//Set channel settings
	channel->setVolume(_volume);
	channel->setLoopCount(_loop ? 100 : 1);
	channel->setChannelGroup(m_channelGroup);
}

// ********************************************************************************
/// <summary>
/// TODO: Implement sound pausing
/// </summary>
/// <param name="_name"></param>
// ********************************************************************************
void Audiosystem::PauseSound(std::string _name)
{
}

Audiosystem* Audiosystem::m_instance = nullptr;

// ********************************************************************************
/// <summary>
/// Gets an instance of the Singleton
/// </summary>
/// <returns></returns>
// ********************************************************************************
Audiosystem* Audiosystem::GetInstance()
{
	if (m_instance == nullptr) { m_instance = new Audiosystem(); } //Creates an instance if one doesnt already exist
	return m_instance;
}

// ********************************************************************************
/// <summary>
/// This is called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void Audiosystem::Update(float _fDeltaTime)
{
	m_audioSystem->update();
}