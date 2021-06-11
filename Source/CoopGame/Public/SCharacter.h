// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

#include "SCharacter.generated.h"

class ASWeapon;
class USHealthComponent;


UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginZoom();
	void EndZoom();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USpringArmComponent* SpringArm;

	USHealthComponent* HealthComp;

	bool bWantsToZoom = false;

	UPROPERTY(BlueprintReadWrite, Category= "Weapons")
	float ZoomedFOV;

	UPROPERTY(BlueprintReadWrite, Category= "Weapons")
	float DefaultFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, Category= "Weapons", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;
	
	float CurrentFOV;

	void BeginCrouch();

	void EndCrouch();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category= "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category= "Player")
	FName WeaponSocketAttachName;

	void StartFire();
	void StopFire();

	bool bDied;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

};
