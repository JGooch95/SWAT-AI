#include "../include/Settings.h"

bool Settings::m_bInstanceFlag = false;
Settings* Settings::m_SettingsLocation = nullptr;

Settings* Settings::getInstance()
{
	//If the object doesnt exist create the object
	if (!m_bInstanceFlag)
	{
		m_SettingsLocation = new Settings();
		m_bInstanceFlag = true;
	}

	//Return the object location
	return m_SettingsLocation;
}

void Settings::save(std::string sDir)
{
	//Saves the object map 
	std::ofstream File1;
	File1.open(sDir, std::ios::out);
	File1 << "res " << m_WindowResolution.x << " " << m_WindowResolution.y;
	File1 << "\n";
	File1 << "sfx " << m_fVolume;
	File1.close();
}

void Settings::load(std::string sDir)
{
	std::ifstream file;
	file.open(sDir); //Open the map file

	if (file.is_open()) //If the file opened correctly
	{
		//Initialise reading variables
		std::string sLine;
		std::string token;

		while (!file.eof()) //while the end of file hasnt been reached
		{
			getline(file, sLine); //Get the next line

			if (sLine != "")
			{
				std::istringstream sWord(sLine);
				sWord >> token;

				if (token == "res")
				{
					sf::Vector2f res;
					sWord >> token;
					res.x = stoi(token);

					sWord >> token;
					res.y = stoi(token);
					setResolution(res);
				}
				if (token == "sfx")
				{
					sWord >> token;
					setVolume(stof(token));
				}

			}
		}
	}
	else
	{
		//Ouptut an error
		std::cout << "Settings File: " << sDir << " could not be opened." << "\n";
	}

	file.close();
}

Settings::Settings()
{
	//Setting default values
	m_bDebug = false;
	m_fVolume = 100.0f;
	m_WindowResolution = sf::Vector2f(800.0f, 600.0f);
	bUpdateWindow = false;
}

//Setters
void Settings::setDebug(bool bSetting)
{
	m_bDebug = bSetting;
}

void Settings::setResolution(sf::Vector2f newRes)
{
	m_WindowResolution = newRes;
}

void Settings::setVolume(float fNewVolume)
{
	m_fVolume = fNewVolume;
}

//Getters
bool Settings::debugActive()
{
	return m_bDebug;
}

sf::Vector2f Settings::getResolution()
{
	return m_WindowResolution;
}

float Settings::getVolume()
{
	return m_fVolume;
}

void Settings::updateWindow(bool bState)
{
	bUpdateWindow = bState;
}

bool Settings::getUpdateState()
{
	return bUpdateWindow;
}
