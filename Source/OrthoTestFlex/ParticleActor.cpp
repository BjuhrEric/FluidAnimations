// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "PaperSpriteComponent.h"
#include "ParticleActor.h"


// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	}
}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParticleActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AParticleActor::SetSprite(UPaperSprite* sprite)
{
	Sprite->SetSprite(sprite);
}

void AParticleActor::SetLocation(float x, float y)
{
	SetActorLocation(FVector(x, y, 0.0f));
}

