#pragma once

#include "odcpch.h"

#include "Odysseus/Events/Event.h"

namespace Odysseus
{
	struct WindowProps
	{
		std::string Title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Odysseus Engine",
			uint32_t pWidth = 1280,
			uint32_t pHeight = 720)
			: Title(title), width(pWidth), height(pHeight) {}
	};

	// Interface representing a desktop system based Window
	class ODYSSEUS_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void Update() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}