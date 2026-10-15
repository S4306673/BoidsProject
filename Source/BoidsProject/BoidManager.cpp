// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidManager.h"
#include "Boid.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABoidManager::ABoidManager() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transform = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	this->SetRootComponent(transform);
}

void ABoidManager::SpawnBoids() {
	//clear existing boids before spawning new ones
	if (myBoids.Num() != 0) {
		for (ABoid* Boid : myBoids) {
			Boid->Destroy();
		}
		myBoids.Empty();
	}
	for (int i = 0; i < spawnCount; i++) {
		FVector spawnLocation = (FMath::VRand() * spawnRadius) + GetActorLocation();
		FRotator spawnRotation = GetActorRotation();

		ABoid* newboid = GetWorld()->SpawnActor<ABoid>(model, spawnLocation, spawnRotation);
		newboid->Manager = this;
		myBoids.Add(newboid);
	}
	//GetTunaPath();
}

// Called when the game starts or when spawned
void ABoidManager::BeginPlay() {
	Super::BeginPlay();
	managerLocation = GetActorLocation();

	frameCount = 0;
}

// Called every frame
void ABoidManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (drawBoundaryDebugCheck) {
		DrawDebugBox(GetWorld(), managerLocation, FVector(boundary, boundary, 500), FQuat::Identity, FColor::Emerald);
	}
	switch (frameCount % 2) {
	case 0:
		for (int i = 0; i < myBoids.Num(); i++) {
			myBoids[i]->UpdateBoid(DeltaTime);
			i++;
		}
		break;
	case 1:
		for (int i = 1; i < myBoids.Num(); i++) {
			myBoids[i]->UpdateBoid(DeltaTime);
			i++;
		}
	}
	frameCount++;
}
//each boid calls this to get nearby boids within a set radius to apply flocking behaviours
TArray<class ABoid*> ABoidManager::GetBoidNeighbourhood(ABoid* thisBoid, float radius) {
	TArray<class ABoid*> returnBoids;
	for (ABoid* Boid : myBoids) {
		if (Boid == thisBoid) continue;
		float aDistance = (Boid->GetActorLocation() - thisBoid->GetActorLocation()).Size();
		if (aDistance < radius) {
			returnBoids.Add(Boid);
		}
	}
	return returnBoids;
}

//void ABoidManager::GetTunaPath() {
//	tunaPath.Empty();
//	FVector newLocation = FVector(boundary, 0, 0);
//	tunaPath.Add(newLocation);
//	FVector currentPoint = newLocation;
//	//find random location to plot path
//	for (int i = 0; i < (numOfPathPoints * 0.5f); i++) {
//		currentPoint += FMath::VRand() * 500 + FVector(1000, 0, 0);
//		tunaPath.Add(currentPoint);
//	}
//	//halfway point to head back to boundary
//	newLocation = tunaPath[numOfPathPoints * 0.5f] + FVector(0, 500, 0);
//	for (int i = 0; i < (numOfPathPoints * 0.5f); i++) {
//		currentPoint += FMath::VRand() * 500 - FVector(1000, 0, 0);
//		tunaPath.Add(currentPoint);
//	}
//}

