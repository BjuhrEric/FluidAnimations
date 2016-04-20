// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "PaperSpriteComponent.h"
#include "PaperGroupedSpriteComponent.h"
#include "GameplayPawn.h"
#include "liquidfun.h"

b2Vec2 gravity(0.0f, -80.0f);
b2World world(gravity);
b2Body* body;
b2ParticleSystem* particleSystem;
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
}

// Called when the game starts or when spawned
void AGameplayPawn::BeginPlay()
{
	Super::BeginPlay();
	InitFluids();
	InitTerrain();
}

// Called when the game ends
void AGameplayPawn::BeginDestroy()
{
	Super::BeginDestroy();
	LFCleanUp();
}

// Called every frame
void AGameplayPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	UpdateLF(DeltaTime);
}

// Called to bind functionality to input
void AGameplayPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

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
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (i < 20 || i > 80)
			{
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set((float)i - 48, (float)-j);
				body = world.CreateBody(&bodyDef);
				b2PolygonShape box;
				box.SetAsBox(0.5f, 0.5f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &box;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.3f;
				body->CreateFixture(&fixtureDef);
				body = world.CreateBody(&bodyDef);
				FVector2D CurrentCoord(i, j);
				TerrainBodies.Add(CurrentCoord,body);
				int32 SpriteIndex = GroupedGroundSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(i*SCALE_FACTOR - 480, -j*SCALE_FACTOR, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
					GroundSprite->GetSprite(), true, FColor::White);
				TerrainInstanceIndices.Add(CurrentCoord, SpriteIndex);
			}
		}
	}
	for (int i = 0; i < 100; i++)
	{
		for (int j = 20; j < 100; j++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set((float)i - 48, (float)-j);
			body = world.CreateBody(&bodyDef);
			b2PolygonShape box;
			box.SetAsBox(0.5f, 0.5f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &box;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			body->CreateFixture(&fixtureDef);
			body = world.CreateBody(&bodyDef);
			TerrainBodies.Add(FVector2D(i, j), body);
			FVector2D CurrentCoord(i, j);
			TerrainBodies.Add(CurrentCoord, body);
			int32 SpriteIndex = GroupedGroundSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(i*SCALE_FACTOR - 480, -j*SCALE_FACTOR, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
				GroundSprite->GetSprite(), true, FColor::White);
			TerrainInstanceIndices.Add(CurrentCoord, SpriteIndex);
		}
	}
}

void AGameplayPawn::LFCleanUp()
{
	world.DestroyParticleSystem(particleSystem);
	b2Body* bodies = world.GetBodyList();
	while (bodies != NULL)
	{
		world.DestroyBody(bodies);
		bodies = bodies->GetNext();
	}
}

void AGameplayPawn::UpdateLF(float DeltaTime)
{
	world.Step(DeltaTime, 8, 3);
	b2Vec2* vs = particleSystem->GetPositionBuffer();
	GroupedFluidSprite->ClearInstances();
	for (int i = 0; i < particleSystem->GetParticleCount(); i++)
	{
		GroupedFluidSprite->AddInstance(FTransform(FRotator(0.0f, 0.0f, 90.0f), FVector(vs[i].x*SCALE_FACTOR, vs[i].y*SCALE_FACTOR, 0.0f), FVector(1.0f, 1.0f, 1.0f)),
			FluidSprite->GetSprite(), true, FColor::White);
	}
}