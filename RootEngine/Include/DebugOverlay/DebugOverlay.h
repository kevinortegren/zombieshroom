#pragma once
#include <memory>

namespace RootEngine
{
	class DebugOverlayInterface abstract
	{
	public:
	};

	class DebugOverlay : public DebugOverlayInterface
	{
	public:
		DebugOverlay();
		~DebugOverlay();
	private:
	};
}