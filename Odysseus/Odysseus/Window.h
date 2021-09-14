#pragma once

#include "odcpch.h"

#include "Core.h"
#include "Events/Event.h"

namespace Odysseus
{
	struct WindowProps
	{
		std::string Title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Odysseus Engine",
			unsigned int pWidth = 1280,
			unsigned int pHeight = 720)
			: Title(title), width(pWidth), height(pHeight) {}
	};

	// Interface representing a desktop system based Window
	class ODYSSEUS_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}