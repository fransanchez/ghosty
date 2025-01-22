#pragma once

namespace sf
{
	class RenderWindow;
}

class UIScreen
{
	public:
		virtual ~UIScreen() = default;

		virtual void init() = 0;
		virtual void deInit() = 0;
		virtual void update(float deltaMilliseconds) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
};