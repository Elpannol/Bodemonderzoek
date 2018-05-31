// Bodemonderzoek.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Bodemonderzoek.h"

#include <VrLib\Log.h>
#include <VrLib\json.hpp>
#include <fstream>

#include <VrLib\tien\components\TransformAttach.h>
#include <VrLib\tien\components\ModelRenderer.h>
#include <VrLib/tien/components/TerrainRenderer.h>
#include <VrLib\tien\components\Camera.h>
#include "Manhole.h"
#include "VrLib/tien/components/TerrainCollider.h"


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
	logger << "Initialized" << logger.newline;

	nodes.push_back({ "data/Bodemonderzoek/models/Manhole/Manhole.fbx", "Manhole", glm::vec3(0,0,0) });
	manhole = new Manhole(&nodes[0]);

	//mainShader.mainShader();

	float scale = 1;
	terrain = new vrlib::tien::Terrain("data/Bodemonderzoek/textures/bikeTex/heightmap.png", 50.0f);

	vrlib::tien::Node* n = new vrlib::tien::Node("Terrain", &Engine.scene);
	n->addComponent(new vrlib::tien::components::Transform(glm::vec3(-terrain->getWidth() / 2.0f * scale, -15, -terrain->getHeight() / 2.0f* scale), glm::quat(), glm::vec3(scale, scale, scale)));
	auto terrainRenderer = new vrlib::tien::components::TerrainRenderer(terrain);
	terrainRenderer->addMaterialLayer("data/Bodemnderzoek/textures/bikeTex/grass_diffuse.png",
		"data/Bodemnderzoek/textures/bikeTex/grass_normal.png",
		"data/Bodemnderzoek/textures/bikeTex/grass_mask.png");
	terrainRenderer->addMaterialLayer("data/TienTest/biker/textures/ground_diffuse.png",
		"data/Bodemnderzoek/textures/bikeTex/ground_normal.png",
		"data/Bodemnderzoek/textures/bikeTex/ground_mask.png");
	n->addComponent(terrainRenderer);
	n->addComponent(new vrlib::tien::components::TerrainCollider(n));

	vive.init();

	leftHand = new HandController(vive.controllers[0]);
	rightHand = new HandController(vive.controllers[1]);

	Engine.init();

	loadScene();

	Engine.start();
}

void Bodemonderzoek::draw(const glm::mat4 & projectionMatrix, const glm::mat4 & modelViewMatrix)
{
	manhole->TestOpenClose();
	for (NodeLoader& nl : nodes) {
		nl.node->transform = new components::Transform(nl.position, nl.rotation, nl.scale);
	}

	Engine.render(projectionMatrix, modelViewMatrix);

	//leftHand->drawRay(modelViewMatrix, projectionMatrix);
	//rightHand->drawRay(modelViewMatrix, projectionMatrix);

	//mainShader.mainDraw(projectionMatrix, modelViewMatrix, 0, 0);
	rightHand->checkTeleport(vive.hmd.getData(), Engine);
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

	Node* leftHand = new Node("LeftHand", &Engine.scene);
	leftHand->addComponent(new components::TransformAttach(vive.controllers[0].transform));
	leftHand->addComponent(new components::Transform(glm::vec3(0, 0, 0)));
	leftHand->addComponent(this->leftHand);
	leftHand->addComponent(new components::ModelRenderer("data/vrlib/rendermodels/vr_controller_vive_1_5/vr_controller_vive_1_6.obj"));

	Node* rightHand = new Node("RightHand", &Engine.scene);
	rightHand->addComponent(new components::TransformAttach(vive.controllers[1].transform));
	rightHand->addComponent(new components::Transform(glm::vec3(0, 0, 0)));
	rightHand->addComponent(this->rightHand);
	rightHand->addComponent(new components::ModelRenderer("data/vrlib/rendermodels/vr_controller_vive_1_5/vr_controller_vive_1_6.obj"));

	this->rightHand->teleportTarget = new vrlib::tien::Node("HandRightIndicator", &Engine.scene);
	this->rightHand->teleportTarget->addComponent(new vrlib::tien::components::Transform(glm::vec3(), glm::quat(), glm::vec3(0.01f, 0.01f, 0.01f)));
	this->rightHand->teleportTarget->addComponent(new vrlib::tien::components::ModelRenderer("data/virtueelpd/models/idle.fbx"));
	this->rightHand->teleportTarget->getComponent<vrlib::tien::components::ModelRenderer>()->visible = false;

	Engine.scene.cameraNode = Engine.scene.findNodeWithName("Camera");

	for (NodeLoader& nl : nodes) {
		Node* node = new Node(nl.name, &Engine.scene);
		node->addComponent(new components::Transform(nl.position, nl.rotation, nl.scale));
		node->addComponent(new components::ModelRenderer(nl.path));
		nl.node = node;
	}
 }

NodeLoader* Bodemonderzoek::searchNode(std::string searchTerm)
{
	NodeLoader *searchNode = nullptr;
	for (NodeLoader nl : nodes) {
		if (nl.name._Equal(searchTerm)) {
			searchNode = &nl;
			break;
		}
	}
	return searchNode;
}
