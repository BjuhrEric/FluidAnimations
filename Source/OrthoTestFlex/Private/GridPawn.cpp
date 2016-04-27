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
	if (gt) {
		gt->dispose();
		free(gt);
	}
	gt = new GridTerrain(width, height);
	UE_LOG(LogTemp, Warning, TEXT("GridPawn Init Complete"));
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UWorld* World = GetWorld();
	if (World && mouseDown) {
		/*
		 * Do stuff here
		 */
		FVector2D MousePos = GetMouseWorldPosition();
		int GridX = MousePos.X;
		int GridY = MousePos.Y;
		
		UE_LOG(LogTemp, Warning, TEXT("(%d, %d)"), GridX, GridY);
		if (GridX >= 0 && GridX < width && GridY >= 0 && GridY < height)
		{
			int64 ptr = gt->getCell(GridX, GridY);
			if (ptr)
			{
				ADestructibleCubeActor* Cube = (ADestructibleCubeActor*)ptr;
				Cube->Destroy();
				gt->setCell(GridX, GridY, NULL);
			}
		}
	}
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Dig", IE_Pressed, this, &AGridPawn::OnClick);
	InputComponent->BindAction("Dig", IE_Released, this, &AGridPawn::OnRelease);
	UE_LOG(LogTemp, Warning, TEXT("Setup input"));
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
				gt->setCell((int32)x, (int32)y, (int64) cube);
			}
		}
	}
}

void AGridPawn::OnClick() {
	mouseDown = true;
	UE_LOG(LogTemp, Warning, TEXT("MouseDown"));
}

void AGridPawn::OnRelease() {
	mouseDown = false;
	UE_LOG(LogTemp, Warning, TEXT("!MouseDown"));
}

FVector2D AGridPawn::GetMouseWorldPosition()
{
	UWorld* TestWorld = GetWorld();
	if (TestWorld)
	{
		APlayerController* PlayerController = TestWorld->GetFirstPlayerController();
		FVector2D MousePos = FVector2D(0, 0);
		FVector WorldPos = FVector(MousePos.X, MousePos.Y, 0);
		FVector Dir = FVector(0, 0, 0);
		if (PlayerController != nullptr)
		{
			PlayerController->GetMousePosition(MousePos.X, MousePos.Y);
			PlayerController->DeprojectMousePositionToWorld(WorldPos, Dir);
		}
		return FVector2D((int)(WorldPos.Y / SCALE_FACTOR), (int)(WorldPos.Z / SCALE_FACTOR));
	}
	return FVector2D::ZeroVector;
}