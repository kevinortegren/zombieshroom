#include "DebugOverlay\DebugOverlay.h"

namespace RootEngine
{
	DebugOverlay::DebugOverlay()
	{
	}

	DebugOverlay::~DebugOverlay()
	{
		if(m_document != nullptr)
			m_document->RemoveReference();
	}

	void DebugOverlay::Initialize(std::shared_ptr<Rocket::Core::ElementDocument> p_document)
	{
		m_document = p_document;
	}

	void DebugOverlay::AttachGUIElement(Rocket::Core::Element* p_element, bool p_alignLeft)
	{
		m_document->GetElementById( p_alignLeft?"left":"right" )->AppendChild(p_element->Clone());
	}
}