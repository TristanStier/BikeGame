// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wheel.generated.h"

UCLASS()
class BIKEGAMEVS_API AWheel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWheel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float RotationRate = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float Radius = 78.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float GSlip = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	UStaticMesh* TireModel;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	


};
