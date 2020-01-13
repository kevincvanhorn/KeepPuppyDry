// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PSaveGame.h"
#include "KeepThePuppyDry.h"

FString UPSaveGame::SaveSlotName = "LocalSaveDefault";
uint32 UPSaveGame::UserIndex = 0;

bool UPSaveGame::SynchronousSave(int32 CurrencyIn, bool bShowTutorialIn)
{
    if (UPSaveGame* SaveGameInstance = Cast<UPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSaveGame::StaticClass())))
    {
        // Set data on the savegame object:
        SaveGameInstance->Currency = CurrencyIn;
        SaveGameInstance->bShowTutorial = bShowTutorialIn;

        // Save the data immediately.
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
        {
            return true;
        }
    }
    return false;
}
