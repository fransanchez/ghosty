#pragma once

#include <UI/UIScreen.h>

namespace sf
{
	class RectangleShape;
}


class UIScreenMainMenu : public UIScreen
{
	public:
		void init() override;
		void deInit() override;
		void update(float deltaMilliseconds) override;
		void render(sf::RenderWindow& window) override;

	private:
		sf::RectangleShape* m_logo { nullptr };
};