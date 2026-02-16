// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

#include "LineBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sta.h"
#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"


AAreaBase::AAreaBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;

	AbilitySystemComponent = CreateDefaultSubobject<UStaAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAreaAttributeSet>(TEXT("AttributeSet"));
	
	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>("AreaMesh");
	SetRootComponent(AreaMesh);
	
	AreaMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaMesh->SetCollisionObjectType(ECC_Area);
	AreaMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void AAreaBase::BeginPlay()
{
	Super::BeginPlay();

	SetInteractOptions();

	if (HasAuthority() && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
		UAreaAttributeSet::GetUnitNumAttribute()).AddUObject(this, &ThisClass::OnUnitNumChanged);
		
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			UAreaAttributeSet::GetDefenseAttribute()).AddUObject(this, &ThisClass::OnDefenseChanged);
	}
	
}

void AAreaBase::OnUnitNumChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;

	if (!GetAttributeSet()) return;

	OnAreaValueChanged.Broadcast(this, Data.NewValue, GetAttributeSet()->GetDefense());
	
	//Send Unit Data To Owner Player
	StaDebug::Print(FString::Printf(TEXT("%s : Before %.0f, After %.0f"), *GetNameSafe(this), Data.OldValue, Data.NewValue));
}

void AAreaBase::OnDefenseChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;
	
	if (!GetAttributeSet()) return;

	OnAreaValueChanged.Broadcast(this, GetAttributeSet()->GetUnitNum(), Data.NewValue);
	
	//Send Unit Data To Owner Player
	StaDebug::Print("Defense Change");
}

void AAreaBase::SetInteractOptions()
{
	FInteractOption InfoOption;
	InfoOption.DisplayName = FText::FromString("Info");
	InfoOption.InteractTag = StaTags::Interaction::Area::Info;
	InfoOption.TargetActor = this;
	
	FInteractOption MoveOption;
	MoveOption.DisplayName = FText::FromString("Move");
	MoveOption.InteractTag = StaTags::Interaction::Area::Move;
	InfoOption.TargetActor = this;
	
	FInteractOption CancelOption;
	CancelOption.DisplayName = FText::FromString("Cancel");
	CancelOption.InteractTag = StaTags::Interaction::Area::Cancel;
	InfoOption.TargetActor = this;

	Options.Add(InfoOption);
	Options.Add(MoveOption);
	Options.Add(CancelOption);
	
}

void AAreaBase::AddLine(ALineBase* Line)
{
	ConnectLines.Add(Line);
}

TArray<AAreaBase*> AAreaBase::GetConnectedArea()
{
	TArray<AAreaBase*> ConnectedAreas;

	for(ALineBase* ConnectLine : ConnectLines)
	{
		if (AAreaBase* OtherArea = ConnectLine->GetConnectArea(this))
		{
			ConnectedAreas.Add(OtherArea);
		}
	}

	return ConnectedAreas;
}

void AAreaBase::SetHighlight(bool bIsHighlight)
{
	if (bIsHighlight)
	{
		SpawnedHighlight = UNiagaraFunctionLibrary::SpawnSystemAttached(HighlightVFX, AreaMesh, NAME_None,
			HighlightSpawnLocation, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	else
	{
		if (SpawnedHighlight)
		{
			SpawnedHighlight->DeactivateImmediate();
		}
	}
	
}

void AAreaBase::OnHoverBegin()
{
	StaDebug::Print(FString::Printf(TEXT("%u Area Unit Num : %f"), AreaTeamId.GetId(), GetAttributeSet()->GetUnitNum()));
}

void AAreaBase::OnHoverEnd()
{

}

void AAreaBase::OnInteractBegin(const FHitResult& HitResult)
{

}

void AAreaBase::OnInteracting(const FHitResult& HitResult)
{

}

void AAreaBase::OnInteractEnd(const FHitResult& HitResult)
{

}

const TArray<FInteractOption>& AAreaBase::GetInteractOptions()
{
	return Options;
}

FText AAreaBase::GetInfoText()
{
	if (Options.IsEmpty()) return FText();
	if (!GetAttributeSet()) return FText();
	
	FText InfoText = FText::FromString(FString::Printf(TEXT("%s\nUnit : %d\nDefense : %d"),
		*GetName(), FMath::FloorToInt(GetAttributeSet()->GetUnitNum()), FMath::FloorToInt(GetAttributeSet()->GetDefense())));
	
	return InfoText;
}

/**
 * AbilitySystemInterface
 */
UAbilitySystemComponent* AAreaBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAreaAttributeSet* AAreaBase::GetAttributeSet() const
{
	return AttributeSet;
}

/**
 * GenericTeamAgentInterface
 */
void AAreaBase::SetGenericTeamId(const FGenericTeamId& TeamID)
{
	AreaTeamId = TeamID;
}

FGenericTeamId AAreaBase::GetGenericTeamId() const
{
	return AreaTeamId;
}
