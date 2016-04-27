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
		FVector2D MousePos = GetMouseWorldPosition();
		int GridX = round(MousePos.X);
		int GridY = round(MousePos.Y);
		int MaxX = fmax(X, GridX);
		int MinX = fmin(X, GridX);
		int MaxY = fmax(Y, GridY);
		int MinY = fmin(Y, GridY);
		
		
		if (X >= 0 && X < width && Y >= 0 && Y < height) {
			for (int x = MinX - 1; x < MaxX + 1; x++) {
				for (int y = MinY - 1; y < MaxY + 1; y++) {
					DestroyCube(x, y);
				}
			}
		}
		X = GridX;
		Y = GridY;
	}
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Dig", IE_Pressed, this, &AGridPawn::OnClick);
	InputComponent->BindAction("Dig", IE_Released, this, &AGridPawn::OnRelease);
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
}

void AGridPawn::OnRelease() {
	mouseDown = false;
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

void AGridPawn::DestroyTerrainLine(int x1, int y1, int x2, int y2, int l) {
	int xl = l;
	if (x1 > x2)
		xl = -xl;
	if (y1 < y2)
		xl = -xl;
	
	int maxX = fmax(x1, x2) + l;
	int maxY = fmax(x1, x2) + l;
	int minX = fmin(x1, x2) - l;
	int minY = fmin(x1, x2) - l;
	if (x1 == x2)
		x1++;
	float K = ((y2 - y1) / (float)(x2 - x1));
	for (int i = minX; i <= maxX; i++) {
		int minLine = (int)(y1 + K * (i - x1 + xl)) - l;
		int maxLine = (int)(y1 + K * (i - x1 - xl)) + l;
		for (int j = fmax(minY, minLine); j <= fmin(maxY, maxLine); j++) {
			DestroyCube(i, j);
		}
	}
}

void AGridPawn::DestroyCube(int x, int y) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		int64 ptr = gt->getCell(x, y);
		if (ptr)
		{
			ADestructibleCubeActor* Cube = (ADestructibleCubeActor*)ptr;
			Cube->Destroy();
			gt->setCell(x, y, NULL);
		}
	}
}