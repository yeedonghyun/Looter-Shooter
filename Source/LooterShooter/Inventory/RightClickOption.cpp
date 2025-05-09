// Fill out your copyright notice in the Description page of Project Settings.


#include "RightClickOption.h"


void URightClickOption::NativeConstruct()
{
    Super::NativeConstruct();

    if (BUse)
    {
        BUse->OnClicked.AddDynamic(this, &URightClickOption::OnUseClicked);
    }

    if (BDrop)
    {
        BDrop->OnClicked.AddDynamic(this, &URightClickOption::OnDropClicked);
    }
}

void URightClickOption::OnUseClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("OnUseClicked")));

    OnActionSelected.Broadcast(ESlotActionType::USE);
}

void URightClickOption::OnDropClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("OnDropClicked")));

    OnActionSelected.Broadcast(ESlotActionType::DROP);
}