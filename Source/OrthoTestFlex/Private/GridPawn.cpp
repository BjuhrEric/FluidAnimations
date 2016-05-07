// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "GridPawn.h"
#include "DestructibleCubeActor.h"
#include "Point.h"

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
	if (DestroyedCubes) {
		for (auto CubeIt = DestroyedCubes->CreateConstIterator(); CubeIt; ++CubeIt) {
			ADestructibleCubeActor* Cube = (ADestructibleCubeActor*) *CubeIt;
			Cube->Destroy();
		}
		free(DestroyedCubes);
	}
	gt = new GridTerrain(width, height);
	DestroyedCubes = new TArray<int64>();
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
		
		if (X >= 0 && X < width && Y >= 0 && Y < height) {

			TArray<int64>* line;
			if (GridX > X)
			{
				line = Line(X, Y, GridX, GridY);
			}
			else
			{
				line = Line(GridX, GridY, X, Y);
			}

			if (line)
			{
				int n = line->Num();
				UE_LOG(LogTemp, Warning, TEXT("Line size = %d"), n);
				for (int i = 0; i < n; i++)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cube Destroyed!"));
					Point* p = (Point*)line->operator[](i);
					DestroySquare(p->x, p->y, 3);
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
	X = -1;
	Y = -1;
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

void AGridPawn::DestroyCube(int x, int y) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		int64 ptr = gt->getCell(x, y);
		if (ptr)
		{
			ADestructibleCubeActor* Cube = (ADestructibleCubeActor*)ptr;
			Cube->GetRootComponent()->SetVisibility(false, true);
			Cube->DisableComponentsSimulatePhysics();
			gt->setCell(x, y, NULL);
			DestroyedCubes->Add((int64)Cube);
		}
	}
}

TArray<int64>* AGridPawn::Line(int32 x1, int32 y1, int32 x2, int32 y2)
{
	TArray<int64>* line = new TArray<int64>();
	if (x1 == x2 && y1 == y2) {
		Point* p = new Point();
		p->x = x2;
		p->y = y2;
		line->Add((int64)p);
		return line;
	}
	if (x1 == x2) return VerticalLine(x1, y1, y2);
	if (y1 == y2) return HorisontalLine(y1, x1, x2);

	int delta_x(x2 - x1);
	// if x1 == x2, then it does not matter what we set here
	signed char const ix((delta_x > 0) - (delta_x < 0));
	delta_x = std::abs(delta_x) << 1;

	int delta_y(y2 - y1);
	// if y1 == y2, then it does not matter what we set here
	signed char const iy((delta_y > 0) - (delta_y < 0));
	delta_y = std::abs(delta_y) << 1;

	Point* p = new Point();
	p->x = x1;
	p->y = y1;
	line->Add((int64)p);

	if (delta_x >= delta_y)
	{
		// error may go below zero
		int error(delta_y - (delta_x >> 1));

		while (x1 != x2)
		{
			if ((error >= 0) && (error || (ix > 0)))
			{
				error -= delta_x;
				y1 += iy;
			}
			// else do nothing

			error += delta_y;
			x1 += ix;
			Point* p = new Point();
			p->x = x1;
			p->y = y1;
			line->Add((int64)p);
		}
	}
	else
	{
		// error may go below zero
		int error(delta_x - (delta_y >> 1));

		while (y1 != y2)
		{
			if ((error >= 0) && (error || (iy > 0)))
			{
				error -= delta_y;
				x1 += ix;
			}
			// else do nothing

			error += delta_x;
			y1 += iy;
			Point* p = new Point();
			p->x = x1;
			p->y = y1;
			line->Add((int64)p);
		}
	}

	
	return line;
}

TArray<int64>* AGridPawn::VerticalLine(int32 x, int32 y0, int32 y1)
{
	UE_LOG(LogTemp, Warning, TEXT("Vertical line, y0:%d, y1:%d"), y0, y1);
	if (y0 > y1) return VerticalLine(x, y1, y0);
	TArray<int64>* line = new TArray<int64>();
	for (int32 y = y0; y <= y1; y++)
	{
		Point* p = new Point();
		p->x = x;
		p->y = y;
		line->Add((int64) p);
	}
	return line;
}

TArray<int64>* AGridPawn::HorisontalLine(int32 y, int32 x0, int32 x1)
{
	UE_LOG(LogTemp, Warning, TEXT("Horisontal line"));
	if (x0 > x1) return HorisontalLine(y, x1, x0);
	TArray<int64>* line = new TArray<int64>();
	for (int32 x = x0; x <= x1; x++)
	{
		Point* p = new Point();
		p->x = x;
		p->y = y;
		line->Add((int64)p);
	}
	return line;
}

void AGridPawn::DestroySquare(int x, int y, int r) {
	for (int i = x - r; i <= x + r; i++) {
		for (int j = y - r; j <= y + r; j++) {
			DestroyCube(i, j);
		}
	}
}