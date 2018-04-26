
#pragma once
#include "Uniforms.h"

#include <VrLib\Application.h>
#include <VrLib\tien\Tien.h>
#include <VrLib\HtcVive.h>
#include <VrLib\Device.h>
#include <glm\glm.hpp>

class Bodemonderzoek : public vrlib::Application{
	vrlib::Vive vive;
	vrlib::tien::Tien Engine;

public:
	Bodemonderzoek();
	~Bodemonderzoek();

	vrlib::Texture* gridTexture;

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;
	virtual void latePreFrame() override;

	void loadScene();
private:
	bool showDebug;
};