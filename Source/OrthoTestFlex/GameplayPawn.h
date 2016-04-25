// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GameplayPawn.generated.h"

UCLASS()
class ORTHOTESTFLEX_API AGameplayPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGameplayPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	void InitFluids();

	void InitTerrain();

	void LFCleanUp();

	void SetLMBPressed();

	void SetLMBReleased();

	void UpdateLF(float DeltaTime);

	FVector2D GetMouseWorldPosition();

	void DestroySquare(FVector2D worldCoord, float r);

	int WorldToGridX(int x);
	int WorldToGridY(int y);
	float GridToWorldX(int x);
	float GridToWorldY(int y);
	void UpdatePos(int x, int y);
	void DestroyBody(int x, int y);
	void CreateBody(int x, int y);
	void RemoveAtIndex(int x, int y);

	void RemoveSpriteAndUpdate(int x, int y);

	//The sprite used for the fluid particles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fluid", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* FluidSprite;

	//The sprite used for the terrain
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* GroundSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fluid", meta = (AllowPrivateAccess = "true"))
	class UPaperGroupedSpriteComponent* GroupedFluidSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	class UPaperGroupedSpriteComponent* GroupedGroundSprite;

	//TMap<FVector2D, class b2Body*> TerrainBodies;

	//TMap<FVector2D, int32> TerrainInstanceIndices;

	//The scaling used to translate LiquidFun coordinates to world space
	const float SCALE_FACTOR = 10.0f;

	bool LMBPressed = false;



	static const int gridX = -50;
	static const int gridY = -100;
	static const int width = 100;
	static const int height = 100;
	static const int spacing = 1;
	char grid[width][height];
	class b2Body* TerrainBodies[width][height];
	TSet<FVector2D> boxSet;
	int32 TerrainInstanceIndices[width][height];
};
