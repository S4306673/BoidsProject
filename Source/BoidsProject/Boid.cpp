// Fill out your copyright notice in the Description page of Project Settings.

#include "Boid.h"
#include "BoidManager.h"

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();
	boidLocation = GetActorLocation();
}

FVector ABoid::Seek(FVector position, float radius) {
	FVector targetVelocity = FVector::ZeroVector;
	if (FVector::Dist(position, boidLocation) < (radius / 4)) {
		targetVelocity = position - boidLocation;
		targetVelocity *= Manager->arrivalSpeed;
	}
	else {
		targetVelocity = position - boidLocation;
	}
	targetVelocity.Normalize();
	return targetVelocity;
}
FVector ABoid::Flee(FVector position) {
	return Seek(position, Manager->arrivalRadius) * -1;
}
FVector ABoid::Pursue(ABoid* targetBoid, FVector targetDestination) {
	FVector targetVelocity = Seek((targetDestination + targetBoid->currentVelocity), Manager->arrivalRadius);
	targetVelocity.Normalize();
	return targetVelocity;
}
FVector ABoid::Evade(ABoid* targetBoid, FVector targetDestination) {
	FVector targetVelocity = Flee((targetDestination + targetBoid->currentVelocity));
	targetVelocity.Normalize();
	return targetVelocity;
}
FVector ABoid::Wander(float radius, float distance) {
	if (FVector::Dist(boidLocation, wanderDestination) < 100) {
		FindWanderDestination(radius, distance);
	}
	//this helps reset wander to prevent boid getting stuck on boundary edges
	else if (FVector::Dist(boidLocation, wanderDestination) > Manager->wanderDistance) {
		FindWanderDestination(radius, distance);
	}
	if (Manager->drawWanderDebugCheck) {
		DrawDebugSphere(GetWorld(), wanderDestination, 50, 8, FColor::Magenta);
	}
	return Seek(wanderDestination, Manager->arrivalRadius);
}
FVector ABoid::FindWanderDestination(float radius, float distance) {
	wanderDestination = boidLocation + (GetActorForwardVector() * distance) + (FMath::VRand() * radius);
	for (int i = 0; i < 20; i++) {
		//check destination is within boundary
		if (FVector::Dist(wanderDestination, Manager->managerLocation) > Manager->boundary && wanderDestination.Z > (Manager->managerLocation.Z + 500) && wanderDestination.Z < (Manager->managerLocation.Z - 500)) {
			wanderDestination = boidLocation + (GetActorForwardVector() * distance) + (FMath::VRand() * radius);
		}
		else break;
	}
	if (FVector::Dist(wanderDestination, Manager->managerLocation) > Manager->boundary && wanderDestination.Z > (Manager->managerLocation.Z + 500) && wanderDestination.Z < (Manager->managerLocation.Z - 500)) {
		wanderDestination = Manager->managerLocation;
	}
	return wanderDestination;
}
void ABoid::BoundaryCheck(FVector managerLocation, float boundary, FVector newVelocity, float deltaTime) {
	//always restrict height between +500 and -500 of manager location
	if (boidLocation.Z > 500 + managerLocation.Z) {
		newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) - FVector(0, 0, 200)), Manager->arrivalRadius);
		newVelocity.Normalize();
		FVector newForce = newVelocity;
		currentVelocity += newForce * deltaTime;
		boidLocation += (currentVelocity * Manager->speed * deltaTime);
		SetActorLocation(boidLocation);
		SetActorRotation(currentVelocity.Rotation());
	}
	else if (boidLocation.Z < managerLocation.Z - 500) {
		newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) + FVector(0, 0, 200)), Manager->arrivalRadius);
		newVelocity.Normalize();
		FVector newForce = newVelocity;
		currentVelocity += newForce * deltaTime;
		boidLocation += (currentVelocity * Manager->speed * deltaTime);
		SetActorLocation(boidLocation);
		SetActorRotation(currentVelocity.Rotation());
	}
	//If wrapping boundary is enabled teleport to opposite side when exceeding boundary
	if (Manager->wrapBoundaryCheck) {
		if (boidLocation.X > boundary - managerLocation.X) {
			SetActorLocation(FVector(boidLocation.X - boundary * 2, boidLocation.Y, boidLocation.Z));
		}
		else if (boidLocation.X < managerLocation.X - boundary) {
			SetActorLocation(FVector(boidLocation.X + boundary * 2, boidLocation.Y, boidLocation.Z));
		}
		if (boidLocation.Y > boundary - managerLocation.Y) {
			SetActorLocation(FVector(boidLocation.X, boidLocation.Y - boundary * 2, boidLocation.Z));
		}
		else if (boidLocation.Y < managerLocation.Y - boundary) {
			SetActorLocation(FVector(boidLocation.X, boidLocation.Y + boundary * 2, boidLocation.Z));
		}
	}
	//If wrapping boundary is disabled apply seek force away from boundary when exceeded
	else if (!Manager->wrapBoundaryCheck) {
		if (boidLocation.X > boundary + managerLocation.X) {
			newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) - FVector(200, 0, 0)), Manager->arrivalRadius);
			newVelocity.Normalize();
			FVector newForce = newVelocity;
			currentVelocity += newForce * deltaTime;
			boidLocation += (currentVelocity * Manager->speed * deltaTime);
			SetActorLocation(boidLocation);
			SetActorRotation(currentVelocity.Rotation());
		}
		else if (boidLocation.X < managerLocation.X - boundary) {
			newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) + FVector(200, 0, 0)), Manager->arrivalRadius);
			newVelocity.Normalize();
			FVector newForce = newVelocity;
			currentVelocity += newForce * deltaTime;
			boidLocation += (currentVelocity * Manager->speed * deltaTime);
			SetActorLocation(boidLocation);
			SetActorRotation(currentVelocity.Rotation());
		}
		if (boidLocation.Y > boundary + managerLocation.Y) {
			newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) - FVector(0, 200, 0)), Manager->arrivalRadius);
			newVelocity.Normalize();
			FVector newForce = newVelocity;
			currentVelocity += newForce * deltaTime;
			boidLocation += (currentVelocity * Manager->speed * deltaTime);
			SetActorLocation(boidLocation);
			SetActorRotation(currentVelocity.Rotation());
		}
		else if (boidLocation.Y < managerLocation.Y - boundary) {
			newVelocity = Seek(boidLocation + (FVector(FMath::VRand() * 100) + FVector(0, 200, 0)), Manager->arrivalRadius);
			newVelocity.Normalize();
			FVector newForce = newVelocity;
			currentVelocity += newForce * deltaTime;
			boidLocation += (currentVelocity * Manager->speed * deltaTime);
			SetActorLocation(boidLocation);
			SetActorRotation(currentVelocity.Rotation());
		}
	}
}
FVector ABoid::Alignment(TArray<ABoid*> neighbours)
{
	if (neighbours.Num() == 0) return FVector::ZeroVector;
	FVector avgLocation = FVector::ZeroVector;
	for (ABoid* neighbour : neighbours) {
		avgLocation += (neighbour->GetActorLocation());
	}
	avgLocation = avgLocation / neighbours.Num();
	return avgLocation;
}
FVector ABoid::Cohesion(TArray<ABoid*> neighbours)
{
	if (neighbours.Num() == 0) return FVector::ZeroVector;
	FVector avgVelocity = FVector::ZeroVector;
	for (ABoid* neighbour : neighbours) {
		avgVelocity += neighbour->GetActorLocation();
	}
	avgVelocity = avgVelocity / neighbours.Num();
	avgVelocity.Normalize();
	return avgVelocity;
}
FVector ABoid::Separation(TArray<ABoid*> neighbours)
{
	if (neighbours.Num() == 0) return FVector::ZeroVector;
	FVector avgVelocity = FVector::ZeroVector;
	for (ABoid* neighbour : neighbours) {
		avgVelocity += (Flee(neighbour->GetActorLocation()));
	}
	avgVelocity = avgVelocity / neighbours.Num();
	avgVelocity.Normalize();
	return avgVelocity;
}
// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABoid::UpdateBoid(float deltaTime) {
	TArray<ABoid*> neighbourhood = Manager->GetBoidNeighbourhood(this, Manager->neighbourhoodRadius);
	FVector newVelocity = FVector::ZeroVector;

	//Weighted Truncated Running Sum with Priority
	if (newVelocity.Size() < 100.0f) {
		newVelocity += Separation(neighbourhood) * Manager->separationWeight;
	}
	if (newVelocity.Size() < 100.0f) {
		newVelocity += Cohesion(neighbourhood) * Manager->cohesionWeight;
	}
	if (newVelocity.Size() < 100.0f) {
		newVelocity += Alignment(neighbourhood) * Manager->alignmentWeight;
	}
	if (newVelocity.Size() < 120.0f) {
		newVelocity += Wander(Manager->wanderRadius, Manager->wanderDistance);
	}
	newVelocity.Normalize();
	FVector newForce = (newVelocity - currentVelocity);
	currentVelocity += newForce * deltaTime;
	if (Manager->drawVectorDebugCheck) {
		DrawDebugLine(GetWorld(), boidLocation, (boidLocation + newVelocity) * 5, FColor::Red);
	}
	boidLocation += (currentVelocity * Manager->speed * deltaTime);

	SetActorLocation(boidLocation);
	SetActorRotation(currentVelocity.Rotation());
	BoundaryCheck(Manager->managerLocation, Manager->boundary, newVelocity, deltaTime);
}

