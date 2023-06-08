#pragma once

#include "Framebuffer.h"
#include "renderer2d.h"

#include <string>
#include <vector>
#include <utility>
#include <memory>

enum class PostProcessingEffect
{
	SSAA = 0,
	MSAA,
	DEFFERED_LIGHTING,
	DEFAULT
};


class PostProcessing
{
public:
	static void Init();
	static void Shutdown();

	static void BeginScene(std::shared_ptr<Framebuffer> initial);
	static std::shared_ptr<Framebuffer> EndScene();

	static void AddStep(PostProcessingEffect effect);
private:
	static uint32_t get_unique();

};

