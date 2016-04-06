// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "LFTestEmitter.h"
#include "liquidfun.h"


// Sets default values
ALFTestEmitter::ALFTestEmitter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALFTestEmitter::BeginPlay()
{
	Super::BeginPlay();
	
	b2Vec2 gravity(0.0f, -9.82f);

	b2World world(gravity);
}

// Called every frame
void ALFTestEmitter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

