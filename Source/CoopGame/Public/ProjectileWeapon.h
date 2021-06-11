// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API AProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()

	protected:

	virtual void Fire() override;
	
	UPROPERTY(EditAnywhere, Category="Projectile Weapon")
	TSubclassOf<AActor> ProjectileClass;

	
};
