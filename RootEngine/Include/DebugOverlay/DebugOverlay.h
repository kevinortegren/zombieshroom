#pragma once
#include <Rocket/Core.h>
#include <memory>

namespace RootEngine
{
	class DebugOverlayInterface abstract
	{
	public:
		virtual void AttachGUIElement(Rocket::Core::Element* p_element, bool p_alignLeft = true) = 0;

		virtual void Initialize(std::shared_ptr<Rocket::Core::ElementDocument> p_document) = 0;
	};

	class DebugOverlay : public DebugOverlayInterface
	{
	public:
		DebugOverlay();
		~DebugOverlay();
		void AttachGUIElement(Rocket::Core::Element* p_element, bool p_alignLeft = true);
		void Initialize(std::shared_ptr<Rocket::Core::ElementDocument> p_document);
	private:
		std::shared_ptr<Rocket::Core::ElementDocument> m_document;
	};
}