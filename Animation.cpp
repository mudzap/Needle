#include "Animation.h"

Animation::Animation(){
}

Animation::Animation(const Quad& drawQuad, const ipair animStates, const unsigned int frameTime)
	: drawQuad(drawQuad), frameTime(frameTime), dimensions(animStates),
	currentSpriteFrame(0), currentFrame(Timer::countedFrames),
	animationFrameOffset(0) {

	currentState = { (unsigned int)(dimensions.x / 2), (unsigned int)(dimensions.y / 2) };
	targetState = currentState;

	//animationSpritesID = new std::vector<unsigned int>[dimensions.x * dimensions.y];

	InitTransformBuffers();

}

Animation::~Animation() {
	//delete[] animationSpritesID;
}

/* //TODO, NOT NECESSARY
void Animation::SetStateSprites(unsigned int frames, unsigned int number, ipair states ...) {

	va_list args;
	va_start(args, states);

	for (unsigned int i = 0; i < number; i++) {

		ipair index = va_arg(args, ipair);
		animationSpritesID[index.x][index.y].resize(frames);

	}

	va_end(args);
}
*/

void Animation::SetStateSprites(const unsigned int frames, const ipair state) {

	animationSpritesID[state.x + dimensions.x * state.y].resize(frames);

}

void Animation::SetStateSprites(const unsigned int frames, const unsigned int states ...) {

	va_list args;
	va_start(args, states);

	for (unsigned int i = 0; i < states; i++) {

		unsigned int indexX = va_arg(args, unsigned int);
		unsigned int indexY = va_arg(args, unsigned int);
		//animationSpritesID[va_arg(args, unsigned int)][va_arg(args, unsigned int)].resize(frames);
		animationSpritesID[indexX + dimensions.x * indexY].resize(frames);

	}

	va_end(args);
}

void Animation::SetStateSprites(const unsigned int frames) {

	for (unsigned int j = 0; j < 1; j++) {

		for (unsigned int i = 0; i < 7; i++) {

			animationSpritesID[i + dimensions.x * j].resize(frames);

		}

	}

}

//MAPPING VECTOR IS READ FROM LEFT TO RIGHT, TOP TO BOTTOM
void Animation::CreateMapAndMesh(const float textureSize, const Quad spriteRegion) {

	unsigned int ID = 0;

	for (unsigned int j = 0; j < dimensions.y; j++) {

		for (unsigned int i = 0; i < dimensions.x; i++) {

			for (unsigned int n = 0; n < animationSpritesID[i + dimensions.x * j].size(); n++) {

				animationSpritesID[i + dimensions.x * j][n] = ID;
				ID++;

			}

		}

	}

	spriteMap.resize(ID);
	vertices.resize(ID);

	const float textConst = 1.f / textureSize;
	
	Quad quad = drawQuad;

	quad.y -= 0.5;
	quad.h -= 0.5;

	//GET INDICES FOR SPRITES IN SPRITEREGION
	const ipair spriteIndices = { (unsigned int)(spriteRegion.w / drawQuad.w), (unsigned int)(spriteRegion.h / drawQuad.h) };

	//FILL SPRITEMAP/VERTEX
	ID = 0;
	for (unsigned int i = 0; i < spriteIndices.y && ID < spriteMap.size(); i++) { //TODO: USE SPRITEMAP, NOT VERTEX
		for (unsigned int j = 0; j < spriteIndices.x && ID < spriteMap.size(); j++) {

			//FLIP Y AXIS 

			vertices[ID][0] = {
				{-drawQuad.w * 0.5f,									   -drawQuad.h * 0.5f},
				{textConst * (spriteRegion.x + drawQuad.w * j),				textConst * (textureSize - (quad.h + spriteRegion.y + drawQuad.h * i))}
			};
			vertices[ID][1] = {
				{drawQuad.w * 0.5f,										   -drawQuad.h * 0.5f},
				{textConst * (quad.w + spriteRegion.x + drawQuad.w * j),	textConst * (textureSize - (quad.h + spriteRegion.y + drawQuad.h * i))}
			};
			vertices[ID][2] = {
				{drawQuad.w * 0.5f,											drawQuad.h * 0.5f},
				{textConst * (quad.w + spriteRegion.x + drawQuad.w * j),	textConst * (textureSize - (spriteRegion.y + drawQuad.h * i))}
			};
			vertices[ID][3] = {
				{-drawQuad.w * 0.5f,										drawQuad.h * 0.5f},
				{textConst * (spriteRegion.x + drawQuad.w * j),				textConst * (textureSize - (spriteRegion.y + drawQuad.h * i))}
			};

			ID++;

		}
	}

	animationFrameOffset = animationSpritesID[currentState.x + dimensions.x * currentState.y][currentSpriteFrame];

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * spriteMap.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

}

void Animation::SetAnimationState(const ipair state) {

	targetState = state;
	currentState = state;
	currentSpriteFrame = 0;
	currentFrame = 0;

}

void Animation::SetAnimationState(const unsigned int stateX, const unsigned int stateY) {

	targetState = { stateX, stateY };
	currentState = { stateX, stateY };
	currentSpriteFrame = 0;
	currentFrame = 0;

}

void Animation::TransitionToAnimationState(const ipair state) {

	targetState = state;

}

void Animation::TransitionToAnimationState(const unsigned int stateX, const unsigned int stateY) {

	targetState = { stateX, stateY };

}

void Animation::HandleAnimation() {

	if (Timer::countedFrames - currentFrame >= frameTime) {

		currentSpriteFrame++;
		if (currentSpriteFrame >= animationSpritesID[currentState.x + dimensions.x * currentState.y].size())
			currentSpriteFrame = 0;

		if (currentState.x != targetState.x) {
			currentState.x += sign(targetState.x - currentState.x);
			currentSpriteFrame = 0;
		}

		if (currentState.y != targetState.y) {
			currentState.y += sign(targetState.y - currentState.y);
			currentSpriteFrame = 0;
		}

		animationFrameOffset = animationSpritesID[currentState.x + dimensions.x * currentState.y][currentSpriteFrame];
		currentFrame = Timer::countedFrames;

	}

}

//IBO AND VERTEX ALREADY DEFINED
void Animation::Draw(const TVertex* instanceData) {
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex), instanceData, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TVertex), instanceData);

	glDrawArrays(GL_TRIANGLE_FAN, animationFrameOffset*4, 4);

	glBindVertexArray(0);

}	