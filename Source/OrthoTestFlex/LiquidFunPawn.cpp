// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "PaperSpriteComponent.h"
#include "LiquidFunPawn.h"
#include "liquidfun.h"
#include "ParticleActor.h"
#include "TerrainActor.h"

b2Vec2 gravity(0.0f, -80.0f);
b2World world(gravity);
b2Body* body;
b2ParticleSystem* particleSystem;
// Sets default values
ALiquidFunPawn::ALiquidFunPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = BoxSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BoxSprite"));
	}

	FluidSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("FluidSprite"));
	GroundSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("GroundSprite"));
}

// Called when the game starts or when spawned
void ALiquidFunPawn::BeginPlay()
{
	Super::BeginPlay();
	LFInitialize();
}

// Called every frame
void ALiquidFunPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	LFUpdate(DeltaTime);
}

// Called to bind functionality to input
void ALiquidFunPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ALiquidFunPawn::LFInitialize()
{
	/*
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0.0f, 0.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape box;
	box.SetAsBox(5.0f, 5.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &box;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	*/
	InitTerrain();
	InitFluids();
}

void ALiquidFunPawn::LFUpdate(float DeltaTime)
{
	world.Step(DeltaTime, 8, 3);
	//SetActorLocation(GetBoxPosition());
	b2Vec2* vs = particleSystem->GetPositionBuffer();
	for (int i = 0; i < ParticleArray.Num(); i++)
	{
		ParticleArray[i]->SetLocation(vs[i].x*SCALE_FACTOR, vs[i].y*SCALE_FACTOR);
	}
}

FVector ALiquidFunPawn::GetBoxPosition()
{
	return FVector(body->GetPosition().x, body->GetPosition().y, 0.0f);
}

float ALiquidFunPawn::GetBoxAngle()
{
	return body->GetAngle();
}

void ALiquidFunPawn::InitFluids()
{
	b2PolygonShape box;
	box.SetAsBox(10.0f, 10.0f);
	b2ParticleSystemDef partSysDef;
	partSysDef.radius = 0.4f;
	particleSystem = world.CreateParticleSystem(&partSysDef);
	b2ParticleGroupDef partGrDef;
	partGrDef.position.Set(0.0f, 30.0f);
	partGrDef.shape = &box;
	particleSystem->CreateParticleGroup(partGrDef);
	b2Vec2* vs = particleSystem->GetPositionBuffer();
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < particleSystem->GetParticleCount(); i++)
	{
		AParticleActor* tmp = GetWorld()->SpawnActor<AParticleActor>(FVector(vs[i].x*SCALE_FACTOR, vs[i].y*SCALE_FACTOR, 0.0f), FRotator(0.0f, 0.0f, 90.0f), spawnParam);
		tmp->SetSprite(FluidSprite->GetSprite());
		ParticleArray.Add(tmp);
	}
}

void ALiquidFunPawn::InitTerrain()
{
	FActorSpawnParameters spawnParam;
	spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (i < 20 || i > 80)
			{
				b2BodyDef bodyDef;
				bodyDef.type = b2_staticBody;
				bodyDef.position.Set((float)i - 49, (float)-j);
				body = world.CreateBody(&bodyDef);
				b2PolygonShape box;
				box.SetAsBox(0.5f, 0.5f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &box;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.3f;
				body->CreateFixture(&fixtureDef);
				body = world.CreateBody(&bodyDef);
				ATerrainActor* tmp = GetWorld()->SpawnActor<ATerrainActor>(FVector(i*SCALE_FACTOR - 480, -j*SCALE_FACTOR, 0.0f), FRotator(0.0f, 0.0f, 90.0f), spawnParam);
				tmp->SetSprite(GroundSprite->GetSprite());
				TerrainArray.Add(tmp);
			}
		}
	}
	for (int i = 0; i < 100; i++)
	{
		for (int j = 20; j < 100; j++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set((float)i - 49, (float)-j);
			body = world.CreateBody(&bodyDef);
			b2PolygonShape box;
			box.SetAsBox(0.5f, 0.5f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &box;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			body->CreateFixture(&fixtureDef);
			body = world.CreateBody(&bodyDef);
			ATerrainActor* tmp = GetWorld()->SpawnActor<ATerrainActor>(FVector(i*SCALE_FACTOR - 480, -j*SCALE_FACTOR, 0.0f), FRotator(0.0f, 0.0f, 90.0f), spawnParam);
			tmp->SetSprite(GroundSprite->GetSprite());
			TerrainArray.Add(tmp);
		}
	}
}