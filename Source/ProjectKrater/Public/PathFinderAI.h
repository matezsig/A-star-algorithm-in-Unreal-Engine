// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.h"
#include "PathFinderAI.generated.h"

UCLASS(Abstract)
class PROJECTKRATER_API APathFinderAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathFinderAI();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float speed = 10.0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	AGridActor* Grid;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FIntPoint> CurrentPath;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint GridPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FIntPoint PlayerGridPosition;

	double GSCORE_UNIT = 1.0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void FindPlayer(FVector PlayerLocation);

	UFUNCTION(BlueprintCallable)
	void MoveToCell();

	double CalculateHValue(FIntPoint CurrentCoordinates, FIntPoint DestinationCoordinates);

	TArray<FIntPoint> ReconstructPath(TMap<FIntPoint, FIntPoint> PreCellMap, FIntPoint CurrentPos);

	TArray<FIntPoint> A_Star(FIntPoint StartPos, FIntPoint Goal);

	UFUNCTION(BlueprintCallable)
	void FindGridPosition();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
