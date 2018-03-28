// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingGroundsFPS.h"
#include "Tile.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "ActorPool.h"

// Sets default values


ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Error, TEXT("[%s] Setting Pool %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough actor in pool."),*GetName());
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] Checked out: {%s}"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	GetWorld()->GetNavigationSystem()->Build();
}

template<class T>
void ATile::RandomlyPlaceActors(TSubclassOf<T> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)

{

	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
			PlaceActor(ToSpawn, SpawnPosition);
		}

	}

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn,int MinSpawn,int MaxSpawn, float Radius, float MinScale, float MaxScale)
{	
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	RandomlyPlaceActors(ToSpawn, MinSpawn, MaxSpawn, Radius, 1, 1);
}



	bool ATile::FindEmptyLocation(FVector & OutLocation, float Radius)
	{
		
		FBox Bounds(MinExtent, MaxExtent);
		const int MAX_ATTEMTPS = 100;
		for (size_t i = 0; i < MAX_ATTEMTPS; i++)
		{
			FVector CandidatePoint = FMath::RandPointInBox(Bounds);
			if (CanSpawnAtLocation(CandidatePoint, Radius))
			{
				OutLocation = CandidatePoint;
				return true;
			}
		}
		return false;
	}

	void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
	{
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		if (Spawned)
		{
			Spawned->SetActorRelativeLocation(SpawnPosition.Location);
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
			Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
		}
	}

	void ATile::PlaceActor(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
	{
		FRotator Rotation = FRotator(0, SpawnPosition.Rotation, 0);
		APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPosition.Location, Rotation);
		if (Spawned == nullptr)
		{
			return;
		}
			
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));			
			Spawned->SpawnDefaultController();
			Spawned->Tags.Add(FName("Enemy"));
		
	}

void ATile::HujCiNaKurwe(int cwel, int menda)
{
	return;
}



// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	CanSpawnAtLocation(GetActorLocation(), 300);
	CanSpawnAtLocation(GetActorLocation()+ FVector(0,0,1000), 300);

	
}
void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Pool != nullptr && NavMeshBoundsVolume != nullptr)
	{
		Pool->Return(NavMeshBoundsVolume);
	}
}


// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
bool ATile::CanSpawnAtLocation(FVector Location, float Radius )
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel
	(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);
	
	return !HasHit;
	
}

