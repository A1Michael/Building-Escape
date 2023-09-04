// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// get the owner (in this case the default pawn)
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	

	Owner = GetOwner();
	
}
//opens the door
void UOpenDoor::OpenDoor()
{
	FRotator NewRotation = FRotator(0.f, OpenAngle, 0.f); // value to open door
	Owner->SetActorRotation(NewRotation); 	// SET THE DOOR ROTATION
}
// closes door
void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 90.f, 0.f)); // reset to default
}
// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOnPlate()> 50.f)	// check mass on the pressure plate
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetRealTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) // check if time to close door has come
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float TotalMass = 0.f;
	// find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// iterate through them adding their masses

	// Iterate over overlapping actors
	for (const auto* Actor : OverlappingActors)
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Actor found : %s *!*"), *Actor->GetName());// log name of each actor found

		// add their masses and test
	TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
	}
	
	

	return TotalMass;
}