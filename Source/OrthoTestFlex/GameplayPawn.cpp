// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "GameplayPawn.h"
#include "liquidfun.h"

UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2Vec2 gravity(0.0f, -80.0f);
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2World world(gravity);
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2Body* body;
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2ParticleSystem* particleSystem;
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2ParticleDef particleDef;

const int NumFloaters = 5;
UPROPERTY(Category = "LF", meta = (AllowPrivateAccess = "true"))
b2Body* FloatingBody[NumFloaters];


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
	FloatingSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FloatingSprite"));
	GroupedFloatingSprite = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("GroupedFloatingSprite"));
	ScoreSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ScoreSprite"));
	ScoreGroupedSprite = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("ScoreGroupedSprite"));
}

// Called when the game starts or when spawned
void AGameplayPawn::BeginPlay()
{
	Super::BeginPlay();
	InitTerrain();
	InitFluids();
	//InitIndestructible();
	InitFloating();
	InitWinning();
}

void AGameplayPawn::CleanUpLF()
{
	if (particleSystem != nullptr)
	{
		world.DestroyParticleSystem(particleSystem);
		particleSystem = nullptr;
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (TerrainBodies[i][j] != nullptr)
			{
				world.DestroyBody(TerrainBodies[i][j]);
				TerrainBodies[i][j] = nullptr;
			}
		}
	}
	for (int i = 0; i < NumFloaters; i++)
	{
		if (FloatingBody[i] != nullptr)
		{
			world.DestroyBody(FloatingBody[i]);
			FloatingBody[i] = nullptr;
		}
	}
}

// Called every frame
void AGameplayPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//SpawnParticle();
	GroupedFloatingSprite->ClearInstances();
	if (LMBPressed)
	{
		FVector2D mousePos = GetMouseWorldPosition();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, FString::FromInt(nx) + FString(", ") + FString::FromInt(ny));
		//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Green, GetMouseWorldPosition().ToString());
		DestroyTerrain(mousePos);
	}
	else
		PrevMouseX = -32767;
	UpdateLF(DeltaTime);
	for (int i = 0; i < NumFloaters; i++)
	{
		GroupedFloatingSprite->AddInstance(FTransform(FRotator(0.0f, FMath::RadiansToDegrees(FloatingBody[i]->GetAngle()), 90.0f), FVector(FloatingBody[i]->GetPosition().x*SCALE_FACTOR, 
			FloatingBody[i]->GetPosition().y*SCALE_FACTOR, -7.0f), FVector(1.0f, 1.0f, 1.0f)), FloatingSprite->GetSprite(), true, FColor::White);
	}
	UpdateWinCheck();
	//GEngine->AddOnScreenDebugMessage(0, 1.0, FColor::Green, FString::FromInt(particleSystem->GetParticleCount()));
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
	//partSysDef.radius = 0.8f;
	//partSysDef.density = 1.0;
	particleSystem = world.CreateParticleSystem(&partSysDef);
	b2ParticleGroupDef partGrDef;
	partGrDef.position.Set(0.0f, 30.0f);
	partGrDef.shape = &box;
	particleSystem->CreateParticleGroup(partGrDef);
}

/*
void AGameplayPawn::InitFluids()
{
	particleDef.position.Set(FMath::FRandRange(-0.5f, 0.5f), -15);
	b2ParticleSystemDef partSysDef;
	particleSystem = world.CreateParticleSystem(&partSysDef);
}

void AGameplayPawn::SpawnParticle()
{
	if (currParticle < numParticles)
	{
		particleIndices[currParticle] = particleSystem->CreateParticle(particleDef);
		currParticle++;
	}
}*/

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
			body->SetActive(false);
			FVector2D CurrentCoord(i, j);
			TerrainBodies[i][j] = body;
		}
	}
	DestroySquare(WorldToGridX(0), WorldToGridY(30), 25);
}

void AGameplayPawn::InitFloating()
{
	int ScoreHeight = SCALE_FACTOR*GridToWorldY(0) - 100;

	b2CircleShape circle;
	circle.m_radius = 1.0f;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = 0.3f;
	fixtureDef.friction = 0.3f;
	for (int i = 0; i < NumFloaters; i++)
	{
		int x = (int)FMath::RandRange(3, width - 5);
		int y = (int)FMath::RandRange(WinHeight + 10, 500);
		DestroySquare(x, y, 3);
		b2BodyDef bDef;
		bDef.type = b2_dynamicBody;
		bDef.position.Set(GridToWorldX(x), GridToWorldY(y));
		bDef.fixedRotation = false;
		FloatingBody[i] = world.CreateBody(&bDef);
		FloatingBody[i]->CreateFixture(&fixtureDef);

		ScoreGroupedSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(SCALE_FACTOR*GridToWorldX(width/(NumFloaters*2) + i*width/NumFloaters), ScoreHeight, -5.0f), FVector(1.0f, 1.0f, 1.0f)),
			ScoreSprite->GetSprite(), true, FColor::White);
	}
}

void AGameplayPawn::InitWinning()
{
	for (int i = 0; i < width; i++)
	{
		MakeIndestructible(i, 0);
		DestroySquare(i, 15, 15);
	}
	for (int i = 1; i < WinHeight; i++)
	{
		MakeIndestructible(0, i);
		MakeIndestructible(width - 1, i);
	}
}

void AGameplayPawn::InitIndestructible()
{
	for (int i = 0; i < 3; i++)
	{
		for (int x = 30 + i * 10; x < 40 + i * 10; x++)
		{
			for (int y = 400 - i * 10; y < 410 - i * 10; y++)
			{
				MakeIndestructible(x, y);
			}
		}
	}
}

void AGameplayPawn::UpdateWinCheck()
{
	numInGoal = 0;
	int ScoreHeight = SCALE_FACTOR*GridToWorldY(0) - 100;
	for (int i = 0; i < NumFloaters; i++)
	{
		b2Vec2 pos = FloatingBody[i]->GetPosition();
		if (WorldToGridX(pos.x) >= 0 && WorldToGridX(pos.x) < width - 1 && WorldToGridY(pos.y) >= 0 && WorldToGridY(pos.y) < WinHeight)
			numInGoal++;
	}
	for (int i = 0; i < numInGoal; i++)
	{
		GroupedFloatingSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(SCALE_FACTOR*GridToWorldX(width / (NumFloaters * 2) + i*width / NumFloaters), ScoreHeight, -5.0f), FVector(1.0f, 1.0f, 1.0f)),
			FloatingSprite->GetSprite(), true, FColor::White);
	}
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

void AGameplayPawn::DestroySquare(int nx, int ny, int nr)
{
	for (int i = nx - nr; i <= nx + nr; i++) 
	{
		for (int j = ny - nr; j <= ny + nr; j++)
		{
			if (i >= 0 && i < width && j >= 0 && j < height)
				if (grid[i][j] != UD_TERRAIN)
					RemoveAtIndex(i, j);
		} 
	}
	nr++;
	for (int i = nx - nr; i <= nx + nr; i++)
	{
		for (int j = ny - nr; j <= ny + nr; j++) 
		{
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
	if (grid[x][y] != E_TERRAIN) 
	{
		int sumAdjacent = 0;
		for (int i = 0; i < 8; i++) 
		{
			if (grid[x + dX[i]][y + dY[i]] != E_TERRAIN)
				sumAdjacent++;
		}
		if (sumAdjacent == 8)
		{
			DestroyBody(x,y);
		}
		else 
		{
			CreateBody(x,y);
		}
	}
	else 
	{
		DestroyBody(x,y);
	}
}

void AGameplayPawn::DestroyBody(int x, int y)
{
	if (boxSet.Find(FVector2D(x, y)) != NULL)
	{
		boxSet.Remove(FVector2D(x, y));
		TerrainBodies[x][y]->SetActive(false);
	}
}

void AGameplayPawn::CreateBody(int x, int y)
{
	if (boxSet.Find(FVector2D(x, y)) == NULL)
	{
		boxSet.Add(FVector2D(x, y));
		TerrainBodies[x][y]->SetActive(true);
	}
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
	GroupedGroundSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(GridToWorldX(x)*SCALE_FACTOR, GridToWorldY(y)*SCALE_FACTOR, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
		GroundSprite->GetSprite(), true, FColor::White);
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

float AGameplayPawn::GetCameraStop()
{
	return (float)SCALE_FACTOR*GridToWorldY(0);
}

int32 AGameplayPawn::GetScore()
{
	return numInGoal;
}

void AGameplayPawn::DestroyLine(int x1, int y1, int x2, int y2, int l)
{
	int xl = l;
	if (x1 > x2)
		xl = -xl;
	if (y1 < y2)
		xl = -xl;
	int maxX = Max(x1, x2) + l;
	int maxY = Max(y1, y2) + l;
	int minX = Min(x1, x2) - l;
	int minY = Min(y1, y2) - l;
	if (x1 == x2)
		x1++;
	float K = ((y2 - y1) / (float)(x2 - x1));
	for (int i = minX; i <= maxX; i++) 
	{
		int minLine = (int)(y1 + K * (i - x1 + xl)) - l;
		int maxLine = (int)(y1 + K * (i - x1 - xl)) + l;
		for (int j = Max(minY, minLine); j <= Min(maxY, maxLine); j++) 
		{
			if (i >= 0 && i < width && j >= 0 && j < height)
				RemoveAtIndex(i, j);
		}
	}
	l++;
	UpdateLine(x1, y1, x2, y2, l);
}

void AGameplayPawn::UpdateLine(int x1, int y1, int x2, int y2, int l)
{
	int xl = l;
	if (x1 > x2)
		xl = -xl;
	if (y1 < y2)
		xl = -xl;
	int maxX = Max(x1, x2) + l;
	int maxY = Max(y1, y2) + l;
	int minX = Min(x1, x2) - l;
	int minY = Min(y1, y2) - l;
	if (x1 == x2)
		x1++;
	float K = ((y2 - y1) / (float)(x2 - x1));
	for (int i = minX; i <= maxX; i++) 
	{
		int minLine = (int)(y1 + K * (i - x1 + xl)) - l;
		int maxLine = (int)(y1 + K * (i - x1 - xl)) + l;
		for (int j = Max(minY, minLine); j <= Min(maxY, maxLine); j++) 
		{
			if (i >= 0 && i < width && j >= 0 && j < height)
				UpdatePos(i, j);
		}
	}
}

int AGameplayPawn::Max(int x1, int x2)
{
	if (x1 > x2)
		return x1;
	return x2;
}

int AGameplayPawn::Min(int x1, int x2)
{
	if (x1 < x2)
		return x1;
	return x2;
}

void AGameplayPawn::DestroyTerrain(FVector2D mousePos)
{
	int x = WorldToGridX(mousePos.X);
	int y = WorldToGridY(mousePos.Y);
	if (PrevMouseX == -32767) 
	{
		DestroySquare(x, y, 5);
	}
	else 
	{
		DestroyLine(PrevMouseX, PrevMouseY, x, y, 5);
	}
	PrevMouseX = x;
	PrevMouseY = y;
}