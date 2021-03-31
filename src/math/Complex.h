#ifndef _COMPLEX_
#define _COMPLEX_

#include "fasttrigo.h"
#include "Math.h"
#include <xmmintrin.h>

#ifndef RAD2DEG
#define RAD2DEG 57.295779513f
#endif
#ifndef DEG2RAD
#define DEG2RAD 0.01745329252f
#endif
#ifndef PI
#define PI 3.14159265358979323846
#endif

struct Complex {

	Complex(const float x = 0.f, const float y = 0.f) : x(x), y(y) {
	};

	//Complex(const float v = 0.f) : x(v), y(v) {
	//};

	~Complex() {};

	float x;
	float y;

	inline static Complex Conjugate(const Complex v) {
		return Complex(v.x, -v.y);
	};
	inline static float Abs(const Complex v) {
		return 1.f / Math::finvsqrt(v.x * v.x + v.y * v.y);
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
		const float invSqrt = Math::finvsqrt(v.x * v.x + v.y * v.y);
		return Complex(v.x * invSqrt, v.y * invSqrt);
	};
	inline static float Dot(const Complex u, const Complex v) {
		return (u.x * v.x + u.y * v.y);
	};
	inline static Complex Projection(const Complex& v1, const Complex& v2) { //V1 ON TO V2 PROJETION
		return v2 * (Complex::Dot(v2, v1) / Complex::SquaredAbs(v2));
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

};

struct Complex_ps {

	Complex_ps(const m128 x = m128(0.f), const m128 y = m128(0.f)) : x(x), y(y) {
	};

	/*
	Complex_ps(const m128 v = m128(0.f)) : x(v), y(v) {
	};
	*/

	~Complex_ps() {};

	m128 x;
	m128 y;


	inline static Complex_ps Conjugate(const Complex_ps v) {
		return Complex_ps(v.x, m128(_mm_rcp_ps(v.y.f)));
	};
	inline static m128 Abs(const Complex_ps v) {
		return m128(1.f) / Math::finvsqrt_ps(v.x * v.x + v.y * v.y);
	};
	inline static m128 SquaredAbs(const Complex_ps v) {
		return (v.x * v.x + v.y * v.y);
	};
	inline static m128 Angle(const Complex_ps v) {
		return m128((FT::atan2_ps(v.y.f, v.x.f))) * RAD2DEG;
	};
	inline static Complex_ps FromAngle(const m128 angle) {
		const Complex_ps rot = { FTA::cos_ps( (m128(angle) * DEG2RAD).f ), FTA::sin_ps( (m128(angle) * DEG2RAD).f ) };
		return Complex_ps::Normal(rot);
	};
	inline static Complex_ps Normal(const Complex_ps v) {
		const m128 invSqrt = Math::finvsqrt_ps(v.x * v.x + v.y * v.y);
		return Complex_ps(v.x * invSqrt, v.y * invSqrt);
	};
	inline static m128 Dot(const Complex_ps u, const Complex_ps v) {
		return (u.x * v.x + u.y * v.y);
	};
	inline static Complex_ps Projection(const Complex_ps& v1, const Complex_ps& v2) { //V1 ON TO V2 PROJETION
		return v2 * (Complex_ps::Dot(v2, v1) / Complex_ps::SquaredAbs(v2));
	};


	inline m128 operator== (const Complex_ps& other) const {
		return ((this->x == other.x) && (this->y == other.y));
	}
	inline m128 operator!= (const Complex_ps& other) const {
		return ((this->x != other.x) || (this->y != other.y));
	}
	inline Complex_ps Add(const Complex_ps& other) const {
		return Complex_ps(this->x + other.x, this->y + other.y);
	}
	inline Complex_ps operator+ (const Complex_ps& other) const {
		return Add(other);
	}
	inline Complex_ps& operator+= (const Complex_ps& other) {
		*this = *this + other;
		return *this;
	}
	inline Complex_ps Substract(const Complex_ps& other) const {
		return Complex_ps(this->x - other.x, this->y - other.y);
	}
	inline Complex_ps operator- (const Complex_ps& other) const {
		return Substract(other);
	}
	inline Complex_ps& operator-= (const Complex_ps& other) {
		*this = *this - other;
		return *this;
	}
	inline Complex_ps Multiply(const Complex_ps& other) const {
		return Complex_ps(this->x * other.x - this->y * other.y, this->x * other.y + this->y * other.x);
	}
	inline Complex_ps operator* (const Complex_ps& other) const {
		return Multiply(other);
	}
	inline Complex_ps& operator*= (const Complex_ps& other) {
		*this = *this * other;
		return *this;
	}
	inline Complex_ps Multiply(const m128& other) const {
		return Complex_ps(this->x * other, this->y * other);
	}
	inline Complex_ps operator* (const m128& other) const {
		return Multiply(other);
	}
	inline Complex_ps& operator*= (const m128& other) {
		*this = *this * other;
		return *this;
	}
	inline Complex_ps Divide(const Complex_ps& other) const {
		m128 divisor = other.x * other.x + other.y * other.y;
		return Complex_ps((this->x * other.x + this->y * other.y) / divisor, (this->y * other.x - this->x * other.y) / divisor);
	}
	inline Complex_ps operator/ (const Complex_ps& other) const {
		return Divide(other);
	}
	inline Complex_ps& operator/= (const Complex_ps& other) {
		*this = *this / other;
		return *this;
	}
	inline Complex_ps Divide(const m128& other) const {
		return Complex_ps(this->x / other, this->y / other);
	}
	inline Complex_ps operator/ (const m128& other) const {
		return Divide(other);
	}
	inline Complex_ps& operator/= (const m128& other) {
		*this = *this / other;
		return *this;
	}

	inline Complex_ps Insert(const Complex v, const int pos) {
		return { this->x.Insert(v.x, pos), this->y.Insert(v.y, pos) };
	}

	static const Complex_ps up;
	static const Complex_ps down;
	static const Complex_ps left;
	static const Complex_ps right;
	static const Complex_ps zero;

};

#endif