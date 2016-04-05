// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GridTerrain.h"
#include "GridPawn.generated.h"


UCLASS(Blueprintable)
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

	UFUNCTION(BlueprintCallable, Category = "Initialization") void init();

private:
	GridTerrain* gt = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions") unsigned int width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions") unsigned int height;
	
};
