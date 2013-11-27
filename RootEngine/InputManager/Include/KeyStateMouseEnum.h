#pragma once
namespace RootEngine
{
	namespace InputManager
	{
		// KeyState specifies whether the key is triggered (edge) or held pressed beyond the trigger (non-edge)
		namespace KeyState
		{
			enum KeyState
			{
				UP,
				DOWN,
				DOWN_EDGE,
				UP_EDGE
			};
		}

		// MouseButton enum is mapped to overlap the key range at the free (unused) SDL scancode IDs for easier key management
		namespace MouseButton
		{
			enum MouseButton
			{
				LEFT = 490,
				MIDDLE,
				RIGHT,
				X1,
				X2
			};
		}
	}
}