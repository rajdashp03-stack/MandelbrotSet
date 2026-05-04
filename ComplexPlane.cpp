#include "ComplexPlane.h"
#include <iostream>
#include <sstream>
#include <complex>
#include <cmath>
#include <vector>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {

	m_pixel_size = Vector2i(pixelWidth, pixelHeight);
	m_aspectRatio = pixelHeight / (float)pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {

	target.draw(m_vArray);
}

void ComplexPlane::zoomIn() {

	m_zoomCount++;
	float xSize = BASE_WIDTH * powf(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * powf(BASE_ZOOM, m_zoomCount);

	m_plane_size = Vector2f(xSize, ySize);
	m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {

	m_zoomCount--;
	float xSize = BASE_WIDTH * powf(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * powf(BASE_ZOOM, m_zoomCount);

	m_plane_size = Vector2f(xSize, ySize);
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {

	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = State::CALCULATING;
}

void ComplexPlane::loadText(Text& text) {

	std::stringstream strStream;
	strStream << "Mandelbrot Set\nCenter: (" << m_plane_center.x << "," << m_plane_center.y << ")\n";
	strStream << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\n";
	strStream << "Left-click to Zoom in\n";
	strStream << "Right-click to Zoom out";
	text.setString(strStream.str());
}

void ComplexPlane::updateRender() {

	if (m_state == State::CALCULATING) {

		for (int i = 0; i < m_pixel_size.y; ++i) {

			for (int j = 0; j < m_pixel_size.x; ++j) {

				int index = j + i * m_pixel_size.x;

				m_vArray[index].position = { (float)j, (float)i };
				Vector2i mapPixel = { j, i };
				int numIter = this->countIterations(this->mapPixelToCoords(mapPixel));
				Uint8 r, g, b;

				this->iterationsToRGB(numIter, r, g, b);
				
				m_vArray[index].color = { r, g, b };

			}
		}

		m_state = State::DISPLAYING;
	}
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {

	m_mouseLocation = this->mapPixelToCoords(mousePixel);
}

int ComplexPlane::countIterations(Vector2f coord) {

	std::complex<float> point(coord.x, coord.y);
	std::complex<float> z(point);


	int i = 0;

	while ((z.real() * z.real() + z.imag() * z.imag()) < 4.0 && i < MAX_ITER) {

		z = (z * z) + point;
		i++;
	}

	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
	
	//Take precentage of iterations then convert to an rgb value
	int val;
	Color color;
	int offset;

	if (count == MAX_ITER) {

		color = Color::Black;
		
	}
	else {

		float percent = (float)count / (float)MAX_ITER;
		float diff;
		Color colorA;
		Color colorB;
		
		if (percent >= 0.5) {

			colorA = Color::Red;
			colorB = Color::Magenta;
			diff = MAX_ITER - 0.5 * MAX_ITER;

		}
		else if (percent >= 0.3) {
			
			colorA = Color::Magenta;
			colorB = Color::Green;
			diff = 0.5 * MAX_ITER - 0.3 * MAX_ITER;

		}
		else {

			colorA = Color::Green;
			colorB = Color::Blue;
			diff = 0.3 * MAX_ITER;
		}

		
		color.r = (colorB.r - colorA.r) * ((float)count / diff) + colorA.r;
		color.b = (colorB.b - colorA.b) * ((float)count / diff) + colorA.b;
		color.g = (colorB.g - colorA.g) * ((float)count / diff) + colorA.g;

	}

	r = color.r;
	g = color.g;
	b = color.b;

}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {

	//Map mousePixel from screen to complex graph
	Vector2f newCoord;
	newCoord.x = ((mousePixel.x) / ((float)m_pixel_size.x - 0.0)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	newCoord.y = ((mousePixel.y - (float)m_pixel_size.y) / (0 - (float)m_pixel_size.y)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);

	return newCoord;
	//TODO: change imaginary plane range boundaries
}