// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GridTerrain.h"
#include "GridPawn.generated.h"


UCLASS()
class ORTHOTESTFLEX_API AGridPawn : public APawn
{
	GENERATED_BODY()

public:

	AGridPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	inline const unsigned int getHeight() const;
	inline const unsigned int getWidth() const;

	inline const void setHeight(unsigned int);
	inline const void setWidth(unsigned int);

	void init();

private:
	GridTerrain* gt = NULL;
	unsigned int width = 0;
	unsigned int height = 0;
	
};
