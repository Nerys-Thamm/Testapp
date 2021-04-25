#pragma once
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <map>
#define cfFILE_PATH "Resources/Data/"
#define cfWINDOW_WIDTH() Config::GetWindowWidth()
#define cfWINDOW_HEIGHT() Config::GetWindowHeight()
#define cfFLAG(var) Config::GetFlag(var)
#define cfSETFLAG(var, var2) Config::SetFlag(var, var2)

namespace fs = std::filesystem;

class Config
{
	//TODO: Make static config class that reads and writes to INI file
public:
	static bool GetFlag(std::string _flag_name);
	static void SetFlag(std::string _flag_name, bool _value);
	static float GetWindowHeight();
	static float GetWindowWidth();;
private:
	Config();
	static void ReadData();
	static void WriteData();
	static bool m_loaded;
	static float m_window_width;
	static float m_window_height;
	static std::map<std::string, bool> m_flags;

	
};
