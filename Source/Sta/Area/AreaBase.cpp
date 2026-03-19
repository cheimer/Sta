// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

#include "LineBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Sta.h"
#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
#include "Components/TextRenderComponent.h"
#include "FunctionLibrary/AreaCalc.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTag/StaTags.h"
#include "Helper/StaHelper.h"
#include "Kismet/GameplayStatics.h"
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
	
	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);
	TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	TextRenderComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	
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

	check(TeamPaletteData);
	SetAreaMaterialColor(TeamPaletteData->GetDefaultColor());
	SetTextRenderComponent();
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

void AAreaBase::UpdateInteractOptions()
{
	for (FInteractOption& Option : FriendOptions)
	{
		Option.DisplayColor = TeamPaletteData->GetColorByTeamId(GetGenericTeamId());
	}
	for (FInteractOption& Option : HostileOptions)
	{
		Option.DisplayColor = TeamPaletteData->GetColorByTeamId(GetGenericTeamId());
	}
}

void AAreaBase::AttackedBy(AAreaBase* Attacker, const float AttackUnitNum)
{
	if (!Attacker || !Attacker->GetOwningState() || !GetAbilitySystemComponent() || !GetAttributeSet()) return;

	float DefenseValue = UAreaCalc::CalcGetDefenseValue(GetAttributeSet()->GetUnitNum(), GetAttributeSet()->GetDefense());

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(Attacker);

	FGameplayEffectSpecHandle DamageSpec = GetAbilitySystemComponent()->MakeOutgoingSpec(UnitChangeEffectClass, 1.0f, EffectContext);
	if (!DamageSpec.IsValid()) return;

	if (DefenseValue >= AttackUnitNum)
	{
		float RemainUnitNum = UAreaCalc::CalcDefenseValueToUnitNum(DefenseValue - AttackUnitNum, GetAttributeSet()->GetDefense());
		DamageSpec.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, -(GetAttributeSet()->GetUnitNum() - RemainUnitNum));
		
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
	}
	else
	{
		DamageSpec.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, -GetAttributeSet()->GetUnitNum());
		
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
		
		OwningState = Attacker->GetOwningState();
		
		DamageSpec.Data->SetSetByCallerMagnitude(StaTags::SetByCaller::UnitNum, AttackUnitNum - DefenseValue);
		
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
	}

	UpdateInteractOptions();

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
	UpdateVisibility();

	for (AAreaBase* ConnectedArea : GetConnectedArea())
	{
		ConnectedArea->UpdateVisibility();
	}
	
	UpdateInteractOptions();
}

void AAreaBase::UpdateVisibility()
{
	if (!OwningState.IsValid() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->PlayerState)
	{
		SetAreaMaterialColor(TeamPaletteData->GetDefaultColor());
		return;
	}
	
	IGenericTeamAgentInterface* LocalTeam = Cast<IGenericTeamAgentInterface>(GetWorld()->GetFirstPlayerController()->PlayerState);
	if (!LocalTeam)
	{
		SetAreaMaterialColor(TeamPaletteData->GetDefaultColor());
		return;
	}

	bool bIsVisible = false;
	if (LocalTeam->GetGenericTeamId() == GetGenericTeamId())
	{
		bIsVisible = true;
	}
	else
	{
		for (AAreaBase* ConnectArea : GetConnectedArea())
		{
			if (ConnectArea->GetGenericTeamId() == LocalTeam->GetGenericTeamId())
			{
				bIsVisible = true;
				break;
			}
		}
	}

	if (bIsVisible)
	{
		SetAreaMaterialColor(TeamPaletteData->GetColorByTeamId(GetGenericTeamId()));
		SetTextRenderComponent();
	}
	else
	{
		SetAreaMaterialAlpha(TeamPaletteData->GetInvisibleValue());
		SetTextRenderEmpty();
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

void AAreaBase::SetAreaMaterialAlpha(float AlphaValue)
{
	if (!AreaMesh) return;
	
	UMaterialInstanceDynamic* AreaMI = Cast<UMaterialInstanceDynamic>(AreaMesh->GetMaterial(0));
	if (!AreaMI)
	{
		AreaMesh->CreateDynamicMaterialInstance(0);
		AreaMI = Cast<UMaterialInstanceDynamic>(AreaMesh->GetMaterial(0));
		if (!AreaMI) return;
	}

	FLinearColor AreaColor;
	AreaMI->GetVectorParameterValue(FName("GlowColor"), AreaColor);
	
	FLinearColor HSVColor = AreaColor.LinearRGBToHSV();
	HSVColor.B *= AlphaValue;
	
	AreaMI->SetVectorParameterValue(FName("GlowColor"), HSVColor.HSVToLinearRGB());
}

void AAreaBase::SetTextRenderComponent()
{
	if (!TextRenderComponent) return;
	
	if (!OwningState.IsValid() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->PlayerState)
	{
		TextRenderComponent->SetText(FText::GetEmpty());
		TextRenderComponent->SetTextRenderColor(TeamPaletteData->GetDefaultColor().ToFColor(true));
		return;
	}
	
	IGenericTeamAgentInterface* StateTeamID = Cast<IGenericTeamAgentInterface>(GetWorld()->GetFirstPlayerController()->GetPlayerState<APlayerState>());
	if (!StateTeamID || GetGenericTeamId() == FGenericTeamId::NoTeam)
	{
		TextRenderComponent->SetText(FText::GetEmpty());
		TextRenderComponent->SetTextRenderColor(TeamPaletteData->GetDefaultColor().ToFColor(true));
		return;
	}
	
	TextRenderComponent->SetText(GetSimpleInfoText(StateTeamID->GetGenericTeamId()));
	TextRenderComponent->SetTextRenderColor(TeamPaletteData->GetColorByTeamId(GetGenericTeamId()).ToFColor(true));
	
}

void AAreaBase::SetTextRenderEmpty()
{
	if (!TextRenderComponent) return;
	
	TextRenderComponent->SetText(FText::GetEmpty());
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
			*GetName(),
			FMath::RoundToInt(GetAttributeSet()->GetUnitNum()),
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

FText AAreaBase::GetSimpleInfoText(FGenericTeamId Interactor)
{
	if (!GetAttributeSet()) return FText();
	
	if (GetGenericTeamId() == Interactor)
	{
		return FText::FromString(FString::Printf(TEXT("Unit : %d(%d)\nDef : %.1f(%.1f)"),
			FMath::RoundToInt(GetAttributeSet()->GetUnitNum()),
			FMath::RoundToInt(UAreaCalc::CalcBluffUnit(this)),
			GetAttributeSet()->GetDefense(),
			UAreaCalc::CalcBluffDefense(this)
			));
	}
	else
	{
		FString InfoString = FString::Printf(TEXT("Unit : %d\nDef : x%.1f"),
			FMath::RoundToInt(GetAttributeSet()->GetUnitNum()),
			GetAttributeSet()->GetDefense()
			);

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
	if (!HasAuthority()) return;
	
	for (APlayerState* PlayerState : UGameplayStatics::GetGameState(GetWorld())->PlayerArray)
	{
		if (!PlayerState) continue;
		IGenericTeamAgentInterface* TeamAgentInterface  = Cast<IGenericTeamAgentInterface>(PlayerState);
		if (!TeamAgentInterface) continue;

		if (TeamAgentInterface->GetGenericTeamId() == TeamID)
		{
			OwningState = PlayerState;
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
