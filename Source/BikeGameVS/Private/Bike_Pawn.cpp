// Fill out your copyright notice in the Description page of Project Settings.


#include "Bike_Pawn.h"
#include "Components/PrimitiveComponent.h" 
#include "Math/Vector.h" 
#include "Wheel.h"
#include "DrawDebugHelpers.h" 
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 

// Sets default values
ABike_Pawn::ABike_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABike_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABike_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABike_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABike_Pawn::SetReferences(AWheel* FrontWheelReference, AWheel* RearWheelReference, UStaticMeshComponent* BodyReference)
{
	mFWheelRef = FrontWheelReference;
	mRWheelRef = RearWheelReference;
	mBodyRef = BodyReference;
}

void ABike_Pawn::TraceWheels()
{
	// FrontWheel
	FVector lTraceStartF = mFWheelRef->GetActorLocation();
	FVector lLineTraceEndF = mFWheelRef->GetActorUpVector() * -150.0f + lTraceStartF;

	FCollisionQueryParams lTraceParamsF;

	TArray<AActor*> lActorsToIgnoreF;
	lActorsToIgnoreF.Add(mBodyRef->GetOwner());
	lActorsToIgnoreF.Add(mFWheelRef);

	bool lFrontWheelOnGround = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		lTraceStartF,
		lLineTraceEndF,
		TraceTypeQuery1,
		false,
		lActorsToIgnoreF,
		EDrawDebugTrace::None,
		mFWheelHit,
		true,
		FLinearColor(0.f, 255.f, 0.f),
		FLinearColor(255.f, 0.f, 0.f));

	// RearWheel
	FVector lTraceStartR = mRWheelRef->GetActorLocation();
	FVector lLineTraceEndR = mRWheelRef->GetActorUpVector() * -150.0f + lTraceStartR;

	FCollisionQueryParams lTraceParamsR;

	TArray<AActor*> lActorsToIgnoreR;
	lActorsToIgnoreR.Add(mBodyRef->GetOwner());
	lActorsToIgnoreR.Add(mRWheelRef);

	bool lRearWheelOnGround = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(), 
		lTraceStartR, 
		lLineTraceEndR,
		TraceTypeQuery1, 
		false, 
		lActorsToIgnoreR,
		EDrawDebugTrace::None,
		mRWheelHit,
		true,
		FLinearColor(0.f, 255.f, 0.f),
		FLinearColor(255.f, 0.f, 0.f));

	// Set mInAir
	if (lFrontWheelOnGround == false)
	{
		if (lRearWheelOnGround == false)
		{
			mInAir = true;
		}
		else
		{
			mInAir = false;
		}
	}
	else
	{
		mInAir = false;
	}

	mFrameVelocity = mBodyRef->GetComponentVelocity();
}

void ABike_Pawn::AddCorrectionForce()
{
	mOldError = mError;
	mError = mTargetRoll - mBodyRef->GetRelativeRotation().Roll;
	
	float lErrorSpeed = (mError - mOldError) / GetWorld()->GetDeltaSeconds();
	float lP = mError * mSpringCof;
	float lD = lErrorSpeed * mSpringDamp;

	FVector lTorqueVector = (lP + lD) * mBodyRef->GetForwardVector() * -1.0f;

	if (mInAir == false)
	{
		if (mBodyRef->GetRelativeRotation().Pitch < 50.0f)
		{
			if (mBodyRef->GetRelativeRotation().Pitch > -60.0f)
			{
				mBodyRef->AddTorqueInDegrees(lTorqueVector, NAME_None, true);
			}
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("TorqueVector: %s"), *mBodyRef->GetForwardVector().ToString());
}

void ABike_Pawn::ExitBike()
{
	FVector lActorLocation = mBodyRef->GetOwner()->GetActorLocation();
	FVector lSpawnLocation = FVector(lActorLocation.X+150, lActorLocation.Y, lActorLocation.Z);
	float lSpawnRotationZ = mBodyRef->GetOwner()->GetActorRotation().Yaw; 
	FRotator lSpawnRotation = FRotator(0, 0, lSpawnRotationZ);
	FActorSpawnParameters lSpawnParams;

	if (!ensure(mPawnToSpawn)) { return; }

	APawn* lSpawnedCharacter = GetWorld()->SpawnActor<APawn>(mPawnToSpawn, lSpawnLocation, lSpawnRotation, lSpawnParams);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(lSpawnedCharacter);
}

void ABike_Pawn::DrawLine(FVector Start, FVector Direction, float Length, FColor Color)
{
	DrawDebugLine
	(
		GetWorld(),
		Start,
		Start + Direction * Length,
		Color,
		false,
		-1,
		0,
		3
	);
}

void ABike_Pawn::FakeForces()
{
	FVector lOutputForce;
	FVector lSlipVelocity;
	// float lNormalLoad;
	float lSimulatedVelocity;

	lSimulatedVelocity = mFWheelRef->Radius * mFWheelRef->RotationRate;
	lSlipVelocity = mFrameVelocity * -1 + lSimulatedVelocity;

	FVector lRearWheelVelocity = mRWheelRef->GetVelocity();

	FVector lRearWheelForward = mRWheelRef->GetActorForwardVector();

	FVector lRearWheelForwardVel = lRearWheelForward * FVector::DotProduct(lRearWheelVelocity, lRearWheelForward);

	// mBodyRef->AddForceAtLocation(lRearWheelVelocity, mRWheelHit.Location);
	
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *lRearWheelForwardVel.ToString());

	DrawLine(mRWheelRef->GetActorLocation(), lRearWheelForwardVel, 1, FColor::Red);

	DrawLine(mRWheelRef->GetActorLocation(), lRearWheelVelocity, 1, FColor::Green);

	FVector Force = lRearWheelForward * 4000;
	mBodyRef->AddForceAtLocation(Force, mBodyRef->GetComponentLocation());

	/*
	The basic math goes like this.
	Output Force is a function of slip velocity and normal load
	Slip velocity is a function of actual velocity * -1 + simulated velocity (rotation rate x radius)
	A good tire has about 1G of available grip so I clamp the output force to be at or under the normal load
	The G of grip is actually my most used tuning parameter. Set it to
	0.2 on ice, 1.4 on a drag strip, 0.8 on dirt etc
	That's also basically the friction coefficient.
	Real tires have load sensitivity but that's far beyond for now
	So each tire has to at least have a variable for radius, a variable for rotation rate (rad/s), and a variable for G per cm/s slip.
	In my case I have moment of inertia as well for the drivetrain Sim but you can just set the rotation rate to something like 3 for testing
	That would be basically equivalent to a tire that always spins and can never slow down, just a constant speed tire

	So if tire is moving 100cm/s in X, has angular velocity of 3 rad/s, has radius of 50 cm, and has 0.1 G per cm/s, it's slip would be:
	-100 + 150 = 50 cm/s, it's G would be 50 x 0.1 or 5G, clamp it to 1G, it's normal load would be something like BikeWeight/2, 
	so output force would be BikeWeight/2 in X direction.
	Normal load is how hard it's being pushed into the ground. Typically something like weight/2 for a bike but you can get it from the constraint.
	Just do dot product of constraint force and contact normal vector.
	As you see, as bike velocity reaches 150, it's output force would taper down to 0
	So the G max is how much grip the tire can have, and the G per cm/s is like a "stiffness"
	This is somewhat like a super simplified tire model, works great for games tho.

	TODO
	make new project thats simpler. bike is just two spheres and a body. No suspension but still have rotation constraint
	*/
}