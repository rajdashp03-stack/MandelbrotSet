#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main()

{

	int pixelWidth = VideoMode::getDesktopMode().width;
	int pixelHeight = VideoMode::getDesktopMode().height;

	VideoMode vm(pixelWidth, pixelHeight);
	RenderWindow window(vm, "Mandelbrot Set", Style::Default);

	ComplexPlane plane(pixelWidth, pixelHeight);

	Font font;
	font.loadFromFile("Roboto-Regular.ttf");
	Text displayText;
	displayText.setFont(font);
	displayText.setFillColor(Color::White);
	displayText.setCharacterSize(30);

	bool isFirst = true;

	chrono::time_point<chrono::steady_clock> start = std::chrono::steady_clock::now();
	std::chrono::duration<long long, std::milli> time;
	chrono::time_point<chrono::steady_clock> end;

	while (window.isOpen())
	{
		///Input
		Event event;
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed) {

				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {

				if (event.mouseButton.button == Mouse::Right) {

					plane.zoomOut();
					plane.setCenter(Mouse::getPosition());
				}

				if (event.mouseButton.button == Mouse::Left) {

					plane.zoomIn();
					plane.setCenter(Mouse::getPosition());
				}
			}

			if (event.type == Event::MouseMoved) {

				plane.setMouseLocation(Mouse::getPosition());
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		///Update

		//checks time passed after start
		end = std::chrono::steady_clock::now();
		time = std::chrono::duration_cast<chrono::milliseconds>(end - start);
		if (time.count() >= 500 || isFirst) {

			//updates render of Mandelbrot set and sets isFirst to false
			plane.updateRender();
			isFirst = false;
			start = std::chrono::steady_clock::now();
		}
		plane.loadText(displayText);

		///Draw

		window.clear();
		window.draw(plane);
		window.draw(displayText);
		window.display();

	}
}