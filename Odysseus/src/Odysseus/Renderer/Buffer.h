#pragma once

namespace Odysseus
{
	enum class ShaderDataType : uint8_t
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Odysseus::ShaderDataType::Float:   return 4;
			break;
		case Odysseus::ShaderDataType::Float2:  return 4 * 2;
			break;
		case Odysseus::ShaderDataType::Float3:	return 4 * 3;
			break;
		case Odysseus::ShaderDataType::Float4:  return 4 * 4;
			break;
		case Odysseus::ShaderDataType::Mat3:	return 4 * 3 * 3;
			break;
		case Odysseus::ShaderDataType::Mat4:	return 4 * 4 * 4;
			break;
		case Odysseus::ShaderDataType::Int:		return 4;
			break;
		case Odysseus::ShaderDataType::Int2:	return 4 * 2;
			break;
		case Odysseus::ShaderDataType::Int3:	return 4 * 3;
			break;
		case Odysseus::ShaderDataType::Int4:	return 4 * 4;
			break;
		case Odysseus::ShaderDataType::Bool:	return 1;
			break;
		}

		ODC_CORE_ASSERT(false, "Unkown ShaderDataType.");
		return 0;
	}

	struct BufferElements
	{
		ShaderDataType Type = Odysseus::ShaderDataType::None;
		std::string Name = "";
		uint32_t Offset = 0;
		uint32_t Size = ShaderDataTypeSize(Odysseus::ShaderDataType::None);
		bool Normalized = false;

		BufferElements() : Name(""), Type(Odysseus::ShaderDataType::None), Size(ShaderDataTypeSize(Odysseus::ShaderDataType::None)), Offset(0), Normalized(false) {}

		BufferElements(ShaderDataType type, const std::string& pName, bool pNormalized = false)
			: Name(pName), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(pNormalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Odysseus::ShaderDataType::Float:  return 1;
				break;
			case Odysseus::ShaderDataType::Float2: return 2;
				break;
			case Odysseus::ShaderDataType::Float3: return 3;
				break;
			case Odysseus::ShaderDataType::Float4: return 4;
				break;
			case Odysseus::ShaderDataType::Mat3:   return 3 * 3;
				break;
			case Odysseus::ShaderDataType::Mat4:   return 4 * 4;
				break;
			case Odysseus::ShaderDataType::Int:    return 1;
				break;
			case Odysseus::ShaderDataType::Int2:   return 2;
				break;
			case Odysseus::ShaderDataType::Int3:   return 3;
				break;
			case Odysseus::ShaderDataType::Int4:   return 4;
				break;
			case Odysseus::ShaderDataType::Bool:   return 1;
				break;
			default:
				break;
			}

			ODC_CORE_ASSERT(false, "Unkown ShaderDataType.");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElements>& element) : m_Elements(element)
		{
			CalculateOffetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); };
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); };
		std::vector<BufferElements>::const_iterator begin()const { return m_Elements.begin(); };
		std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); };
	private:

		void CalculateOffetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride = element.Size;
			}
		}

	private:
		std::vector<BufferElements> m_Elements;
		uint32_t m_Stride = 0;
	};

	class  VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	private:

	};

	class  IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	private:

	};
}