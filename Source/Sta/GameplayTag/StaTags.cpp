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
		UE_DEFINE_GAMEPLAY_TAG(Duration, "StaTags.SetByCaller.Duration");
	}
	
	namespace Ability
	{
		namespace Card
		{
			UE_DEFINE_GAMEPLAY_TAG(Spell, "StaTags.Ability.Card.Spell");
			UE_DEFINE_GAMEPLAY_TAG(Employ, "StaTags.Ability.Card.Employ");
			UE_DEFINE_GAMEPLAY_TAG(Delayed, "StaTags.Ability.Card.Delayed");
			UE_DEFINE_GAMEPLAY_TAG(Deception, "StaTags.Ability.Card.Deception");
		}
	}

	namespace Event
	{
		namespace Card
		{
			UE_DEFINE_GAMEPLAY_TAG(Draw, "StaTags.Event.Card.Draw");
			UE_DEFINE_GAMEPLAY_TAG(Use, "StaTags.Event.Card.Use");
			UE_DEFINE_GAMEPLAY_TAG(Discard, "StaTags.Event.Card.Discard");
		}
		
		namespace Area
		{
			UE_DEFINE_GAMEPLAY_TAG(Move, "StaTags.Event.Area.Move");
			UE_DEFINE_GAMEPLAY_TAG(dispute, "StaTags.Event.Area.dispute");
			UE_DEFINE_GAMEPLAY_TAG(Captured, "StaTags.Event.Area.Captured");
		}
	}

	namespace Interaction
	{
		UE_DEFINE_GAMEPLAY_TAG(Card_Root, "StaTags.Interaction.Card");
		namespace Card
		{
			UE_DEFINE_GAMEPLAY_TAG(None, "StaTags.Interaction.Card.None");
		}
		
		UE_DEFINE_GAMEPLAY_TAG(Area_Root, "StaTags.Interaction.Area");
		namespace Area
		{
			UE_DEFINE_GAMEPLAY_TAG(Info, "StaTags.Interaction.Area.Info");
			UE_DEFINE_GAMEPLAY_TAG(Move, "StaTags.Interaction.Area.Move");
			UE_DEFINE_GAMEPLAY_TAG(Cancel, "StaTags.Interaction.Area.Cancel");
		}
	}
	
	namespace State
	{
		namespace Controller
		{
			UE_DEFINE_GAMEPLAY_TAG(Idle, "StaTags.State.Controller.Idle");
			UE_DEFINE_GAMEPLAY_TAG(Drag, "StaTags.State.Controller.Drag");
			UE_DEFINE_GAMEPLAY_TAG(Menu, "StaTags.State.Controller.Menu");
			UE_DEFINE_GAMEPLAY_TAG(Targeting, "StaTags.State.Controller.Targeting");
		}
		
		namespace Area
		{
			UE_DEFINE_GAMEPLAY_TAG(Blocked, "StaTags.State.Area.Blocked");
		}
	}
}
