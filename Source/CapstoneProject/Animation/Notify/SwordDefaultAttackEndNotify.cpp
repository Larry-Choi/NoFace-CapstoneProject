// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/SwordDefaultAttackEndNotify.h"
#include "Interface/BowInterface.h"
#include "Character/CharacterDefaultAttackComponent.h"

USwordDefaultAttackEndNotify::USwordDefaultAttackEndNotify()
{
}

void USwordDefaultAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IBowInterface* Interface = Cast<IBowInterface>(MeshComp->GetOwner()->GetComponentByClass<UCharacterDefaultAttackComponent>());
	if (Interface)
	{
		Interface->SwordDefaultAttackEnd();
	}
}
