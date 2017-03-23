#pragma once

class Settings
{
private:
	static bool m_bInstanceFlag;
	bool m_bDebug;

public:
	Settings(); //Constructor

	static Settings* settingsLocation;
	static Settings* getInstance();

	bool debugActive();
	void setDebug(bool bSetting);
};