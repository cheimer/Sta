#include "GameplayTag/StaTags.h"



namespace StaTags
{
	UE_DEFINE_GAMEPLAY_TAG(None, "StaTags.None");
	
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG(None, "StaTags.SetByCaller.None");
		
		UE_DEFINE_GAMEPLAY_TAG(Cost, "StaTags.SetByCaller.Cost");
		UE_DEFINE_GAMEPLAY_TAG(UnitNum, "StaTags.SetByCaller.UnitNum");
		UE_DEFINE_GAMEPLAY_TAG(Defense, "StaTags.SetByCaller.Defense");
		
	}
	
	namespace Ability
	{
		namespace Card
		{
			UE_DEFINE_GAMEPLAY_TAG(Spell, "StaTags.Ability.Card.Spell");
			UE_DEFINE_GAMEPLAY_TAG(Employ, "StaTags.Ability.Card.Employ");
		}
		
		namespace Area
		{
			
		}
		
	}

	namespace Event
	{
		namespace Card
		{
			UE_DEFINE_GAMEPLAY_TAG(Draw, "StaTags.Event.Card.Draw");
			UE_DEFINE_GAMEPLAY_TAG(Use, "StaTags.Event.Card.Use");
		}
		
	}
	
}
