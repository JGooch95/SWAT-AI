#include "../include/Settings.h"

Settings::Settings()
{
	m_bDebug = false;
}

bool Settings::m_bInstanceFlag = false;
Settings* Settings::settingsLocation = nullptr;

Settings* Settings::getInstance()
{
	if (!m_bInstanceFlag)
	{
		settingsLocation = new Settings();
		m_bInstanceFlag = true;
	}
	return settingsLocation;
}

bool Settings::debugActive()
{
	return m_bDebug;
}

void Settings::setDebug(bool bSetting)
{
	m_bDebug = bSetting;
}