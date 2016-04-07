// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "TerrainActor.h"


// Sets default values
ATerrainActor::ATerrainActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = GroundSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	}
}

// Called when the game starts or when spawned
void ATerrainActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerrainActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATerrainActor::SetSprite(UPaperSprite* sprite)
{
	GroundSprite->SetSprite(sprite);
}

void ATerrainActor::SetLocation(float x, float y)
{
	SetActorLocation(FVector(x, y, 0.0f));
}