#include "Complex.h"

Complex::Complex()
	: x(0), y(0) {
}

Complex::Complex(const float x, const float y)
	: x(x), y(y) {
}

Complex::~Complex() {

}

const Complex Complex::up = Complex(0.0f, 1.0f);
const Complex Complex::down = Complex(0.0f, -1.0f);
const Complex Complex::left = Complex(-1.0f, 0.0f);
const Complex Complex::right = Complex(1.0f, 0.0f);
const Complex Complex::zero = Complex(0.0f, 0.0f);