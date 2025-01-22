#pragma once

namespace sf
{
	class RenderWindow;
}

class UIScreen;

class UIManager
{
	public:

		void show(UIScreen& screen); // To-Do pass a transition
		void update(float deltaMilliseconds);
		void render(sf::RenderWindow& window);

	private:

		UIScreen* m_currentUIScreen{ nullptr };
};