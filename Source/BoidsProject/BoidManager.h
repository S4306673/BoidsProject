// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoidManager.generated.h"

UCLASS()
class BOIDSPROJECT_API ABoidManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoidManager();
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	//class AObstacleManager* ObstacleManager;
	UFUNCTION(BlueprintCallable)
	void SpawnBoids();
	//UFUNCTION(BlueprintCallable)
	//void GetTunaPath();
	int frameCount;
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Setting")
	UClass* model;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Setting")
	int spawnCount;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float spawnRadius;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float boundary;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	float boundaryVelocity;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Boundary Settings")
	bool wrapBoundaryCheck;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float speed;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float arrivalSpeed;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float neighbourhoodRadius;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "50.0"), Category = "Flocking Settings")
	float separationWeight;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "50.0"), Category = "Flocking Settings")
	float cohesionWeight;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "50.0"), Category = "Flocking Settings")
	float alignmentWeight;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "500.0"), Category = "Wander Settings")
	float wanderDistance;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "250.0"), Category = "Wander Settings")
	float wanderRadius;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
	bool drawBoundaryDebugCheck = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
	bool drawWanderDebugCheck = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
	bool drawVectorDebugCheck = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
	bool drawPathDebug = true;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Debug Settings")
	bool drawAllDebugCheck = true;

	float arrivalRadius;
	FCollisionQueryParams Params;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Obstacle Setting")
	float raycastDistance;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Obstacle Setting")
	float obstacleRadius;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Seagull Setting")
	float targetDistance;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Tuna Setting")
	bool followingPath;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Tuna Setting")
	int numOfPathPoints;
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite, Category = "Tuna Setting")
	int queueRadius;

	FVector managerLocation;
	USceneComponent* transform;
	UPROPERTY(BlueprintReadOnly)
	TArray<class ABoid*> myBoids;
	TArray<FVector> tunaPath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<class ABoid*> GetBoidNeighbourhood(ABoid* thisBoid, float neighbourhoodRadius);

};
