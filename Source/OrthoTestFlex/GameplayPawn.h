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

	// Called when the game ends
	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	void InitFluids();

	void InitTerrain();

	void LFCleanUp();

	void UpdateLF(float DeltaTime);

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

	TMap<FVector2D, class b2Body*> TerrainBodies;

	TMap<FVector2D, int32> TerrainInstanceIndices;

	//The scaling used to translate LiquidFun coordinates to world space
	const float SCALE_FACTOR = 10.0f;
};
