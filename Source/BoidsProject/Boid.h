// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"

UCLASS()
class BOIDSPROJECT_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	FVector Seek(FVector position, float radius);
	FVector Flee(FVector position);
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Velocity")
	FVector currentVelocity;
	FVector boidLocation;
	UPROPERTY(BlueprintReadOnly, Category = "Settings")
	class ABoidManager* Manager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector Pursue(ABoid* target, FVector targetDestination);
	FVector Evade(ABoid* target, FVector targetDestination);
	FVector Wander(float radius, float distance);
	FVector wanderDestination;
	FVector FindWanderDestination(float radius, float distance);
	FVector BoundaryCheck(FVector managerLocation, float boundaryRadius, float deltaTime);

	FVector Alignment(TArray<ABoid*> neighbours);
	FVector Cohesion(TArray<ABoid*> neighbours);
	FVector Separation(TArray<ABoid*> neighbours);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateBoid(float deltaTime);
};
