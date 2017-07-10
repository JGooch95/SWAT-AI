#include "../include/ContentScreen.h"

ContentScreen::ContentScreen()
{
	m_CurrentSettings = Settings::getInstance();
	m_Textures = TextureLoader::getInstance();
	m_Fonts = FontManager::getInstance();
	m_SoundManager = SoundManager::getInstance();
	m_CurrentMap = Map::getInstance();
}

void ContentScreen::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
}

void ContentScreen::update(sf::Vector2i mousePos)
{
}

int ContentScreen::processInput(sf::Event keyCode, sf::Vector2i mousePos)
{
	return 0;
}
