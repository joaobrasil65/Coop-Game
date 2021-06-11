// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include <dsound.h>

#include "DrawDebugHelpers.h"
#include "CoopGame/CoopGame.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Console Variable
static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.debugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);


// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	// MeshComp->SetRelativeRotation(RotationOffset);

	MuzzleSocketName = "MuzzleSocket";
	BaseDamage = 20.0f;
	RateOfFire = 300.0f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots  = 60/RateOfFire;
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
	
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		// we overrode this viewpoint
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceStart = EyeLocation;
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		// Tracer particle end Target parameter
		FVector TracerEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bReturnPhysicalMaterial = true;
		// QueryParams.AddIgnoredActor(MyOwner);
		// exact hit
		QueryParams.bTraceComplex = true;

		FHitResult Hit;
		FVector HitFromDirection = EyeRotation.Vector();

		


		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd,
		                                         COLLISION_WEAPON, QueryParams))
		{
			// if(GEngine)
			// {
			// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
			// }

			// Blocking hit then process damage
			AActor* HitActor = Hit.GetActor();

			// had to add "PhysicsCore" in the Build.cs to make this work
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			UParticleSystem* SelectedEffect = nullptr;

			//if (SurfaceType == )

			switch (SurfaceType)
			{
			case SurfaceType8:
				SelectedEffect = FleshImpactEffect;
				break;
				
			default: SelectedEffect = DefaultImpactEffect;
				break;
			}

			FString HitName = Hit.GetActor()->GetName().ToLower();

			// DAMAGE
			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESH_VULNERABLE)
			{
				// increasing the actual damage by 4 times the existing value
				ActualDamage *= 4.0f;

				
			} else
			{
				ActualDamage = BaseDamage;
			}
			FFloat16 damage = UGameplayStatics::ApplyPointDamage(
				HitActor, ActualDamage, HitFromDirection, Hit,
				MyOwner->GetInstigatorController(), this, DamageType);

			
			FString damageAmount = FString::SanitizeFloat(damage);
			
			// DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 5.0f,0, 1.0f);
			              
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, damageAmount);

			// UGameplayStatics::ApplyDamage(HitActor, 30.0f, MyOwner->GetInstigatorController(), this, DamageType);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			                                 HitName + damageAmount);
			DrawDebugString(GetWorld(), Hit.Location, damageAmount, HitActor, FColor::Orange, 4.0f);

			DrawDebugSphere(GetWorld(), Hit.Location, 10.0f, 16, FColor::Red, false, 5.0f,
			                0, 1.0f);




			switch (SurfaceType)
			{
				case SurfaceType8:
					SelectedEffect = FleshImpactEffect;
				break;
				
				default: SelectedEffect = DefaultImpactEffect;
					break;
			}

			if (SelectedEffect)
			{
			
				UGameplayStatics::SpawnEmitterAtLocation(
                    GetWorld(), SelectedEffect, Hit.ImpactPoint,
                    Hit.ImpactNormal.Rotation());
				
			}

			TracerEndPoint = Hit.ImpactPoint;	
		}

		if(DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Green, false, 5.0f,0, 1.0f);
		}
		
		PlayFireEffects();
		LastFireTime = GetWorld()->TimeSeconds;
	
	}
}



void ASWeapon::PlayFireEffects()
{
	FVector EyeLocation;
	FRotator EyeRotation;
	APawn* MyOwner = Cast<APawn>(GetOwner());

	if(MyOwner)
	{
		// we overrode this viewpoint
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector TraceStart = EyeLocation;
		FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 10000);

		// Tracer particle end Target parameter
		FVector TracerEndPoint = TraceEnd;
	
		FVector MuzzleSocketLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleSocketRotator = MeshComp->GetSocketRotation(MuzzleSocketName);
	
		//DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp,
                                                   MuzzleSocketName);
			if(FireSound)
			{
				UGameplayStatics::SpawnSoundAtLocation(this, FireSound, MuzzleSocketLocation, MuzzleSocketRotator, 1.0f, 1.0f,
					0.0f, nullptr, nullptr, true );
			}
			
		}
		
		if (TracerEffect)
		{
			// particle system component
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect,
                                                 MuzzleSocketLocation, MuzzleSocketRotator);
			if (TracerComp)
			{
				TracerComp->SetVectorParameter("BeamEnd", TracerEndPoint);
			}
		}

		// APawn* MyOwner = Cast<APawn>(GetOwner());
		if (MyOwner)
		{
			APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
			if(PC)
			{
				// his camera shake was deprecated
				PC->ClientStartCameraShake(FireCameraShake);
			}
		}
	}

}


