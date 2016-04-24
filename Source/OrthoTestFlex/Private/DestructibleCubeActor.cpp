// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "DestructibleCubeActor.h"


// Sets default values
ADestructibleCubeActor::ADestructibleCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleCubeActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADestructibleCubeActor::SetType(int32 type) {
	ADestructibleCubeActor::type = type;
}

int32 ADestructibleCubeActor::GetType() {
	return type;
}

