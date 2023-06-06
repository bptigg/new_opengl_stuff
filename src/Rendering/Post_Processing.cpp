#include "Post_Processing.h"

struct PostProcessingData
{
	std::shared_ptr<Framebuffer> m_ActiveFramebuffer;

	std::unordered_map<PostProcessingEffect, std::shared_ptr<Framebuffer>> m_Framebuffers;
	std::vector<PostProcessingEffect> m_pipeline;
};

PostProcessingData* s_data = nullptr;

void PostProcessing::Init()
{
	s_data = new PostProcessingData;
}

void PostProcessing::Shutdown()
{
	s_data->m_Framebuffers.clear();
	s_data->m_pipeline.clear();

	delete s_data;
}

void PostProcessing::BeginScene(std::shared_ptr<Framebuffer> initial)
{
	s_data->m_ActiveFramebuffer = initial;

	for (int i = 0; i < s_data->m_pipeline.size(); i++)
	{
		auto buffer = s_data->m_Framebuffers[s_data->m_pipeline[i]];
		auto attachments = buffer->GetSpec().Attachments.Attachments.size();
		for (size_t e = 0; e < attachments; e++)
		{
			buffer->clear_attachment(e, -1);
		}
	}
}

std::shared_ptr<Framebuffer> PostProcessing::EndScene()
{
	return s_data->m_ActiveFramebuffer;
}