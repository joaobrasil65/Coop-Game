// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SWeapon.generated.h"

class UDamageType;
class UParticleSystem;

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
	virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapons")
		virtual void Fire();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	USoundBase* FireSound;
	

protected:
	
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* MeshComp;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	UParticleSystem* MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	UParticleSystem* FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapons")
	UParticleSystem* TracerEffect;
	
	void PlayFireEffects();
	
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	float BaseDamage;

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;
	
	// RPM bullets fired per minute
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	float RateOfFire;

	float TimeBetweenShots;



};
