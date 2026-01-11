#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace StaTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	
	namespace Ability
	{
		namespace Card
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spell);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Employ);
		}
		
	}

	namespace Event
	{
		namespace Card
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Draw);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Use);
		}
		
	}
	
}
