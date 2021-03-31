#ifndef _MATH_
#define _MATH_

#include <xmmintrin.h>


struct ipair {
	unsigned int x;
	unsigned int y;
};

struct m128 {

	m128(__m128 v) : f(v) {
	};

	m128(float v) : f(__m128{ v, v, v, v }) {
	};

	__m128 f = { 0.f, 0.f, 0.f, 0.f };

	inline m128 operator== (const m128& other) const {
		return m128(_mm_cmpeq_ps(this->f, other.f));
	}
	inline m128 operator!= (const m128& other) const {
		return m128(_mm_cmpneq_ps(this->f, other.f));
	}
	inline m128 Add(const m128& other) const {
		return m128(_mm_add_ps(this->f, other.f));
	}
	inline m128 operator+ (const m128& other) const {
		return Add(other);
	}
	inline m128& operator+= (const m128& other) {
		*this = *this + other;
		return *this;
	}
	inline m128 Substract(const m128& other) const {
		return m128(_mm_sub_ps(this->f, other.f));
	}
	inline m128 operator- (const m128& other) const {
		return Substract(other);
	}
	inline m128& operator-= (const m128& other) {
		*this = *this - other;
		return *this;
	}
	inline m128 Multiply(const m128& other) const {
		return m128(_mm_mul_ps(this->f, other.f));
	}
	inline m128 operator* (const m128& other) const {
		return Multiply(other);
	}
	inline m128& operator*= (const m128& other) {
		*this = *this * other;
		return *this;
	}
	inline m128 Multiply(const float& other) const {
		return m128(_mm_mul_ps(this->f, __m128{other, other, other, other}));
	}
	inline m128 operator* (const float& other) const {
		return Multiply(other);
	}
	inline m128& operator*= (const float& other) {
		*this = *this * other;
		return *this;
	}
	inline m128 Divide(const m128& other) const {
		return m128(_mm_div_ps(this->f, other.f));
	}
	inline m128 operator/ (const m128& other) const {
		return Divide(other);
	}
	inline m128& operator/= (const m128& other) {
		*this = *this / other;
		return *this;
	}
	inline m128 Divide(const float& other) const {
		return m128(_mm_div_ps(this->f, __m128{other, other, other, other}));
	}
	inline m128 operator/ (const float& other) const {
		return Divide(other);
	}
	inline m128& operator/= (const float& other) {
		*this = *this / other;
		return *this;
	}

	inline m128 operator&& (const m128& other) const {
		return (*this) * other;
	}
	inline m128 operator|| (const m128& other) const {
		return (*this) + other;
	}

	inline m128 Insert(const float f, const int pos) const {
		const __m128 x = _mm_set_ps(0 == pos ? f : 0.f, 1 == pos ? f : 0.f, 2 == pos ? f : 0.f, 3 == pos ? f : 0.f);
		return (*this) + m128(x);
	}

};

class Math {

public:

	inline static float finvsqrt(const float x) {
		long i;
		float x2, y;
		//thanks Walsh
		x2 = x * 0.5f;
		y = x;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (1.5f - (x2 * y * y));   // 1st iteration
		//y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration,
		return y;
	};

	inline static m128 finvsqrt_ps(const m128 v) {
		//reciprocal square root (invsqrt)
		return _mm_rsqrt_ps(v.f);
	};

	inline static int sign(int x) {
		return (x > 0) - (x < 0);
	}

};

#endif