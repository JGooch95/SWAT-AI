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
}

bool Settings::debugActive()
{
	return m_bDebug;
}

void Settings::setDebug(bool bSetting)
{
	m_bDebug = bSetting;
}