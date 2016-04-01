// Fill out your copyright notice in the Description page of Project Settings.

#include "OrthoTestFlex.h"
#include "GridPawn.h"

// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

inline const unsigned int AGridPawn::getHeight() const {
	return height;
};

inline const unsigned int AGridPawn::getWidth() const {
	return width;
};

void AGridPawn::init() {
	gt = new GridTerrain(width, height);
}

// Called every frame
void AGridPawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

inline const void AGridPawn::setHeight(unsigned int h) {
	height = h;
};

inline const void AGridPawn::setWidth(unsigned int w) {
	width = w;
};


