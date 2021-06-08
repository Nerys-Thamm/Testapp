// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : Audiosystem.h
// Description : Header file for the audiosystem class
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#pragma once
#include <fmod.hpp>
#include "GameObject.h"
#include <map>
#include <filesystem>
#include "Config.h"

// --------------------------------------------------------------------------------
/// <summary>
///  System for game audio
/// </summary>
// --------------------------------------------------------------------------------
class Audiosystem :
	public CGameObject
{
public:
	void AddSound(std::string _filename, std::string _name);
	void AddSoundFromYoutube(std::string _URL, std::string _name);
	void RemoveSound(std::string _name);
	void PlaySound(std::string _name, float _volume = 1.0f, bool _loop = false);
	void PauseSound(std::string _name); //TODO: Implement this
	static Audiosystem* GetInstance();
private:
	Audiosystem();
	static Audiosystem* m_instance; //The instance of the singleton
	virtual void Update(float _fDeltaTime);
	FMOD::System* m_audioSystem = nullptr;
	FMOD::ChannelGroup* m_channelGroup = nullptr;
	std::map<std::string, FMOD::Sound*> m_sounds;
	std::map<FMOD::Sound*, FMOD::Channel*> m_channelBindings;
};
