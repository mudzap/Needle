#ifndef _CAMERA_
#define _CAMERA_

#include "glm/glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Transform.h"
#include "Complex.h"

enum CameraMovementState {
	CM_STOP,
	CM_LINEAR,
	CM_QUAD_BEZIER,
	CM_QUAD_IN,
	CM_QUAD_OUT,
	CM_CUBIC_BEZIER,
	CM_CUBIC_IN,
	CM_CUBIC_OUT,
	CM_CUBIC_BOTH
};

enum CameraRotationState {
	CR_STOP,
	CR_FIXED,
	CR_FOLLOW,
	CR_FOLLOW_YAW,
	CR_FOLLOW_PITCH,
	CR_LINEAR,
	CR_QUAD_BEZIER,
	CR_QUAD_IN,
	CR_QUAD_OUT,
	CR_CUBIC_BEZIER,
	CR_CUBIC_IN,
	CR_CUBIC_OUT,
	CR_CUBIC_BOTH
};

class Camera {

	public: 

		Camera()
			: position({ 0.f, 0.f, 0.f }), forward({ 0.f, 0.f, 1.f }), up({ 0.f, 1.f, 0.f }) {
			view = glm::lookAt(position, position + forward, up);
			HandleCamera();
		}

		Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
			: position(position), forward(forward), up(up) {
			view = glm::lookAt(position, position + forward, up);
			HandleCamera();
		};

		void inline HandleCamera() {
			UpdateCameraPosition();
			UpdateCameraVectors();
			view = glm::lookAt(position, position + forward, up);
			vpMat = projection * view;
			GetPlanes();
		};

		bool inline IsPointInsideView(glm::vec3 point) const { //DONE WITH SPHERES, SO NO NEED TO CHECK CUBES

			for (int i = 0; i < 6; i++) {
				if (planes[i][0] * point.x + planes[i][1] * point.y + planes[i][2] * point.z + planes[i][3] <= 0) {
					return false;
				}
			}

			return true;

		}

		bool inline IsSphereInsideView(glm::vec3 center, float radius) const {

			unsigned int count = 0;
			float distance;

			for (int i = 0; i < 6; i++) {

				distance = planes[i][0] * center.x + planes[i][1] * center.y + planes[i][2] * center.z + planes[i][3];

				if (distance <= -radius)
					return false;
				else
					count++;
			}

			return (count == 6);

		}

		void SetPosition(const float x, const float y, const float z);
		void LinearInterpTo(const unsigned int frames, const float x, const float y, const float z);
		void CubicInterpBoth(const unsigned int frames, const float x, const float y, const float z);
		void CubicInterpStart(const unsigned int frames, const float x, const float y, const float z);
		void CubicInterpEnd(const unsigned int frames, const float x, const float y, const float z);
		void QuadraticInterpStart(const unsigned int frames, const float x, const float y, const float z);
		void QuadraticInterpEnd(const unsigned int frames, const float x, const float y, const float z);
		void CameraStop();
		void QuadraticBezier(const unsigned int frames, const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
		void CubicBezier(const unsigned int frames, const float x1, const float y1, const float z1, const float x2, const float y2, const float z2, const float x3, const float y3, const float z3);

		void SetAngle(const float yaw, const float pitch);
		void CameraFixViewOn(const float x, const float y, const float z);
		void LinearInterpToAngle(const unsigned int frames, const float yaw, const float pitch);
		void CubicInterpBothAngle(const unsigned int frames, const float yaw, const float pitch);
		void CubicInterpStartAngle(const unsigned int frames, const float yaw, const float pitch);
		void CubicInterpEndAngle(const unsigned int frames, const float yaw, const float pitch);
		void QuadraticInterpStartAngle(const unsigned int frames, const float yaw, const float pitch);
		void QuadraticInterpEndAngle(const unsigned int frames, const float yaw, const float pitch);
		void CameraFollow(const float yaw, const float pitch);
		void CameraFollowYaw(const float pitch);
		void CameraFollowPitch(const float yaw);
		void CameraFloat();
		void QuadraticBezierAngle(const unsigned int frames, const float yaw1, const float pitch1, const float yaw2, const float pitch2);
		void CubicBezierAngle(const unsigned int frames, const float yaw1, const float pitch1, const float yaw2, const float pitch2, const float yaw3, const float pitch3);

		glm::vec3 position;
		glm::vec3 velocity;
		glm::vec2 rotationDelta;

		glm::mat4 view;
		glm::mat4 projection;
		glm::mat4 vpMat;

		glm::vec3 target;

		glm::vec2 angles;
		glm::vec2 offset;

	private:

		CameraMovementState movState = CM_STOP;
		CameraRotationState rotState = CR_STOP;

		float time;
		float timeStep;
		glm::vec3 differential;
		glm::vec3 differential2;
		glm::vec3 differential3;
		glm::vec3 tempPos;
		glm::vec3 lastPos;

		float planes[6][4];

		glm::vec3 forward;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 globalUp = { 0.f, 1.f, 0.f };

		float aTime;
		float aTimeStep;
		glm::vec2 anglesDelta;
		glm::vec2 anglesDelta2;
		glm::vec2 anglesDelta3;
		glm::vec2 tempAngles;

		void UpdateCameraVectors();
		void UpdateCameraPosition();
		void InterpPreparation(const unsigned int frames, const glm::vec3 target);
		void InterpPreparationAngle(const unsigned int frames, const glm::vec2 target);
		void SetForwardVectorFromRotation();
		void GetPlanes();

};

#endif