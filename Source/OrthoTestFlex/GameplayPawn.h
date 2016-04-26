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

	UFUNCTION(BlueprintCallable, Category = "Cleanup")
	void CleanUpLF();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
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

	void AddSpriteAndUpdate(int x, int y);

	void MakeIndestructible(int x, int y);

	//The sprite used for the fluid particles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fluid", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* FluidSprite;

	//The sprite used for the terrain
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* GroundSprite;

	//The sprite used for the indestructible terrain
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* IDSprite;

	//Background sprite
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Background", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* BGSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fluid", meta = (AllowPrivateAccess = "true"))
	class UPaperGroupedSpriteComponent* GroupedFluidSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground", meta = (AllowPrivateAccess = "true"))
	class UPaperGroupedSpriteComponent* GroupedGroundSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Indestructible", meta = (AllowPrivateAccess = "true"))
	class UPaperGroupedSpriteComponent* GroupedIDSprite;

	//The scaling used to translate LiquidFun coordinates to world space
	const float SCALE_FACTOR = 10.0f;

	bool LMBPressed = false;



	static const int gridX = -50;
	static const int gridY = -465;
	static const int width = 101;
	static const int height = 650;
	static const int spacing = 1;
	char grid[width][height];
	class b2Body* TerrainBodies[width][height];
	TSet<FVector2D> boxSet;
	int32 TerrainInstanceIndices[width][height];
};
