// Bodemonderzoek.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bodemonderzoek.h"

#include <VrLib\Log.h>
#include <VrLib\json.hpp>
#include <fstream>

#include <VrLib\tien\components\TransformAttach.h>
#include <VrLib\tien\components\ModelRenderer.h>
#include "VrLib/tien/components/MeshRenderer.h"
#include "vrlib/Texture.h"

using vrlib::logger;
using namespace vrlib::tien;

Bodemonderzoek::Bodemonderzoek()
{
}

Bodemonderzoek::~Bodemonderzoek()
{
}

void Bodemonderzoek::init()
{
	mainShader.mainShader();

	logger << "Initialized" << logger.newline;

	//vive.init();
	Engine.init();
	loadScene();
	Engine.start();
}

void Bodemonderzoek::draw(const glm::mat4 & projectionMatrix, const glm::mat4 & modelViewMatrix)
{
	Engine.render(projectionMatrix, modelViewMatrix);
	mainShader.mainDraw(projectionMatrix, modelViewMatrix);

}

void Bodemonderzoek::preFrame(double frameTime, double totalTime)
{
	Engine.update((float)frameTime / 1000.0f);
}

void Bodemonderzoek::latePreFrame()
{
}

void Bodemonderzoek::loadScene()
{
	Engine.scene.reset();

	std::ifstream str = std::ifstream("../Development/Applications/Bodemonderzoek/data/Scenes/TabletWorld.json");
	json scene = json::parse(str);
	Engine.scene.fromJson(scene["scene"], scene, [](auto) {return nullptr; });
}
