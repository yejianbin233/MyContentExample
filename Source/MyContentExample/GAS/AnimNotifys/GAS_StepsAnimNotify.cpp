 // Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AnimNotifys/GAS_StepsAnimNotify.h"
#include "GAS/Character/GAS_Character.h"
#include "GAS/Components/GAS_FootstepsComponent.h"

 void UGAS_StepsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
 {
	 Super::Notify(MeshComp, Animation, EventReference);

 	check(MeshComp);

 	AGAS_Character* Character = MeshComp ? Cast<AGAS_Character>(MeshComp->GetOwner()) : nullptr;
	 if (Character)
	 {
		 // UGAS_FootstepsComponent* FootstepsComponent = Character->GetFootstepsComponent();
		 // if (FootstepsComponent)
		 // {
			//  FootstepsComponent->HandleFootstep(Foot);
		 // }
	 }
 }
