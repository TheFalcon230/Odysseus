#pragma once

#include "odcpch.h"

namespace Odysseus
{
#ifndef ODYSSEUS_BUFFER_USAGE_TYPE_STRUCT
#define ODYSSEUS_BUFFER_USAGE_TYPE_STRUCT
	enum class BufferUsageType
	{
		STATIC_DRAW = 0,
		DYNAMIC_DRAW = 1,
		STREAM_DRAW = 2
	};
#endif // !ODYSSEUS_BUFFER_USAGE_TYPE_STRUCT


	class UniformBuffer
	{
	public:
		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding, BufferUsageType usage = BufferUsageType::STATIC_DRAW);
		virtual ~UniformBuffer() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetBindingPoint(uint32_t binding) = 0;
		virtual void AddData(uint32_t offset, uint32_t size, const void* data) = 0;
	};


}