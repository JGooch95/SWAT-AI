#pragma once

class Settings
{
	private:
		static bool m_bInstanceFlag; //Holds whether the Settings have been created
		static Settings* m_SettingsLocation; //Holds the memory location of the object
		bool m_bDebug; //Holds whether the system is in debug mode or not

	public:
		Settings(); //Constructor
		static Settings* getInstance(); //Gets the pointer to the object
		
		void setDebug(bool bSetting); //Sets the debug state
		bool debugActive(); //Returns whether debug is active or not
};