// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"
#include "Item/Weapons/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::WeaponColliisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetSwordBox())
	{
		EquippedWeapon->GetSwordBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{

}

 void ABaseCharacter::AttackEnd()
{
	
}

 int32 ABaseCharacter::PlayAttackMontage()
 {
	 
	return PlayRandomMontageSection(AttackMontageSections, AttackMontage);
 }

 void ABaseCharacter::DirectionalHit(const FVector& ImpactPoint)
 {
	 //Super::GetHit(const FVector& ImpactPoint);
	 const FVector Location = ImpactPoint;
	 UWorld* World = GetWorld();
	 //DrawDebugSphere(World, Location, 8.f, 24, FColor::Orange, false, 5.f);


	 FVector ForwardVector = GetActorForwardVector();
	 FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal();
	 FVector ToHitDisplay = FVector(ToHit.X, ToHit.Y, ForwardVector.Z);

	 const double CosTheta = FVector::DotProduct(ForwardVector, ToHit);
	 double Theta = FMath::Acos(CosTheta);
	 Theta = FMath::RadiansToDegrees(Theta);

	 if (GEngine)
	 {
		 GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));

	 }
	 //UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60.f, 5.f, FColor::Red, 5.f);
	 //UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHitDisplay * 60.f, 5.f, FColor::Green, 5.f);

	 const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	 if (CrossProduct.Z < 0)
	 {
		 Theta *= -1.f;
	 }
	 //UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

	 FName SectionName = FName("React_Back");

	 if (Theta >= -45 && Theta < 45)
	 {
		 SectionName = FName("React_Front");
	 }
	 else if (Theta >= -135 && Theta < -45)
	 {
		 SectionName = FName("React_Left");
	 }
	 else if (Theta >= 45 && Theta < 135)
	 {
		 SectionName = FName("React_Right");
	 }
	 PlayHitMontage(SectionName);
 }

 void ABaseCharacter::PlayHitMontage(FName SectionName)
 {
	 UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	 if (AnimInstance && HitMontage)
	 {
		 AnimInstance->Montage_Play(HitMontage);
		 AnimInstance->Montage_JumpToSection(SectionName, HitMontage);
	 }
 }

 bool ABaseCharacter::IsInTargetRange(AActor* Target, double RangeValue)
 {
	 const double Distance = (Target->GetActorLocation() - GetActorLocation()).Size(); 
	 return Distance <= RangeValue;
 }
 void ABaseCharacter::EquippedWeaponDestroy()
 {

 }
 bool ABaseCharacter::IsCharacterAlive()
 {
	 return Attributes && Attributes->IsAlive();
 }
 void ABaseCharacter::PlayHitSound(const FVector ImpactPoint)
 {
	 if (HitSound)
		 if (HitSound)
		 {
			 UGameplayStatics::PlaySoundAtLocation(
				 this,
				 HitSound,
				 ImpactPoint
			 );
		 }
 }
 void ABaseCharacter::SpawnHitParticles(const FVector ImpactPoint)
 {
	 if (HitParticles)
	 {
		 UGameplayStatics::SpawnEmitterAtLocation
		 (GetWorld(),
			 HitParticles,
			 ImpactPoint);
	 }
 }
 void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
 {
	 UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	 if (AnimInstance && Montage)
	 {
		 AnimInstance->Montage_Play(Montage);
		 AnimInstance->Montage_JumpToSection(SectionName, Montage);
	 }
 }
 int32 ABaseCharacter::PlayRandomMontageSection(const TArray<FName> Section, UAnimMontage* Montage)
 {
	 if (Section.Num() <= 0) return -1;
	 const int32 MaxSectionIndex = Section.Num() - 1;
	 const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	 PlayMontageSection(Montage, Section[Selection]);
	 return Selection;
 }
 int32 ABaseCharacter::PlayDeathMontage()
 {
	 
	 return PlayRandomMontageSection(DeathMontageSections, DeathMontage);
 }
 void ABaseCharacter::Die()
 {
 }
