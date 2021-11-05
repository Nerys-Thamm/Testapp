// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Config.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "Config.h"

// ********************************************************************************
/// <summary>
/// Checks whether a flag in the config.ini file is true or false
/// </summary>
/// <param name="_flag_name"></param>
/// <returns></returns>
// ********************************************************************************
bool Config::GetFlag(std::string _flag_name)
{
	if (!m_loaded) { ReadData(); }
	return m_flags[_flag_name];
}

// ********************************************************************************
/// <summary>
/// Sets a flag to true or false in the config.ini file
/// </summary>
/// <param name="_flag_name"></param>
/// <param name="_value"></param>
// ********************************************************************************
void Config::SetFlag(std::string _flag_name, bool _value)
{
	if (!m_loaded) { ReadData(); }
	m_flags[_flag_name] = _value;
	WriteData();
}

// ********************************************************************************
/// <summary>
/// Gets the height of the window from the config.ini file
/// </summary>
/// <returns></returns>
// ********************************************************************************
float Config::GetWindowHeight()
{
	if (!m_loaded) { ReadData(); }
	return m_window_height;
}

// ********************************************************************************
/// <summary>
/// Gets the width of the window from the config.ini file
/// </summary>
/// <returns></returns>
// ********************************************************************************
float Config::GetWindowWidth()
{
	if (!m_loaded) { ReadData(); }
	return m_window_width;
}

bool Config::m_loaded = false;
float Config::m_window_height = 0;
float Config::m_window_width = 0;
std::map<std::string, bool> Config::m_flags;

// ********************************************************************************
/// <summary>
/// Reads the data of the config.ini file into memory
/// </summary>
// ********************************************************************************
void Config::ReadData()
{
	m_flags.clear();
	std::ifstream Reader; //File reader
	Reader.open(cfFILE_PATH "CONFIG.INI"); //Open file
	if (Reader.fail())//Create file if it doesnt exist
	{
		std::cout << "Config file: MISSING" << std::endl << "Creating new Config file..." << std::endl;
		fs::create_directories(cfFILE_PATH);
		std::ofstream Writer(cfFILE_PATH "CONFIG.INI");
		Writer << "#PROGRAM CONFIGURATION" << std::endl << std::endl << "#WINDOW_SETTINGS:" << std::endl << "<Window_Width:1920>" << std::endl << "<Window_Height:1080>" << std::endl << "#----------------" << std::endl << std::endl << "#FLAGS:" << std::endl << "<ENDFILE>";
		Writer.close();
		Reader.open(cfFILE_PATH "CONFIG.INI");
	}
	bool bReading = true;

	std::cout << "Loading Config File...";

	//Args for Tags
	std::string Arg1;
	std::string Arg2;
	std::string Arg3;
	std::string Temp;

	while (bReading)//While reading
	{
		std::string line;
		std::string tag;
		std::getline(Reader, line); //Get a line from the file
		if (line == "" || line[0] == '#') //Skip the line if its empty of starts with the comment character #
		{
			continue;
		}

		std::istringstream stream(line); //Make a stringstream from the line
		std::getline(stream, Temp, '<'); //Dump all characters before the first argument
		std::getline(stream, tag, '>'); //Get all text within the angle brackets to parse
		std::istringstream tagstream(tag); //create stringstream from tag

		std::cout << ".";

		if (tag == "ENDFILE")//If the argument is ENDFILE
		{
			bReading = false; //Stop reading
			std::cout << "...Complete!" << std::endl;
			break;
		}

		std::getline(tagstream, Arg1, ':'); //Get the 1st Arg

		if (Arg1 == "Window_Width")
		{
			std::getline(tagstream, Arg2);
			m_window_width = std::stof(Arg2);
		}
		else if (Arg1 == "Window_Height")
		{
			std::getline(tagstream, Arg2);
			m_window_height = std::stof(Arg2);
		}
		else
		{
			std::getline(tagstream, Arg2);
			m_flags.insert(std::pair<std::string, bool>(Arg1, (Arg2 == "true")));
		}
	}
	std::cout << "Configuration File Loaded" << std::endl;
	Reader.close(); //Close file
	m_loaded = true;
}

// ********************************************************************************
/// <summary>
/// Writes the data stored in memory into the config.ini file
/// </summary>
// ********************************************************************************
void Config::WriteData()
{
	std::ofstream Writer;
	Writer.open(cfFILE_PATH "CONFIG.INI");
	Writer << "#DATA FILE" << std::endl << std::endl << "#WINDOW_SETTINGS:" << std::endl << "<Window_Width:" + std::to_string(m_window_width) + ">" << std::endl << "<Window_Height:" + std::to_string(m_window_height) + ">" << std::endl << "#----------------" << std::endl << std::endl << "#FLAGS:" << std::endl;
	for (std::pair<std::string, bool> pair : m_flags)
	{
		Writer << "<" + pair.first + ":" + (pair.second ? "true" : "false") + ">" << std::endl;
	}
	Writer.close();
}