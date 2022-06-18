// Fill out your copyright notice in the Description page of Project Settings.


#include "Bike_Pawn.h"
#include "GameFramework/Actor.h" 
#include "Kismet/GameplayStatics.h" 
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/BoxComponent.h" 
#include "ThirdPersonCharacter.h"

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AThirdPersonCharacter::SetReferences(UBoxComponent* BoxComponentReference)
{
	mBoxComponentRef = BoxComponentReference;
}

void AThirdPersonCharacter::EnterBike()
{
	TArray<AActor*> lOverlappingActors;
	// mBoxComponentRef->GetOverlappingActors(lOverlappingActors, TSubclassOf<ABike_Pawn>());
	mBoxComponentRef->GetOverlappingActors(lOverlappingActors, ABike_Pawn::StaticClass());

	for (int i = 0; i<lOverlappingActors.Num(); i++)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%s"), *lOverlappingActors[i]->GetName());

		APawn* lPawnToPosses = Cast<APawn>(lOverlappingActors[i]);

		if (lPawnToPosses != nullptr)
		{
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(lPawnToPosses);
			Destroy();
		}
	}
}