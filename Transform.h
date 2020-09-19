#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "SDL.h"
#include <vector>
#include "Complex.h"

struct TransformArgs {

	Complex position = { 0, 0 };
	Complex velocity = { 0, 0 };
	Complex acceleration = { 0, 0 };
	Complex rotation = { 0, 0 };
	Complex rotationDelta = { 0, 0 };
	Complex visualRotation = { 0, 0 };

};
const TransformArgs defaultTransform;

class Transform
{

		public:

		Transform();
		Transform(const TransformArgs& args);
		~Transform();
		TransformArgs transform;

		inline void TranslateCartesian(Complex translation) {
			transform.position += translation;
		};
		inline void Translate(Complex translation, Complex rotationMat) {
			transform.position += rotationMat * translation;
		};
		inline void Accelerate(Complex acceleration) {
			transform.velocity += acceleration;
		};
		inline void UpdateKinematicCartesian() {
			TranslateCartesian(transform.velocity);
			Accelerate(transform.acceleration);
			UpdateAngle();
		};
		inline void UpdateKinematic() {
			Translate(transform.velocity, transform.rotation);
			Accelerate(transform.acceleration);
			Rotate(transform.rotationDelta);
			UpdateAngle();
		};
		inline void UpdateAngle() {
			transform.visualRotation = Complex::Normal(transform.rotation * transform.velocity);
		};
		inline void Rotate(Complex rotMat) {
			transform.rotation = Complex::Normal(transform.rotation * rotMat);
		};



		static inline void TranslateCartesian(std::vector<Complex>& position, const int ID, const Complex translation) {
			position[ID] += translation;
		};
		static inline void Translate(std::vector<Complex>& position, const int ID, const Complex translation, const Complex rotationMat) {
			position[ID] += translation * rotationMat;
		};
		static inline void Accelerate(std::vector<Complex>& velocity, const int ID, const Complex acceleration) {
			velocity[ID] += acceleration;
		};
		static inline void UpdateAngle(std::vector<Complex>& visualRotation, const int ID, const std::vector<Complex>& rotationMat, const std::vector<Complex>& velocity) {
			visualRotation[ID] = Complex::Normal(rotationMat[ID] * velocity[ID]);
		};
		static inline void Rotate(std::vector<Complex>& rotationMat, const int ID, const Complex rotMat) {
			rotationMat[ID] = Complex::Normal(rotationMat[ID] * rotMat);
		};



		static inline void BatchTranslateCartesian(std::vector<Complex>& position, const std::vector<Complex>& translation, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				position[i] += translation[i];
		};
		static inline void BatchTranslate(std::vector<Complex>& position, const std::vector<Complex>& translation, const std::vector<Complex>& rotationMat, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				position[i] += translation[i] * rotationMat[i];
		};
		static inline void BatchAccelerate(std::vector<Complex>& velocity, const std::vector<Complex>& acceleration, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				velocity[i] += acceleration[i];
		};
		static inline void BatchUpdateAngle(std::vector<Complex>& visualRotation, const std::vector<Complex>& rotationMat, const std::vector<Complex>& velocity, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				visualRotation[i] = Complex::Normal(rotationMat[i] * velocity[i]);
		};
		static inline void BatchRotate(std::vector<Complex>& rotationMat, const std::vector<Complex>& rotMat, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				rotationMat[i] = Complex::Normal(rotationMat[i] * rotMat[i]);
		};
		static inline void BatchRotate(std::vector<Complex>& rotationMat, const Complex rotMat, unsigned int currentSize) {
			for (unsigned int i = 0; i < currentSize; i++)
				rotationMat[i] = Complex::Normal(rotationMat[i] * rotMat);
		};

};

#endif