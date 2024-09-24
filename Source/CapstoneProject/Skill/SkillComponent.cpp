// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/SkillComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/CharacterSkillMontageData.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyBase.h"
#include "Skill/StaffMeteor.h"
#include "Skill/StaffArea.h"
#include "Skill/StaffUpGround.h"
#include "Skill/StaffThunderbolt.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = CastChecked<ACharacter>(GetOwner());
	PlayerController = CastChecked<APlayerController>(Character->GetController());
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCasting)
	{
		//Cursor가 멤버변수니까 해당 위치 정보를 가지고 각 스킬 함수에서 Cursor의 위치를 참조하여 시전 범위를 생성하면됨.
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, Cursor);
		DrawDebugSphere(GetWorld(), Cursor.Location, 20.f, 32, FColor::Red, false);
	}
}

void USkillComponent::PlaySkill_Q()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSwordSting();
		break;
	case 1:
		BeginBowSeveralArrows();
		break;
	case 2:
		BeginStaffMeteor();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_W()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSwordWhirlwind();
		break;
	case 1:
		BeginBowExplosionArrow();
		break;
	case 2:
		BeginStaffArea();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_E()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSwordParrying();
		break;
	case 1:
		BeginBowBackstep();
		break;
	case 2:
		BeginStaffUpGround();
		break;
	default:
		break;
	}
}

void USkillComponent::PlaySkill_R()
{
	switch (CurrentWeaponType)
	{
	case 0:
		BeginSwordAura();
		break;
	case 1:
		BeginBowAutoTargeting();
		break;
	case 2:
		BeginStaffThunderbolt();
		break;
	default:
		break;
	}
}

void USkillComponent::SetWeaponType(const int32& InCurrentWeaponType)
{
	CurrentWeaponType = InCurrentWeaponType;
	UE_LOG(LogTemp, Display, TEXT("Current Weapon Type : %d"), CurrentWeaponType);
}

void USkillComponent::BeginSwordSting()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[0], 1.0f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSwordSting);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[0]);
}

void USkillComponent::EndSwordSting(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void USkillComponent::BeginSwordWhirlwind()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[1], 1.0f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSwordWhirlwind);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[1]);
}

void USkillComponent::EndSwordWhirlwind(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void USkillComponent::BeginSwordParrying()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[2], 1.0f);
	ParryingSign.ExecuteIfBound();

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSwordParrying);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[2]);
}

void USkillComponent::EndSwordParrying(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	ParryingSign.ExecuteIfBound();
}

void USkillComponent::ParryingSuccess(AActor* Attacker)
{
	if (AEnemyBase* Enemy = Cast<AEnemyBase>(Attacker))
	{
		Enemy->Stun();
		//반격 애니메이션 혹은 추가 공격;
	}
}

void USkillComponent::BeginSwordAura()
{
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	AnimInstance->Montage_Play(SkillMontageData->SwordMontages[3], 3.0f);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &USkillComponent::EndSwordAura);
	AnimInstance->Montage_SetEndDelegate(MontageEnd, SkillMontageData->SwordMontages[3]);
}

void USkillComponent::EndSwordAura(UAnimMontage* Target, bool IsProperlyEnded)
{
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void USkillComponent::BeginBowSeveralArrows()
{
}

void USkillComponent::EndBowSeveralArrows(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginBowExplosionArrow()
{
	if (bCasting)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RainArrows, Cursor.Location, FRotator::ZeroRotator);
		UGameplayStatics::ApplyRadialDamage(GetOwner(), 50.f, Cursor.Location, 200.f, UDamageType::StaticClass(), TArray<AActor*>(), GetOwner());

		bCasting = false;
	}
	else
	{
		bCasting = true;

		SkillQueue.Enqueue([this]()
			{
				BeginBowExplosionArrow();
			});
	}
}

void USkillComponent::EndBowExplosionArrow(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginBowBackstep()
{
}

void USkillComponent::EndBowBackstep(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginBowAutoTargeting()
{
}

void USkillComponent::EndBowAutoTargeting(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginStaffMeteor()
{
	if (bCasting)
	{
		FVector SpawnLocation = Character->GetActorLocation() + FVector(0.f, 0.f, 150.f);
		FRotator SpawnRotation = Character->GetActorRotation();
		AStaffMeteor* Meteor = GetWorld()->SpawnActor<AStaffMeteor>(MeteorClass, SpawnLocation, SpawnRotation);
		Meteor->Init(Cursor.Location);

		bCasting = false;
	}
	else
	{
		bCasting = true;
		
		//컨테이너에 람다형식으로 함수 등록
		SkillQueue.Enqueue([this]() {
			BeginStaffMeteor();
        });
	}
}

void USkillComponent::EndStaffMeteor(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginStaffArea()
{
	if (bCasting)
	{
		FVector SpawnLocation = Cursor.Location;
		FRotator SpawnRotation = Character->GetActorRotation();
		AStaffArea* Area = GetWorld()->SpawnActor<AStaffArea>(AreaClass, SpawnLocation, SpawnRotation);
		Area->SetOwner(Character);

		bCasting = false;
	}
	else
	{
		bCasting = true;

		//컨테이너에 람다형식으로 함수 등록
		SkillQueue.Enqueue([this]()
			{
				BeginStaffArea();
			});
	}
}

void USkillComponent::EndStaffArea(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginStaffUpGround()
{
	if (bCasting)
	{
		FVector SpawnLocation = Cursor.Location;
		FRotator SpawnRotation = Character->GetActorRotation();
		AStaffUpGround* UpGround = GetWorld()->SpawnActor<AStaffUpGround>(UpGroundClass, SpawnLocation, SpawnRotation);
		UpGround->SetOwner(Character);
		UpGround->ActiveGroundUp();

		bCasting = false;
	}
	else
	{
		bCasting = true;

		//컨테이너에 람다형식으로 함수 등록
		SkillQueue.Enqueue([this]()
			{
				BeginStaffUpGround();
			});
	}
}

void USkillComponent::EndStaffUpGround(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void USkillComponent::BeginStaffThunderbolt()
{
	FVector SpawnLocation = Character->GetMesh()->GetSocketLocation(TEXT("root")) + FVector(0.f, 0.f, 5.f);
	FRotator SpawnRotation = Character->GetActorRotation();
	AStaffThunderbolt* Thunderbolt = GetWorld()->SpawnActor<AStaffThunderbolt>(ThunderboltClass, SpawnLocation, SpawnRotation);
	Thunderbolt->SetOwner(Character);
	Thunderbolt->ActiveThunderbolt();
}

void USkillComponent::EndStaffThunderbolt(UAnimMontage* Target, bool IsProperlyEnded)
{
}



