// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
//Finds and set up input component and finds Physics Handle
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	
	SetUpInputComponent();
	FindPhysicsHandleComponent();
}

// look for  input component(only apprears at runtime
void UGrabber::SetUpInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>(); // gets the physics component from the BP
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"));
		
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);//Bind the InputAxis
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);//Bind the InputAxis
	}
	else
	{
		// owner is the name of the BP ref by pointer
		UE_LOG(LogTemp, Error, TEXT("Cannot find Input component.\n"), *GetOwner()->GetName());
	}
}
//look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>(); // gets the physics component from the BP
	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle Complete.\n"));
	}
	else
	{
		// owner is the name of the BP ref by pointer
		UE_LOG(LogTemp, Error, TEXT("Cannot find Physics Handle component.\n"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	// try LINE TRACE and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyHit();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// if we hit something then attach a physics handle
	if (ActorHit != nullptr)
	{

		// grabs the component based on the hitresult
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			HitResult.Location,
			FRotator::ZeroRotator);
		UE_LOG(LogTemp, Warning, TEXT("Grabbing %s.\n"),*ActorHit->GetName());

	}
}
void UGrabber::Release()
{
	//TODO Release Physics
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// if the physics handle is attached
		// move the object that we're holding

	// get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector 87H, = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// draw debug line
	DrawDebugLine(GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 10),
		false,
		0.f,
		0.f,
		10.f
	);
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);// move  the object attached
		UE_LOG(LogTemp, Warning, TEXT("Moving Object.\n"));

	}
}
const FHitResult UGrabber::GetFirstPhysicsBodyHit()
{
	// get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;// reach of raycast

	// draw debug line
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		false,
		0.f,
		0.f,
		
			);

	/// setup query parameters
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	FHitResult LineTraceHit;
	// Raycast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT LineTraceHit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParam
	);

	// if we find a actor, return hit
	AActor* ActorHit = LineTraceHit.GetActor();
	if (ActorHit)
	{																				// my implementation
		UE_LOG(LogTemp, Warning, TEXT("Actor Hit is : %s"), *ActorHit->GetName()); // UE_LOG(LogTemp, Warning, TEXT("Actor Hit is : %s"),*LineTraceHit.GetActor()->GetName()); 
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("raycast failed!!!!\n"));

	}

	return LineTraceHit;
}
