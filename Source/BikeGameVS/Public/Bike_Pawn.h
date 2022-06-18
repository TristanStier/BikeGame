// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Bike_Pawn.generated.h"

class AWheel;

UCLASS()
class BIKEGAMEVS_API ABike_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABike_Pawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	AWheel* mFWheelRef = nullptr;

	UPROPERTY()
	AWheel* mRWheelRef = nullptr;

	UPROPERTY()
	UStaticMeshComponent* mBodyRef = nullptr;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APawn> mPawnToSpawn;

	bool mInAir = false;

	float mTargetRoll = 0.0f;

	FRotator mOldSteerRotation = FRotator(0, 0, 0);

	FVector mFrameVelocity;

	float mOldError = 0.0f;

	float mError = 0.0f;

	FHitResult mFWheelHit;

	FHitResult mRWheelHit;

	UPROPERTY(EditAnywhere, Category = "Stability")
	float mSpringCof = 700.0f;

	UPROPERTY(EditAnywhere, Category = "Stability")
	float mSpringDamp = 70.0f;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetReferences(AWheel* FrontWheelReference, AWheel* RearWheelReference, UStaticMeshComponent* BodyReference);

	UFUNCTION(BlueprintCallable, Category = "Runtime")
	void TraceWheels();

	UFUNCTION(BlueprintCallable, Category = "Stability")
	void AddCorrectionForce();

	UFUNCTION(BlueprintCallable, Category = "Stability")
	void ExitBike(); 

	UFUNCTION(BlueprintCallable, Category = "Stability")
	void FakeForces();

	void DrawLine(FVector Start, FVector Direction, float Length, FColor Color);
};
