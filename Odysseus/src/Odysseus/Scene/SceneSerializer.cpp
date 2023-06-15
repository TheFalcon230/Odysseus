#include "odcpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	/*template<>
	struct convert<Odysseus::UUID>
	{
		static Node encode(const Odysseus::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, Odysseus::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};*/

}


namespace Odysseus
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeObject(YAML::Emitter& out, Object object)
	{
		out << YAML::BeginMap; // Object
		out << YAML::Key << "Object";
		out << YAML::Value << "184563512379";

		if (object.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // Tag Component

			auto& tag = object.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Name" << YAML::Value << tag;

			out << YAML::EndMap; // Tag Component
		}

		if (object.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // Transform Component

			auto& pos = object.GetComponent<TransformComponent>().Position;
			auto& rot = object.GetComponent<TransformComponent>().Rotation;
			auto& scale = object.GetComponent<TransformComponent>().Scale;
			out << YAML::Key << "Position" << YAML::Value << pos;
			out << YAML::Key << "Rotation" << YAML::Value << rot;
			out << YAML::Key << "Scale" << YAML::Value << scale;

			out << YAML::EndMap; // Transform Component
		}

		if (object.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // Camera Component

			auto& cameraComponent = object.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera 
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << (int)camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera 

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.bIsMainCamera;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.bHasFixedAspectRatio;

			out << YAML::EndMap; // Camera Component
		}

		if (object.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = object.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			/*if (spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();

			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;*/

			out << YAML::EndMap; // SpriteRendererComponent
		}

		out << YAML::EndMap; // Object
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "New Scene";
		out << YAML::Key << "Objects";
		out << YAML::Value << YAML::BeginSeq;
		m_Scene->registry.each([&](auto& objectID)
		{
			Object object = {objectID, m_Scene.get() };
		if (!object)
			return;

		SerializeObject(out, object);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		ODC_CORE_ASSERT(false, "Not Implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			ODC_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		ODC_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Objects"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Object"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Name"].as<std::string>();

				ODC_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Object deserializedEntity = m_Scene->CreateObject(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Position = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::EProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNear(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFar(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNear(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFar(cameraProps["OrthographicFar"].as<float>());

					cc.bIsMainCamera = cameraComponent["Primary"].as<bool>();
					cc.bHasFixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}
								
				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					/*if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.Texture = Texture2D::Create(path.string());
					}

					if (spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();*/
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		ODC_CORE_ASSERT(false, "Not Implemented");
		return false;
	}

}