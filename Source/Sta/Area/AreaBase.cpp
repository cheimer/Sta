// Sta Project - Multiplay RTS with GAS


#include "AreaBase.h"

#include "Sta.h"
#include "AbilitySystem/StaAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AreaAttributeSet.h"
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
	
}

void AAreaBase::OnHoverBegin()
{
	StaDebug::Print(FString::Printf(TEXT("Area Unit Num : Not yet")));
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
	StaDebug::Print("Area Interacting");
}

void AAreaBase::OnInteractEnd(const FHitResult& HitResult)
{
	StaDebug::Print("Area Interact End");
}

UAbilitySystemComponent* AAreaBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAreaBase::GetAttributeSet() const
{
	return AttributeSet;
}
