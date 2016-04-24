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
	// Sets default values for this pawn's properties
	AGridPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	UFUNCTION(BlueprintCallable, Category="Initialization") void init();
	UFUNCTION(BlueprintCallable, Category="Initialization") void spawnCubes();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions") int32 width = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions") int32 height = 0;

private:
	bool mouseDown;
	float x, y;
	GridTerrain* gt = NULL;
	TSubclassOf<class ADestructibleCubeActor> MyItemBlueprint;

	inline void OnClick();
	inline void OnRelease();
	inline void SetMouseX(float x);
	inline void SetMouseY(float y);
};
