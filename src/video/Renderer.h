#ifndef _RENDERER_
#define _RENDERER_

#include "GL/glew.h"
#include <iostream>
#include <vector>
#include <deque>
#include <set>
#include <functional>

#include "Mesh.h"
#include "Shader.h"
#include "Model.h"

struct TransformableInfo {
	std::shared_ptr<TVertex> transforms;
	std::shared_ptr<Vertex> vertex2D;
};

struct CachedTransformableInfo {
	std::shared_ptr<TVertex> transforms;
	std::shared_ptr<Vertex> vertex2D;
	unsigned int quadHandle;
};

/*

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        //glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        //START FRAMEBUFFER

        intermediateFramebuffer.Bind();

        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, newPlayWidth, newPlayHeight);
        glScissor(0, 0, newPlayWidth, newPlayHeight);



        //DRAW STAGE

        shader3D.Bind();

        shader3D.SetUniformMat4f("u_VP", camera.vpMat);
        shader3D.SetUniform3f("lightDir", 0.f, 0.34202f, 0.93969f);
        //shader3D.SetUniform3f("lightPos", 60.f * FT::cos(SDL_GetTicks() / 1000.f), 150.f, 60.f * FT::sin(SDL_GetTicks() / 1000.f));
        shader3D.SetUniform3f("lightPos", camera.position.x, camera.position.y, camera.position.z);
        shader3D.SetUniform3f("viewPos", camera.position.x, camera.position.y, camera.position.z);


        stage.DrawTilesOcclude(shader3D);

        //DRAW SKYBOX

        shaderCube.Bind();
        skybox.Draw(shaderCube, camera);


        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);


        //DRAW ENTITIES

        shader.Bind();

        player.DrawBullets();
        player.Draw();

        for (Enemy* enemies : enemyPool.activeEnemies) {
            enemies->DrawBullets();
        }
        for (Enemy* enemies : enemyPool.activeEnemies) {
            enemies->Draw();
        }

        //MOVE TO SPAWNER
        //myLaser.ResetDraw();

        // FINISH FRAMEBUFFER
        fboIntermediateShader.Bind();
        frameBuffer.Bind();

        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        intermediateFramebuffer.BindTexture(4);
        intermediateFramebuffer.Draw();

        glViewport(0, 0, newPlayWidth/DOWNSCALE_FACTOR, newPlayHeight/DOWNSCALE_FACTOR);
        glScissor(0, 0, newPlayWidth/DOWNSCALE_FACTOR, newPlayHeight/DOWNSCALE_FACTOR);


        bool horizontal = true, first_iteration = true;
        int amount = 3; //KIND OF PERFORMANCE HEAVY
        gaussianShader.Bind();

        for (unsigned int i = 0; i < amount; i++)
        {
            gaussianFramebuffer[horizontal].Bind();
            gaussianFramebuffer[horizontal].BindTexture(5);
            gaussianShader.SetUniform1i("horizontal", horizontal);

            if (first_iteration) {
                frameBuffer.BindTexture(5, 2);
                frameBuffer.Draw();
            }
            else {
                gaussianFramebuffer[!horizontal].BindTexture(5);
                gaussianFramebuffer[!horizontal].Draw();
            }

            horizontal = !horizontal;

            if (first_iteration)
                first_iteration = false;
        }
        frameBuffer.Unbind(); //UNBINDS ALL FRAMEBUFFERS, SHOULD BE STATIC

        glViewport(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);
        glScissor(newXPlayOffset, newYPlayOffset, newPlayWidth, newPlayHeight);

        fboShader.Bind();
        frameBuffer.Draw();

        frameBuffer.BindTexture(4, 1);
        gaussianFramebuffer[!horizontal].BindTexture(5);

        //TEXT
        glViewport(newXOffset, newYOffset, newWidth, newHeight);
        glScissor(newXOffset, newYOffset, newWidth, newHeight);


        uiShader.Bind();
        gameUI.Draw();

        textShader.Bind();
        //MOVE TO UI
        myFont.StaticDraw();

        SDL_GL_SwapWindow(window);
        */


class Renderer {

public:
	
    int CacheQuad;

private:

	std::vector< std::shared_ptr<Mesh> > transformables;
    std::vector< std::shared_ptr<Mesh3D> > stageGeometry;
	
	/*
	ORDER		PRIM	VBO		IBO		SHADER
	3D: 		TRIS	3D		NONSEQ	3D		CH
	Effects:	FAN		FX		SEQ		TRANSF	CH
	Particles:	IFAN	BULLET  SEQ		TRANSF		!
J	Entities:	FAN		ENTITY	SEQ		TRANSF
J	Laser: 		STRIP	STRIP	SEQ		TRANSF
	Bullets: 	IFAN	BULLET	SEQ		TRANSF		!
J	Entities2:	FAN		ENTITY	SEQ		TRANSF
J	Text: 		FAN		TEXT2	SEQ		SCHANN	CH
	*/

};


#endif



