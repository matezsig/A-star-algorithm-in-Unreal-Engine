// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinderAI.h"
#include <limits>

// Sets default values
APathFinderAI::APathFinderAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APathFinderAI::BeginPlay()
{
	Super::BeginPlay();

    PlayerGridPosition.X = -2;  // Setting a default value, which is not in the Grid; We will check the Players exact position
    PlayerGridPosition.Y = -2;
	
}

// Function to find the Grid Position of the AI
void APathFinderAI::FindGridPosition()
{
    for (FGridCell Cell : Grid->Cells)
    {
        if (FVector::DistXY(GetActorLocation(), Cell.AbsPosition) <= (sqrt(2.0) * Grid->CellSize / 2)) // vagy <=
        {
            GridPosition.X = Cell.GridPosition.X;
            GridPosition.Y = Cell.GridPosition.Y;
        }
    }
}

// A Utility Function to calculate the 'h' heuristics.
double APathFinderAI::CalculateHValue(FIntPoint CurrentCoordinates, FIntPoint DestinationCoordinates)
{
    return ((double)sqrt( pow((DestinationCoordinates.X - CurrentCoordinates.X), 2) + pow((DestinationCoordinates.Y - CurrentCoordinates.Y), 2) ));
}

TArray<FIntPoint> APathFinderAI::ReconstructPath(TMap<FIntPoint, FIntPoint> PreCellMap, FIntPoint CurrentPos)
{
    TArray<FIntPoint> TotalPath;
    TotalPath.Add(CurrentPos);
    while (PreCellMap.Contains(CurrentPos))
    {
        CurrentPos = PreCellMap[CurrentPos];
        TotalPath.Insert(CurrentPos, 0);
    }
    return TotalPath;
}


// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
TArray<FIntPoint> APathFinderAI::A_Star(FIntPoint StartPos, FIntPoint Goal)
{
    // The set of discovered nodes that may need to be (re-)expanded.
    // Initially, only the start node is known.
    // This is usually implemented as a min-heap or priority queue rather than a hash-set.
    TArray<FIntPoint> CellsToAnalyze; //openSet
    CellsToAnalyze.Add(StartPos);

    // For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start
    // to n currently known.
    TMap<FIntPoint, FIntPoint> PreCellMap; //cameFrom := an empty map

    // For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
    TMap<FIntPoint, double> GScoreMap;
    for (int x = 0; x < Grid->NumOfX; x++)
    {
        for (int y = 0; y < Grid->NumOfY; y++)
        {
            FIntPoint NewFIntPoint;
            NewFIntPoint.X = x;
            NewFIntPoint.Y = y;
            GScoreMap.Add(NewFIntPoint, std::numeric_limits<double>::max());
        }
    }
    GScoreMap[StartPos] = 0.0;

    // For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
    // how cheap a path could be from start to finish if it goes through n.
    TMap<FIntPoint, double> FScoreMap; //TODO: for: add all cells
    FScoreMap.Add(StartPos, CalculateHValue(StartPos, Goal));

    while (CellsToAnalyze.Num() > 0)
    {
        //
        //GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Yellow, FString::FromInt(1).Append(" While utan"));


        // This operation can occur in O(Log(N)) time if openSet is a min-heap or a priority queue
        FIntPoint CurrentPos;
        double MinFScore = std::numeric_limits<double>::max();
        for (FIntPoint Cell : CellsToAnalyze)
        {
            if (FScoreMap[Cell] < MinFScore )
            {
                MinFScore = FScoreMap[Cell];
                CurrentPos = Cell;
            }
        }
          // = the node in openSet having the lowest fScore[] value
        if (CurrentPos == Goal) return ReconstructPath(PreCellMap, CurrentPos);

        //log
       // GEngine->AddOnScreenDebugMessage(INDEX_NONE, 90.f, FColor::Yellow, FString::SanitizeFloat(FScoresOfCellsToAnalyze[0]).Append(" FScores Sort[0]"));
        //log
       // GEngine->AddOnScreenDebugMessage(INDEX_NONE, 90.f, FColor::Yellow, FString::FromInt(FScoreMap.FindKey(FScoresOfCellsToAnalyze[0])->X).Append(" FScoreMap.FindKey(fscorecells[0].X"));

        //
        //GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Yellow, FString::FromInt(2).Append(" Remove elott"));

        CellsToAnalyze.Remove(CurrentPos);

        TArray<FIntPoint> NeighborCells;

        if (CurrentPos.X - 1 >= 0) NeighborCells.Add(FIntPoint::FIntPoint(CurrentPos.X - 1, CurrentPos.Y));
        if (CurrentPos.Y - 1 >= 0) NeighborCells.Add(FIntPoint::FIntPoint(CurrentPos.X, CurrentPos.Y - 1));
        if (CurrentPos.X + 1 < Grid->NumOfX) NeighborCells.Add(FIntPoint::FIntPoint(CurrentPos.X + 1, CurrentPos.Y));
        if (CurrentPos.Y + 1 < Grid->NumOfY) NeighborCells.Add(FIntPoint::FIntPoint(CurrentPos.X, CurrentPos.Y + 1));

        for (FIntPoint CurrentNeighbor : NeighborCells)
        {
            // d(current,neighbor) is the weight of the edge from current to neighbor
            // tentative_gScore is the distance from start to the neighbor through current
            double TentativeGScore = GScoreMap[CurrentPos] + GSCORE_UNIT;    // GSCORE_UNIT = d(current, neighbor)
            if (TentativeGScore < GScoreMap[CurrentNeighbor])
            {
                // This path to neighbor is better than any previous one. Record it!
                PreCellMap.Add(CurrentNeighbor, CurrentPos);
                GScoreMap[CurrentNeighbor] = TentativeGScore;
                FScoreMap.Add(CurrentNeighbor, (TentativeGScore + CalculateHValue(CurrentNeighbor, Goal)));
                if ( !CellsToAnalyze.Contains(CurrentNeighbor) ) CellsToAnalyze.Add(CurrentNeighbor);
            }
        }
    }

    // Open set is empty but goal was never reached
    return TArray<FIntPoint>();
}


void APathFinderAI::FindPlayer(FVector PlayerLocation)
{
    if (Grid == nullptr) return;

    FIntPoint NewPlayerPosition = Grid->GetPlayerCellCoordinates(PlayerLocation);
    if (NewPlayerPosition == PlayerGridPosition) return; // If equals, No new A Star needed, proceed with the movement

    PlayerGridPosition = NewPlayerPosition;
    if (PlayerGridPosition == FIntPoint(-1, -1) ||
        PlayerGridPosition == GridPosition ) return;

    CurrentPath.Empty();
    CurrentPath = A_Star(GridPosition, PlayerGridPosition);
}

// Setting ActorLocation to the next Cell of Shortest Path to Player, Updating GridPosition
void APathFinderAI::MoveToCell()
{
	if (Grid == nullptr || CurrentPath.Num() < 2) return;

	FGridCell DestinationCell = Grid->GetCellByIndex(CurrentPath[1].X, CurrentPath[1].Y);
	SetActorLocation(DestinationCell.AbsPosition);
    GridPosition.X = DestinationCell.GridPosition.X;
    GridPosition.Y = DestinationCell.GridPosition.Y;

    CurrentPath.RemoveAt(0);
}

// Called every frame
void APathFinderAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

