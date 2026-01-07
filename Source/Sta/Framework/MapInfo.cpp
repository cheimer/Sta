// Sta Project - Multiplay RTS with GAS


#include "MapInfo.h"

#include "Kismet/GameplayStatics.h"


AMapInfo::AMapInfo()
{
	PrimaryActorTick.bCanEverTick = false;
	
	
}

AMapInfo* AMapInfo::GetInstance(const UWorld* World)
{
	if (!World) return nullptr;

	return Cast<AMapInfo>(UGameplayStatics::GetActorOfClass(World, StaticClass()));
}

bool AMapInfo::IsOutMap(const FVector2D& Location)
{
	if (Location.X > MapOrigin.X + MapSize.X || Location.X < MapOrigin.X - MapSize.X) return true;
	if (Location.Y > MapOrigin.Y + MapSize.Y || Location.Y < MapOrigin.Y - MapSize.Y) return true;

	return false;
}

FVector2D AMapInfo::ClampLocation(const FVector2D& Location)
{
	FVector2D ClampLoc;
	ClampLoc.X = FMath::Clamp(Location.X, MapOrigin.X - MapSize.X, MapOrigin.X + MapSize.X);
	ClampLoc.Y = FMath::Clamp(Location.Y, MapOrigin.Y - MapSize.Y, MapOrigin.Y + MapSize.Y);

	return ClampLoc;
}
