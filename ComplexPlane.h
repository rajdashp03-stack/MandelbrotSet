#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State {CALCULATING, DISPLAYING};

class ComplexPlane : public sf::Drawable {

public:

	ComplexPlane(int pixelWidth, int pixelHeight);

	//allows Renderwindow class to call draw on ComplexPlane
	void draw(RenderTarget& target, RenderStates states) const;

	//Zooms in onto Mandelbrot Set
	void zoomIn();

	//Zooms out of Mandelbrot set
	void zoomOut();

	//Sets plane center to mousePixel screen location
	void setCenter(Vector2i mousePixel);

	//loads instructions and position of center and cursor
	void loadText(Text& text);

	//updates Mandelbrot Set based on current center and complex plane bounds
	void updateRender();

	//sets mouse location
	void setMouseLocation(Vector2i mousePixel);


private:

	VertexArray m_vArray;
	enum State m_state;
	Vector2f m_mouseLocation;
	Vector2i m_pixel_size;
	Vector2f m_plane_center;
	Vector2f m_plane_size;
	int m_zoomCount;
	float m_aspectRatio; 

	//Counts iterations using Mandelbrot set formula for coord
	int countIterations(Vector2f coord);

	//converts iterations to and RGB value
	void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);

	//converts screen pixel to coordinate on complex plane
	Vector2f mapPixelToCoords(Vector2i mousePixel);
};