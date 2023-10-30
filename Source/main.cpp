#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

int WIDTH = 800;
int HEIGHT = 800;

bool is_drawing = false;
float m_x, m_y;

sf::Sprite sprite;
sf::Texture spriteTexture;
sf::Image spriteImage;

void DrawPixel(sf::RenderWindow &window, float x, float y);
void Render(sf::RenderWindow &window);
void WindowUpdate(sf::RenderWindow &window);

void HandleUserInput(sf::RenderWindow& window, const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;
	case sf::Event::MouseButtonPressed:
		is_drawing = true;
		
		break;
	case sf::Event::MouseButtonReleased:
		is_drawing = false;
		break;
	case sf::Event::KeyPressed:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			spriteImage.create(WIDTH, HEIGHT);
			WindowUpdate(window);
		}
		break;
	default:
		break;
	}
}

void UpdateMousePosition(sf::RenderWindow &window)
{
	m_x = sf::Mouse::getPosition(window).x;
	m_y = sf::Mouse::getPosition(window).y;
}

void Update(sf::RenderWindow& window, const sf::Time& deltaClock)
{
	if (is_drawing)
	{
		UpdateMousePosition(window);
		DrawPixel(window, m_x, m_y);
	}
}

void Render(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void DrawPixel(sf::RenderWindow &window, float x, float y)
{
	if (x <= 0 || x >= WIDTH || y <= 0 || y >= HEIGHT) return;
	spriteImage.setPixel(x, y, sf::Color(rand() % 255, rand() % 255, rand() % 255));

	WindowUpdate(window);
}

void WindowUpdate(sf::RenderWindow &window)
{
	spriteTexture.loadFromImage(spriteImage);
	sprite.setTexture(spriteTexture);
	sprite.setPosition(0, 0);

	window.draw(sprite);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Geometry modeling 1", sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(240);
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "ImGui initialization failed\n";
		return -1;
	}

	spriteImage.create(WIDTH, HEIGHT);

	sf::Clock deltaClock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(window, event);
			HandleUserInput(window, event);
		}

		sf::Time deltaTime = deltaClock.restart();
		ImGui::SFML::Update(window, deltaTime);
		Update(window, deltaTime);

		window.clear();

		Render(window);

		ImGui::SFML::Render(window);

		window.display();
	}
	ImGui::SFML::Shutdown();

	return 0;
}
