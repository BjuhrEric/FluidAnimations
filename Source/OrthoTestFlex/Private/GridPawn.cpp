// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "GridPawn.h"
#include "DestructibleCubeActor.h"


// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/DestructibleCubeActor_Blueprint.DestructibleCubeActor_Blueprint'"));
	if (ItemBlueprint.Object) {
		MyItemBlueprint = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGridPawn::init()
{
	/*if (gt) {
		gt->dispose();
		free(gt);
	}
	gt = new GridTerrain(width, height);*/
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AGridPawn::spawnCubes()
{
	UWorld* const World = GetWorld();
	if (World && MyItemBlueprint) {
		
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = this;
				FVector* pos = new FVector(0, x * 100, y * 100);
				ADestructibleCubeActor* const cube = World->SpawnActor<ADestructibleCubeActor>(MyItemBlueprint, *pos, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
}

