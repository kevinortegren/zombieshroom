#pragma once

#include <Utility/ECS/Include/Component.h>

namespace RootForce
{
    struct DamageAndKnockback : public ECS::Component<DamageAndKnockback>
	{
		float Damage;
		float Knockback;
	};
}