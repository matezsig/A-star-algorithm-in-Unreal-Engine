// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

USTRUCT(BlueprintType)
struct FGridCell
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector AbsPosition;
	UPROPERTY(BlueprintReadWrite)
	FIntPoint GridPosition;
	UPROPERTY(BlueprintReadWrite)
	bool bIsObstacle = false;
	UPROPERTY(BlueprintReadWrite)
	bool bContainsPlayer = false;
};


UCLASS(Abstract)
class PROJECTKRATER_API AGridActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridActor();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CellSize = 100.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumOfX = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 NumOfY = 10;

	TArray<FGridCell> Cells;

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGridCell GetCellByIndex(int32 XCoor, int32 YCoor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntPoint GetPlayerCellCoordinates(FVector PlayerLocation);
};
