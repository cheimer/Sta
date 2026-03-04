#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace StaTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	
	namespace SetByCaller
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cost);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(UnitNum);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Defense);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BluffUnitNum);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(BluffDefense);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Duration);
	}
	
	namespace Ability
	{
		namespace Card
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spell);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Employ);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bluff);
		}
	}

	namespace Event
	{
		namespace Card
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Draw);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Use);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Discard);
		}
		
		namespace Area
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Move);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attack);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scan);
		}
	}
	
	namespace Interaction
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Card_Root);
		namespace Card
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
		}
		
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Area_Root);
		namespace Area
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Info);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Move);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scan);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Cancel);
		}
	}
	
	namespace State
	{
		namespace Controller
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Idle);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Drag);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Menu);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Targeting);
		}

		namespace Area
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Blocked);
		}
	}
}
