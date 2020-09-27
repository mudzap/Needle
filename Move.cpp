#include "Move.h"

Move::Move() {
}

Move::Move(EntityArgs *args) {
	this->entityArgs = *args;
}

Move::~Move() {
	//delete vertices;
}

void Move::TranslateCartesian(Move* entity, Complex translation)
{
	entity->entityArgs.position += translation;
}

void Move::Translate(Move* entity, Complex translation, float angle)
{
	entity->entityArgs.position += Complex::FromAngle(angle) * translation;
}

/*
void Move::AccelerateCartesian(Move* entity, Complex acceleration)
{
	entity->velocity += acceleration;
}
void Move::Accelerate(Move* entity, Complex acceleration, float angle)
{
	entity->velocity += Complex::RotationMatrix(angle) * acceleration;
}
*/

void Move::Accelerate(Move* entity, Complex acceleration)
{
	entity->entityArgs.velocity += acceleration;
}

void Move::UpdateKinematicCartesian(Move* entity)
{
	TranslateCartesian(entity, entity->entityArgs.velocity);
	Accelerate(entity, entity->entityArgs.acceleration);
	UpdateAngle(entity);
}

void Move::UpdateKinematic(Move* entity)
{
	Translate(entity, entity->entityArgs.velocity, entity->entityArgs.rotation);
	//AccelerateCartesian(entity, entity->acceleration);
	Accelerate(entity, entity->entityArgs.acceleration);
	Rotate(entity, entity->entityArgs.rotationDelta);
	UpdateAngle(entity);
}

void Move::UpdateAngle(Move* entity)
{
	entity->entityArgs.visualRotation = entity->entityArgs.rotation + Complex::Angle(entity->entityArgs.velocity);
}


void Move::Rotate(Move* entity, float angle)
{
	entity->entityArgs.rotation += angle;
}

void Move::Initialize() {
	float textConst = 1.f / 1024.f;
	Quad tempQuad = this->entityArgs.quad;


	vertices[0] = {
		-tempQuad.w * 0.5f,	 tempQuad.h * 0.5f,	textConst * tempQuad.x,					textConst * (1024.f - (tempQuad.y + tempQuad.h))
	};
	vertices[1] = {
		 tempQuad.w * 0.5f,  tempQuad.h * 0.5f,	textConst* (tempQuad.x + tempQuad.w),	textConst * (1024.f - (tempQuad.y + tempQuad.h))
	};
	vertices[2] = {
		 tempQuad.w * 0.5f, -tempQuad.h * 0.5f,	textConst* (tempQuad.x + tempQuad.w),	textConst * (1024.f - tempQuad.y)
	};
	vertices[3] = {
		-tempQuad.w * 0.5f, -tempQuad.h * 0.5f,	textConst* tempQuad.x,					textConst * (1024.f - tempQuad.y)
	};

	unsigned int indices[4] = {
		0, 1, 2, 3
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,uv));

	if (instanced)
		glGenBuffers(1, &instanceVbo);

	glBindVertexArray(0);
	
}

void Move::Draw(unsigned int instances) {
	glBindVertexArray(vao);

	if (instanced)
	{
		glDrawElementsInstanced(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0, instances);
	}
	else
	{
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, 0);
	}
}
