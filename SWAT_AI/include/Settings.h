#pragma once

class Settings
{
	private:
		static bool m_bInstanceFlag; //!< Holds whether the Settings have been created
		static Settings* m_SettingsLocation; //!< Holds the memory location of the object
		bool m_bDebug; //!< Holds whether the system is in debug mode or not

	public:
		/// \brief Default constructor
		Settings();

		/// \brief Gets a pointer to the singleton object and creates one if it doesnt exist.
		/// \return returns the pointer to the object
		static Settings* getInstance();
		
		/// \brief Sets the debug state
		/// \param bSetting The state of debug being set
		void setDebug(bool bSetting); 

		/// \brief Gets whether debug mode is active
		/// \return returns whether debug is active
		bool debugActive();
};