#include "Camera.h"

void Camera::UpdateCameraVectors() {

	aTime += aTimeStep;

	switch (rotState) {

		case CR_FIXED: {
			forward = glm::normalize(target - position);
			angles.x = glm::degrees(glm::acos(glm::dot(glm::vec3{ 1, 0, 0 }, glm::vec3{ forward.x, 0, forward.z })));
			angles.y = 90 - glm::degrees(glm::acos(glm::dot(forward, globalUp)));
			break;
		}

		case CR_FOLLOW: {

			if (position != lastPos) {
				forward = glm::normalize(position - lastPos);
				angles.x = glm::degrees(glm::acos(glm::dot(glm::vec3{ 1, 0, 0 }, glm::vec3{ forward.x, 0, forward.z }))) + offset.x;
				angles.y = 90 - glm::degrees(glm::acos(glm::dot(forward, globalUp))) + offset.y;

				SetForwardVectorFromRotation();

			}

			break;
		}

		case CR_FOLLOW_YAW: {
			glm::vec3 tempVector = { forward.x, 0, forward.z };

			if (position != lastPos) {
				tempVector = glm::normalize(position - lastPos);

				angles.x = glm::degrees(glm::acos(glm::dot(glm::vec3{ 1, 0, 0 }, glm::vec3{ tempVector.x, 0, tempVector.z })));
				angles.y = offset.y;

				if (angles.y > 89.9f)
					angles.y = 89.9f;
				if (angles.y < -89.9f)
					angles.y = -89.9f;

				tempVector.y = sin(glm::radians(angles.y));
				forward = glm::normalize(tempVector);

			}

			break;
		}

		case CR_FOLLOW_PITCH: {

			if (position != lastPos) {
				forward = glm::normalize(position - lastPos);
				
				angles.x = offset.x;
				angles.y = 90 - glm::degrees(glm::acos(glm::dot(forward, globalUp)));

				SetForwardVectorFromRotation();

			}

			break;
		}

		case CR_LINEAR: {
			angles = tempAngles + anglesDelta * aTime;
			SetForwardVectorFromRotation();
			break;
		}

		case CR_QUAD_BEZIER: {
			angles = (1 - aTime) * (1 - aTime) * tempAngles + 2 * aTime * (1 - aTime) * anglesDelta + aTime * aTime * anglesDelta2;
			SetForwardVectorFromRotation();
			break;
		}

		case CR_QUAD_IN: {
			angles = tempAngles + anglesDelta * aTime * aTime;
			SetForwardVectorFromRotation();
			break;
		}

		case CR_QUAD_OUT: {
			angles = tempAngles + anglesDelta * ((aTime - 1) * (aTime - 1) + 1);
			SetForwardVectorFromRotation();
			break;
		}

		case CR_CUBIC_BEZIER: {
			float aTime2 = aTime * aTime;
			float OneMinT2 = (1 - aTime) * (1 - aTime);
			angles =
				(1 - aTime) * OneMinT2 * tempAngles +
				3 * aTime * OneMinT2 * anglesDelta +
				3 * aTime2 * (1 - aTime) * anglesDelta2 +
				aTime * aTime2 * anglesDelta3;
			SetForwardVectorFromRotation();
			break;
		}

		case CR_CUBIC_IN: {
			angles = tempAngles + anglesDelta * aTime * aTime * aTime;
			SetForwardVectorFromRotation();
			break;
		}

		case CR_CUBIC_OUT: {
			angles = tempAngles + anglesDelta * ((aTime - 1.f) * (aTime - 1.f) * (aTime - 1.f) + 1);
			SetForwardVectorFromRotation();
			break;
		}

		case CR_CUBIC_BOTH: {
			float aTime2 = aTime * aTime;
			angles = tempAngles + anglesDelta * (-2.f * aTime2 * aTime + 3.f * aTime2);
			SetForwardVectorFromRotation();
			break;
		}

		case CR_STOP: {
			angles.x += rotationDelta.x;
			angles.y += rotationDelta.y;
			SetForwardVectorFromRotation();
			break;
		}

	}

	if (aTime >= 1.f - aTimeStep) {
		rotState = CR_STOP;
	}

	right = glm::normalize(glm::cross(forward, globalUp));
	up = glm::normalize(glm::cross(right, forward));
};


void Camera::SetForwardVectorFromRotation() {

	if (angles.y > 89.9f)
		angles.y = 89.9f;
	if (angles.y < -89.9f)
		angles.y = -89.9f;

	glm::vec3 front;
	front.x = cos(glm::radians(angles.x)) * cos(glm::radians(angles.y));
	front.y = sin(glm::radians(angles.y));
	front.z = sin(glm::radians(angles.x)) * cos(glm::radians(angles.y));
	forward = glm::normalize(front);

}

void Camera::UpdateCameraPosition() {

	lastPos = position;

	time += timeStep;

	switch (movState) {

		case CM_LINEAR: {
			position = tempPos + differential * time;
			break;
		}

		case CM_QUAD_BEZIER: {
			position = (1 - time) * (1 - time) * tempPos + 2 * time * (1 - time) * differential + time * time * differential2;
			break;
		}

		case CM_QUAD_IN: {
			position = tempPos + differential * time * time;
			break;
		}

		case CM_QUAD_OUT: {
			position = tempPos + differential * ((time - 1) * (time - 1) + 1);
			break;
		}

		case CM_CUBIC_BEZIER: {
			float time2 = time * time;
			float OneMinT2 = (1 - time) * (1 - time);
			position =
				(1 - time) * OneMinT2 * tempPos +
				3 * time * OneMinT2 * differential +
				3 * time2 * (1 - time) * differential2 +
				time * time2 * differential3;
			break;
		}

		case CM_CUBIC_IN: {
			position = tempPos + differential * time * time * time;
			break;
		}

		case CM_CUBIC_OUT: {
			position = tempPos + differential * ((time - 1.f) * (time - 1.f) * (time - 1.f) + 1);
			break;
		}

		case CM_CUBIC_BOTH: {
			float time2 = time * time;
			position = tempPos + differential * (-2.f * time2 * time + 3.f * time2);
			break;
		}

		case CM_STOP: {
			position += right * velocity.x;
			position += globalUp * velocity.y;
			position += forward * velocity.z;
			break;
		}

	}

	if (time >= 1.f - timeStep) {
		movState = CM_STOP;
	}

}

void Camera::SetPosition(const float x, const float y, const float z) {
	InterpPreparation(1, { 0, 0, 0 });
	movState = CM_STOP;
	position = glm::vec3{ x, y, z };
}

void Camera::LinearInterpTo(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_LINEAR;
}

void Camera::CubicInterpBoth(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_CUBIC_BOTH;
}

void Camera::CubicInterpStart(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_CUBIC_IN;
}

void Camera::CubicInterpEnd(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_CUBIC_OUT;
}

void Camera::QuadraticInterpStart(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_QUAD_IN;
}

void Camera::QuadraticInterpEnd(const unsigned int frames, const float x, const float y, const float z) {
	InterpPreparation(frames, { x, y, z });
	movState = CM_QUAD_OUT;
}

void Camera::CameraStop() {
	movState = CM_STOP;
}

void Camera::SetAngle(const float yaw, const float pitch) {
	InterpPreparationAngle(1, { 0, 0 });
	rotState = CR_STOP;
	angles = glm::vec2{ yaw, pitch };
}

void Camera::LinearInterpToAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_LINEAR;
}

void Camera::CubicInterpBothAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_CUBIC_BOTH;
}

void Camera::CubicInterpStartAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_CUBIC_IN;
}

void Camera::CubicInterpEndAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_CUBIC_OUT;
}

void Camera::QuadraticInterpStartAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_QUAD_IN;
}

void Camera::QuadraticInterpEndAngle(const unsigned int frames, const float yaw, const float pitch) {
	InterpPreparationAngle(frames, { yaw, pitch });
	rotState = CR_QUAD_OUT;
}

void Camera::CameraFollow(const float yaw, const float pitch) {
	rotState = CR_FOLLOW;
	offset = { yaw, pitch };
}

void Camera::CameraFollowYaw(const float pitch) {
	rotState = CR_FOLLOW_YAW;
	offset = { 0, pitch };
}

void Camera::CameraFollowPitch(const float yaw) {
	rotState = CR_FOLLOW_PITCH;
	offset = { yaw, 0 };
}

void Camera::CameraFloat() {
	rotState = CR_STOP;
}

void Camera::QuadraticBezierAngle(
	const unsigned int frames,
	const float yaw1, const float pitch1,
	const float yaw2, const float pitch2) {
	rotState = CR_QUAD_BEZIER;
	InterpPreparationAngle(frames, { yaw1, pitch1 });
	anglesDelta = angles + glm::vec2{ yaw1, pitch1 };
	anglesDelta2 = anglesDelta + glm::vec2{ yaw2, pitch2 };
}

void Camera::CubicBezierAngle(
	const unsigned int frames,
	const float yaw1, const float pitch1,
	const float yaw2, const float pitch2,
	const float yaw3, const float pitch3) {
	rotState = CR_CUBIC_BEZIER;
	InterpPreparationAngle(frames, { yaw1, pitch1 });
	anglesDelta = angles + glm::vec2{ yaw1, pitch1 };
	anglesDelta2 = anglesDelta + glm::vec2{ yaw2, pitch2 };
	anglesDelta3 = anglesDelta2 + glm::vec2{ yaw3, pitch3 };
}

void Camera::InterpPreparationAngle(const unsigned int frames, const glm::vec2 target) {
	tempAngles = angles;
	aTime = 0.f;
	aTimeStep = 1.f / frames;
	anglesDelta = target;
}

void Camera::QuadraticBezier(
	const unsigned int frames,
	const float x1, const float y1, const float z1,
	const float x2, const float y2, const float z2) {
	movState = CM_QUAD_BEZIER;
	InterpPreparation(frames, { x1, y1, z1 });
	differential = position + glm::vec3{ x1, y1, z1 };
	differential2 = differential + glm::vec3{ x2, y2, z2 };
}

void Camera::CubicBezier(
	const unsigned int frames,
	const float x1, const float y1, const float z1,
	const float x2, const float y2, const float z2,
	const float x3, const float y3, const float z3) {
	movState = CM_CUBIC_BEZIER;
	InterpPreparation(frames, { x1, y1, z1 });
	differential = position + glm::vec3{ x1, y1, z1 };
	differential2 = differential + glm::vec3{ x2, y2, z2 };
	differential3 = differential2 + glm::vec3{ x3, y3, z3 };
}

void Camera::InterpPreparation(const unsigned int frames, const glm::vec3 target) {

	time = 0.f;
	timeStep = 1.f / frames;
	differential = target;
	tempPos = position;

}

void Camera::CameraFixViewOn(const float x, const float y, const float z) {
	if (x * x + y * y + z * z > 0.001f)	//USE AN EPSILON
		target = position + glm::vec3{ x, y, z };
	else
		target = glm::vec3{ 0.f, 0.f, 0.001f };

	rotState = CR_FIXED;
}

void Camera::GetPlanes() {

	//Thanks dude:
	//http://www.crownandcutlass.com/features/technicaldetails/frustum.html
	//http://web.archive.org/web/20120531231005/http://crazyjoke.free.fr/doc/3D/plane%20extraction.pdf

	//LEFT
	planes[0][0] = vpMat[0][3] + vpMat[0][0];
	planes[0][1] = vpMat[1][3] + vpMat[1][0];
	planes[0][2] = vpMat[2][3] + vpMat[2][0];
	planes[0][3] = vpMat[3][3] + vpMat[3][0];

	//RIGHT
	planes[1][0] = vpMat[0][3] - vpMat[0][0];
	planes[1][1] = vpMat[1][3] - vpMat[1][0];
	planes[1][2] = vpMat[2][3] - vpMat[2][0];
	planes[1][3] = vpMat[3][3] - vpMat[3][0];

	//TOP
	planes[2][0] = vpMat[0][3] - vpMat[0][1];
	planes[2][1] = vpMat[1][3] - vpMat[1][1];
	planes[2][2] = vpMat[2][3] - vpMat[2][1];
	planes[2][3] = vpMat[3][3] - vpMat[3][1];

	//BOTTOM
	planes[3][0] = vpMat[0][3] + vpMat[0][1];
	planes[3][1] = vpMat[1][3] + vpMat[1][1];
	planes[3][2] = vpMat[2][3] + vpMat[2][1];
	planes[3][3] = vpMat[3][3] + vpMat[3][1];

	//NEAR (NEEDED?)
	planes[4][0] = vpMat[0][3] - vpMat[0][2];
	planes[4][1] = vpMat[1][3] - vpMat[1][2];
	planes[4][2] = vpMat[2][3] - vpMat[2][2];
	planes[4][3] = vpMat[3][3] - vpMat[3][2];

	//FAR (NEEDED?)
	planes[5][0] = vpMat[0][2];
	planes[5][1] = vpMat[1][2];
	planes[5][2] = vpMat[2][2];
	planes[5][3] = vpMat[3][2];

	float norm;

	norm = Complex::finvsqrt(planes[5][0] * planes[5][0] + planes[5][1] * planes[5][1] + planes[5][2] * planes[5][2]);
	planes[5][0] *= norm;
	planes[5][1] *= norm;
	planes[5][2] *= norm;
	planes[5][3] *= norm;

	norm = Complex::finvsqrt(planes[4][0] * planes[4][0] + planes[4][1] * planes[4][1] + planes[4][2] * planes[4][2]);
	planes[4][0] *= norm;
	planes[4][1] *= norm;
	planes[4][2] *= norm;
	planes[4][3] *= norm;

}