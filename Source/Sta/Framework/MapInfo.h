// Sta Project - Multiplay RTS with GAS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapInfo.generated.h"

UCLASS()
class STA_API AMapInfo : public AActor
{
	GENERATED_BODY()

public:
	AMapInfo();

	static AMapInfo* GetInstance(const UWorld* World);
	
	bool IsOutMap(const FVector2D& Location);

	FVector2D ClampLocation(const FVector2D& Location);

protected:
	UPROPERTY(EditAnywhere, Category = "Sta|Map")
	FVector2D MapSize;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Map")
	FVector2D MapOrigin;
	
	UPROPERTY(EditAnywhere, Category = "Sta|Map")
	TArray<APlayerStart*> StartPoint;

};
