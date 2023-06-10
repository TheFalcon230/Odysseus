#pragma once
#include <Odysseus.h>

class Sandbox2D: public Odysseus::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Odysseus::Timestep updateTime) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Odysseus::Event& e) override;
private:
	Odysseus::OrthographicCameraController m_CameraController;

	//Temp variables
	Odysseus::ShaderLibrary m_ShaderLibrary;
	Odysseus::Ref<Odysseus::VertexArray> m_VertexArray;
	Odysseus::Ref<Odysseus::Shader> m_Shader;
	Odysseus::Ref<Odysseus::Texture2D> m_Texture;
	Odysseus::Ref<Odysseus::Texture2D> T_Spritesheet;
	Odysseus::Ref<Odysseus::Sprite> Sp_Bush_01;
	Odysseus::Ref<Odysseus::QuadProperties> testQuad;

	Odysseus::Timestep time;

	std::unordered_map<char, Odysseus::Ref<Odysseus::Sprite>> s_TextureMap;
	uint32_t mapWidth = 24, mapHeight = 12;






	struct ProfileResult
	{
		const char* Name;
		float Time;

		ProfileResult(const char* name, float time)
			:Time(time), Name(name)
		{

		}
	};

	struct ScrollingBuffer
	{
		int MaxSize;
		int Offset;
		std::vector<glm::vec2> Data;
		ScrollingBuffer(int max_size = 2000) {
			MaxSize = max_size;
			Offset = 0;
			Data.reserve(MaxSize);
		}
		void AddPoint(float time, float execTime) {
			if (Data.size() < MaxSize)
				Data.push_back({ time, execTime });
			else {
				Data[Offset] = { time, execTime };
				Offset = (Offset + 1) % MaxSize;
			}
		}
		void Erase() {
			if (Data.size() > 0) {
				Data.clear();
				Offset = 0;
			}
		}
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 1.f,1.f,1.f ,1.f };
};
