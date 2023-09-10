// Fill out your copyright notice in the Description page of Project Settings.

#include "GridActor.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGridActor::AGridActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGridActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	for (int x = 0; x < NumOfX; x++)
	{
		for (int y = 0; y < NumOfY; y++)
		{
			FVector Pos;
			Pos.X = (x * CellSize) + GetActorLocation().X - (NumOfX * CellSize / 2) + (CellSize / 2);	//CellPostion + ActorPostion - Half of the Actor for adjustment + Half the Cellsize for being the center
			Pos.Y = (y * CellSize) + GetActorLocation().Y - (NumOfY * CellSize / 2) + (CellSize / 2);
			Pos.Z = GetActorLocation().Z;
			DrawDebugSphere(GetWorld(), Pos, 20.0, 10, FColor::Yellow, false, 30, 0, 1.5);

			FGridCell Cell;
			Cell.GridPosition.X = x;
			Cell.GridPosition.Y = y;
			Cell.AbsPosition = Pos;
			Cells.Add(Cell);
		}
	}
}

// Called when the game starts or when spawned
void AGridActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FGridCell AGridActor::GetCellByIndex(int32 XCoor, int32 YCoor)
{
	if (0 <= XCoor && XCoor < NumOfX && 0 <= YCoor && YCoor < NumOfY && (XCoor * NumOfY + YCoor) < Cells.Num() ) return Cells[XCoor * NumOfY + YCoor];
	
	return FGridCell();
}

FIntPoint AGridActor::GetPlayerCellCoordinates(FVector PlayerLocation)
{
	FIntPoint PlayerCellCoordinates = FIntPoint(-1, -1);
	float minimumDistanceFromPlayer = FVector::DistXY(PlayerLocation, FVector::ZeroVector);
	for (FGridCell Cell : Cells)
	{
		float cellDistanceFromPlayer = FVector::DistXY(PlayerLocation, Cell.AbsPosition);
		if (cellDistanceFromPlayer < minimumDistanceFromPlayer && cellDistanceFromPlayer < (sqrt(2.0) * CellSize / 2))
		{
			minimumDistanceFromPlayer = cellDistanceFromPlayer;
			PlayerCellCoordinates.X = Cell.GridPosition.X;
			PlayerCellCoordinates.Y = Cell.GridPosition.Y;
		}
	}

	return PlayerCellCoordinates;
}
