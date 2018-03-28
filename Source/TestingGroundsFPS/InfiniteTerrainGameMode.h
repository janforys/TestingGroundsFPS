// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestingGroundsFPSGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */




UCLASS()
class TESTINGGROUNDSFPS_API AInfiniteTerrainGameMode : public ATestingGroundsFPSGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
		void PopulateBoundsVolumePool();

	

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
    class UActorPool* NavMeshBoundsVolumePool;

private:
	void AddToPool(ANavMeshBoundsVolume * VolumeToAdd);
	

	
};
