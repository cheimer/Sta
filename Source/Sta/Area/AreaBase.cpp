// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

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
	
	AreaMesh->SetCastShadow(false);
	

}

void AAreaBase::BeginPlay()
{
	Super::BeginPlay();

	SetInteractOptions();
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

void AAreaBase::OnHoverBegin()
{
	StaDebug::Print(FString::Printf(TEXT("Area Unit Num : %f"), GetAttributeSet()->GetUnitNum()));
}

void AAreaBase::OnHoverEnd()
{
	StaDebug::Print("Area Hover End");
}

void AAreaBase::OnInteractBegin(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interact Begin");
}

void AAreaBase::OnInteracting(const FHitResult& HitResult)
{

}

void AAreaBase::OnInteractEnd(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interact End");
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

UAbilitySystemComponent* AAreaBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAreaAttributeSet* AAreaBase::GetAttributeSet() const
{
	return AttributeSet;
}
