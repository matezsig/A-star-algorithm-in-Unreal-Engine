// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyAI.generated.h"

UCLASS()
class PROJECTKRATER_API AMyAI : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;


	FVector GetNextPathPoint();

	// Next point in navigation path
	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "MyAI")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "MyAI")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "MyAI")
	float RequiredDistanceToTarget;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

};
