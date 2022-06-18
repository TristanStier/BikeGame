// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

class UBoxComponent;

UCLASS()
class BIKEGAMEVS_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UBoxComponent* mBoxComponentRef = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void SetReferences(UBoxComponent* BoxComponentReference);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void EnterBike();
};
