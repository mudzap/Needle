#ifndef _COMPLEX_
#define _COMPLEX_

#include "fasttrigo.h"

#ifndef RAD2DEG
#define RAD2DEG 57.295779513f
#endif
#ifndef DEG2RAD
#define DEG2RAD 0.01745329252f
#endif
#ifndef PI
#define PI 3.14159265358979323846
#endif

class Complex {
	
	public:
		Complex();
		Complex(const float x, const float y);
		~Complex();

		float x;
		float y;

		inline static Complex Conjugate(const Complex v) {
			return Complex(v.x, -v.y);
		};
		inline static float Abs(const Complex v) {
			return 1.f / Complex::finvsqrt(v.x * v.x + v.y * v.y);
		};
		inline static float SquaredAbs(const Complex v) {
			return (v.x * v.x + v.y * v.y);
		};
		inline static float Angle(const Complex v) {
			return (FT::atan2(v.y, v.x)) * RAD2DEG;
		};
		inline static Complex FromAngle(const float angle) {
			const Complex rot = { FTA::cos(angle * DEG2RAD), FTA::sin(angle * DEG2RAD) };
			return Complex::Normal(rot);
		};
		inline static Complex Normal(const Complex v) {
			const float invSqrt = finvsqrt(v.x * v.x + v.y * v.y);
			return Complex(v.x * invSqrt, v.y * invSqrt);
		};
		inline static float Dot(const Complex u, const Complex v) {
			return (u.x * v.x + u.y * v.y);
		};
		inline static Complex Projection(const Complex& v1, const Complex& v2) { //V1 ON TO V2 PROJETION
			return v2 * (Complex::Dot(v2, v1) / Complex::SquaredAbs(v2));
		}
		inline static float finvsqrt(const float x) {
			long i;
			float x2, y;
			const float threehalfs = 1.5f;
			//thanks Walsh
			x2 = x * 0.5f;
			y = x;
			i = *(long*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;
			y = y * (threehalfs - (x2 * y * y));   // 1st iteration
			//y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration,
			return y;
		};



		inline bool operator== (const Complex& other) const {
			return ((this->x == other.x) && (this->y == other.y));
		}
		inline bool operator!= (const Complex& other) const {
			return (!((*this) == other));
		}
		inline Complex Add(const Complex& other) const {
			return Complex(this->x + other.x, this->y + other.y);
		}
		inline Complex operator+ (const Complex& other) const {
			return Add(other);
		}
		inline Complex& operator+= (const Complex& other) {
			*this = *this + other;
			return *this;
		}
		inline Complex Substract(const Complex& other) const {
			return Complex(this->x - other.x, this->y - other.y);
		}
		inline Complex operator- (const Complex& other) const {
			return Substract(other);
		}
		inline Complex& operator-= (const Complex& other) {
			*this = *this - other;
			return *this;
		}
		inline Complex Multiply(const Complex& other) const {
			return Complex(this->x * other.x - this->y * other.y, this->x * other.y + this->y * other.x);
		}
		inline Complex operator* (const Complex& other) const {
			return Multiply(other);
		}
		inline Complex& operator*= (const Complex& other) {
			*this = *this * other;
			return *this;
		}
		inline Complex Multiply(const float& other) const {
			return Complex(this->x * other, this->y * other);
		}
		inline Complex operator* (const float& other) const {
			return Multiply(other);
		}
		inline Complex& operator*= (const float& other) {
			*this = *this * other;
			return *this;
		}
		inline Complex Divide(const Complex& other) const {
			float divisor = other.x * other.x + other.y * other.y;
			return Complex((this->x * other.x + this->y * other.y) / divisor, (this->y * other.x - this->x * other.y) / divisor);
		}
		inline Complex operator/ (const Complex& other) const {
			return Divide(other);
		}
		inline Complex& operator/= (const Complex& other) {
			*this = *this / other;
			return *this;
		}
		inline Complex Divide(const float& other) const {
			return Complex(this->x / other, this->y / other);
		}
		inline Complex operator/ (const float& other) const {
			return Divide(other);
		}
		inline Complex& operator/= (const float& other) {
			*this = *this / other;
			return *this;
		}


		static const Complex up;
		static const Complex down;
		static const Complex left;
		static const Complex right;
		static const Complex zero;

	private:

};

#endif