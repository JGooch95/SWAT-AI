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

Settings::Settings()
{
	m_bDebug = false;
	fVolume = 100;
	gameResolution = sf::Vector2f(800, 600);
}

bool Settings::debugActive()
{
	return m_bDebug;
}

void Settings::setDebug(bool bSetting)
{
	m_bDebug = bSetting;
}

sf::Vector2f Settings::getResolution()
{
	return gameResolution;
}

float Settings::getVolume()
{
	return fVolume;
}

void Settings::setResolution(sf::Vector2f newRes)
{
	gameResolution = newRes;
}

void Settings::setVolume(float fNewVolume)
{
	fVolume = fNewVolume;
}