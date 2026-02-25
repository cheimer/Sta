// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

#include "LineBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sta.h"
#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "FunctionLibrary/AreaCalc.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Net/UnrealNetwork.h"


AAreaBase::AAreaBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	bAlwaysRelevant = true;

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

void AAreaBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, OwningState);
	
}

void AAreaBase::BeginPlay()
{
	Super::BeginPlay();

	SetInteractOptions();

	if (HasAuthority() && GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			UAreaAttributeSet::GetUnitNumAttribute()).AddUObject(this, &ThisClass::OnValueChanged);
		
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			UAreaAttributeSet::GetDefenseAttribute()).AddUObject(this, &ThisClass::OnValueChanged);
		
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			UAreaAttributeSet::GetBluffUnitAddAttribute()).AddUObject(this, &AAreaBase::OnBluffChanged);
		
		GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(
			UAreaAttributeSet::GetBluffDefenseAddAttribute()).AddUObject(this, &AAreaBase::OnBluffChanged);
		
	}

	SetAreaMaterialColor(NeutralColor);
}

void AAreaBase::OnValueChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;
	if (!GetAttributeSet()) return;

	OnAreaValueChanged.Broadcast(this, GetAttributeSet()->GetUnitNum(), GetAttributeSet()->GetDefense());
	
	StaDebug::Print(FString::Printf(TEXT("%s Value : Before %.0f, After %.0f\n"), *GetNameSafe(this), Data.OldValue, Data.NewValue));
}

void AAreaBase::OnBluffChanged(const FOnAttributeChangeData& Data)
{
	if (!HasAuthority()) return;
	if (!GetAttributeSet()) return;

	OnAreaBluffChanged.Broadcast(this, GetAttributeSet()->GetBluffUnitAdd(), GetAttributeSet()->GetBluffDefenseAdd());
	
	StaDebug::Print(FString::Printf(TEXT("%s Bluff : Before %.0f, After %.0f\n"), *GetNameSafe(this), Data.OldValue, Data.NewValue));
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
	MoveOption.TargetActor = this;
	
	FInteractOption ScanOption;
	ScanOption.DisplayName = FText::FromString("Scan");
	ScanOption.InteractTag = StaTags::Interaction::Area::Scan;
	ScanOption.TargetActor = this;

	FInteractOption CancelOption;
	CancelOption.DisplayName = FText::FromString("Cancel");
	CancelOption.InteractTag = StaTags::Interaction::Area::Cancel;
	CancelOption.TargetActor = this;

	FriendOptions.Add(InfoOption);
	FriendOptions.Add(MoveOption);
	FriendOptions.Add(CancelOption);

	HostileOptions.Add(InfoOption);
	HostileOptions.Add(ScanOption);
	HostileOptions.Add(CancelOption);
	
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

void AAreaBase::OnRep_OwningState()
{
	if (!OwningState.IsValid() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->PlayerState)
	{
		SetAreaMaterialColor(NeutralColor);
		return;
	}

	if (GetWorld()->GetFirstPlayerController()->PlayerState == OwningState)
	{
		SetAreaMaterialColor(FriendlyColor);
	}
	else
	{
		SetAreaMaterialColor(HostileColor);
	}

}

void AAreaBase::SetAreaMaterialColor(FLinearColor Color)
{
	if (!AreaMesh) return;
	
	UMaterialInstanceDynamic* AreaMI = Cast<UMaterialInstanceDynamic>(AreaMesh->GetMaterial(0));
	if (!AreaMI)
	{
		AreaMesh->CreateDynamicMaterialInstance(0);
		AreaMI = Cast<UMaterialInstanceDynamic>(AreaMesh->GetMaterial(0));
		if (!AreaMI) return;
	}

	AreaMI->SetVectorParameterValue(FName("GlowColor"), Color);
	
}

void AAreaBase::SetLastScanTime()
{
	LastScanTime = GetWorld()->GetUnpausedTimeSeconds();
}

void AAreaBase::OnHoverBegin()
{

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

const TArray<FInteractOption>& AAreaBase::GetInteractOptions(FGenericTeamId Interactor)
{
	if (GetGenericTeamId() == Interactor)
	{
		return FriendOptions;
	}
	else
	{
		return HostileOptions;
	}
}

FText AAreaBase::GetInfoText(FGenericTeamId Interactor)
{
	if (!GetAttributeSet()) return FText();
	
	if (GetGenericTeamId() == Interactor)
	{
		return FText::FromString(FString::Printf(TEXT("%s\nUnit : %d (Bluff : %d)\nDefense : x%.1f (Bluff : x%.1f)"),
			*GetName(),
			FMath::RoundToInt(GetAttributeSet()->GetUnitNum()),
			FMath::RoundToInt(UAreaCalc::CalcBluffUnit(this)),
			GetAttributeSet()->GetDefense(),
			UAreaCalc::CalcBluffDefense(this)
			));
	}
	else
	{
		FString InfoString = FString::Printf(TEXT("%s\nUnit : %d\nDefense : x%.1f"),
			*GetName(), FMath::RoundToInt(GetAttributeSet()->GetUnitNum()),
			GetAttributeSet()->GetDefense()
			);
		
		if (GetLastScanTime() > 0)
		{
			InfoString += FString::Printf(TEXT("\nLast Scan : %lld seconds age"),
				FMath::FloorToInt(FMath::Clamp(GetWorld()->GetUnpausedTimeSeconds() - GetLastScanTime(), 0.0f, 99.0f)));
		}

		return FText::FromString(InfoString);
	}
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
	TArray<AActor*> FoundActors;
	for (FConstPlayerControllerIterator PCIter = GetWorld()->GetPlayerControllerIterator(); PCIter; ++PCIter)
	{
		IGenericTeamAgentInterface* PlayerTeam  = Cast<IGenericTeamAgentInterface>(PCIter->Get()->PlayerState);
		if (!PlayerTeam) continue;

		if (PlayerTeam->GetGenericTeamId() == TeamID)
		{
			OwningState = Cast<APlayerState>(PCIter->Get()->PlayerState);
			break;
		}
	}
	
}

FGenericTeamId AAreaBase::GetGenericTeamId() const
{
	IGenericTeamAgentInterface* GenericTeam = Cast<IGenericTeamAgentInterface>(OwningState);
	if (!GenericTeam) return FGenericTeamId::NoTeam;

	return GenericTeam->GetGenericTeamId();
}
