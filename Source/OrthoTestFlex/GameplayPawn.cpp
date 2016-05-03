// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "GameplayPawn.h"
#include "liquidfun.h"

b2Vec2 gravity(0.0f, -80.0f);
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2World world(gravity);
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2Body* body;
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2ParticleSystem* particleSystem;


const char D_TERRAIN = 0, E_TERRAIN = 1, UD_TERRAIN = 2;
const int dX[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
const int dY[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
float timer = 0.0f;
// Sets default values
AGameplayPawn::AGameplayPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = GroundSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GroundSprite"));
	}
	GroupedGroundSprite = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("GroupedGroundSprite"));

	FluidSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FluidSprite"));
	GroupedFluidSprite = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("GroupedFluidSprite"));
	IDSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("IndestructibleSprite"));
	GroupedIDSprite = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("GroupedIndestrubtibleSprite"));
	BGSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BackgroundSprite"));
	BGSprite->SetRelativeLocation(FVector(0.0f, -2150.0f, 0.0f));
	BGSprite->SetWorldRotation(FRotator(0.0f, 0.0f, 90.0f));
}

// Called when the game starts or when spawned
void AGameplayPawn::BeginPlay()
{
	Super::BeginPlay();
	GroupedFluidSprite->ClearInstances();
	GroupedGroundSprite->ClearInstances();
	InitTerrain();
	InitFluids();
}

void AGameplayPawn::CleanUpLF()
{
	world.DestroyParticleSystem(particleSystem);
	particleSystem = nullptr;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			world.DestroyBody(TerrainBodies[i][j]);
		}
	}
}

// Called every frame
void AGameplayPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (LMBPressed)
	{
		FVector2D mousePos = GetMouseWorldPosition();
		int nx = WorldToGridX(mousePos.X);
		int ny = WorldToGridY(mousePos.Y);
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, FString::FromInt(nx) + FString(", ") + FString::FromInt(ny));
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, GetMouseWorldPosition().ToString());
		DestroySquare(GetMouseWorldPosition(), 5);
	}
	UpdateLF(DeltaTime);
}

// Called to bind functionality to input
void AGameplayPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Dig", IE_Pressed, this, &AGameplayPawn::SetLMBPressed);
	InputComponent->BindAction("Dig", IE_Released, this, &AGameplayPawn::SetLMBReleased);

}

void AGameplayPawn::InitFluids()
{
	b2PolygonShape box;
	box.SetAsBox(25.0f, 25.0f);
	b2ParticleSystemDef partSysDef;
	partSysDef.radius = 0.8f;
	particleSystem = world.CreateParticleSystem(&partSysDef);
	b2ParticleGroupDef partGrDef;
	partGrDef.position.Set(0.0f, 30.0f);
	partGrDef.shape = &box;
	particleSystem->CreateParticleGroup(partGrDef);
}

void AGameplayPawn::InitTerrain()
{
	b2PolygonShape box;
	box.SetAsBox(spacing / 1.9f, spacing / 1.9f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(GridToWorldX(i), GridToWorldY(j));
			body = world.CreateBody(&bodyDef);
			body->CreateFixture(&fixtureDef);
			body->SetActive(true);
			FVector2D CurrentCoord(i, j);
			TerrainBodies[i][j] = body;
		}
	}
	DestroySquare(FVector2D(0, 30), 25);
}

void AGameplayPawn::UpdateLF(float DeltaTime)
{
	world.Step(DeltaTime, 8, 3);
	b2Vec2* vs = particleSystem->GetPositionBuffer();
	GroupedFluidSprite->ClearInstances();
	for (int i = 0; i < particleSystem->GetParticleCount(); i++)
	{
		GroupedFluidSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(vs[i].x*SCALE_FACTOR, vs[i].y*SCALE_FACTOR, -5.0f), FVector(1.0f, 1.0f, 1.0f)),
			FluidSprite->GetSprite(), true, FColor::White);
	}
}

FVector2D AGameplayPawn::GetMouseWorldPosition()
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
		return FVector2D((int)(WorldPos.X/SCALE_FACTOR), (int)(WorldPos.Y/SCALE_FACTOR));
	}
	return FVector2D::ZeroVector;
}

void AGameplayPawn::SetLMBPressed()
{
	LMBPressed = true;
}

void AGameplayPawn::SetLMBReleased()
{
	LMBPressed = false;
}

void AGameplayPawn::DestroySquare(FVector2D worldCoord, float r)
{
	int nx = WorldToGridX(worldCoord.X);
	int ny = WorldToGridY(worldCoord.Y);
	GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, FString::FromInt(nx) + FString(", ") + FString::FromInt(ny));
	int nr = r / spacing;
	for (int i = nx - nr; i <= nx + nr; i++) {
		for (int j = ny - nr; j <= ny + nr; j++) {
			if (i >= 0 && i < width && j >= 0 && j < height)
				RemoveAtIndex(i, j);
		}
	}
	nr++;
	for (int i = nx - nr; i <= nx + nr; i++) {
		for (int j = ny - nr; j <= ny + nr; j++) {
			if (i >= 0 && i < width && j >= 0 && j < height)
				UpdatePos(i, j);
		}
	}
}

int AGameplayPawn::WorldToGridX(int x)
{
	return (x - gridX) / spacing;
}

int AGameplayPawn::WorldToGridY(int y)
{
	return (y - gridY) / spacing;
}

float AGameplayPawn::GridToWorldX(int x)
{
	return x * spacing +gridX;
}

float AGameplayPawn::GridToWorldY(int y)
{
	return y * spacing +gridY;
}

void AGameplayPawn::UpdatePos(int x, int y)
{
	FVector2D coord(x,y);
	bool isInBoxSet = grid[x][y] != E_TERRAIN;
	if (grid[x][y] != E_TERRAIN) {
		int sumAdjacent = 0;
		for (int i = 0; i < 8; i++) {
			if (grid[x + dX[i]][y + dY[i]] != E_TERRAIN)
				sumAdjacent++;
		}
		if (sumAdjacent == 8) {
			if (isInBoxSet) {
				DestroyBody(x,y);
			}
		}
		else {
			if (!isInBoxSet) {
				CreateBody(x,y);
			}
		}
	}
	else if (isInBoxSet) {
		DestroyBody(x,y);
	}
}

void AGameplayPawn::DestroyBody(int x, int y)
{
	if (boxSet.Find(FVector2D(x, y)) != NULL)
		boxSet.Remove(FVector2D(x, y));
	TerrainBodies[x][y]->SetActive(false);
}

void AGameplayPawn::CreateBody(int x, int y)
{
	if (boxSet.Find(FVector2D(x, y)) == NULL)
		boxSet.Add(FVector2D(x, y));
	TerrainBodies[x][y]->SetActive(true);
}

void AGameplayPawn::RemoveAtIndex(int x, int y)
{
	if (grid[x][y] == D_TERRAIN)
	{
		AddSpriteAndUpdate(x, y);
		grid[x][y] = E_TERRAIN;
		TerrainBodies[x][y]->SetActive(false);
	}
}

void AGameplayPawn::AddSpriteAndUpdate(int x, int y)
{
	if (grid[x][y] == D_TERRAIN)
	{
		GroupedGroundSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(GridToWorldX(x)*SCALE_FACTOR, GridToWorldY(y)*SCALE_FACTOR, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
			GroundSprite->GetSprite(), true, FColor::White);
		TerrainBodies[x][y]->SetActive(false);
	}
}

void AGameplayPawn::MakeIndestructible(int x, int y)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		grid[x][y] = UD_TERRAIN;
		GroupedIDSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(GridToWorldX(x)*SCALE_FACTOR, GridToWorldY(y)*SCALE_FACTOR, -10.0f), FVector(1.0f, 1.0f, 1.0f)),
			IDSprite->GetSprite(), true, FColor::White);
		TerrainBodies[x][y]->SetActive(true);
	}
}