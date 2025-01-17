// Copyright 2020, Kevin VanHorn. All rights reserved.


#include "PSaveGame.h"
#include "KeepThePuppyDry.h"

FString UPSaveGame::SaveSlotName = "LocalSaveDefault";
uint32 UPSaveGame::UserIndex = 0;

bool UPSaveGame::SynchronousSave(int32 CurrencyIn, bool bShowTutorialIn, const TArray<uint8>& UPatternsIn, int32 LS_UmbrellaPatternIn,
    const TArray<uint8>& DogChoicesIn, int32 LS_DogIn)
{
    if (UPSaveGame* SaveGameInstance = Cast<UPSaveGame>(UGameplayStatics::CreateSaveGameObject(UPSaveGame::StaticClass())))
    {
        // Set data on the savegame object:
        SaveGameInstance->Currency = CurrencyIn;
        SaveGameInstance->bShowTutorial = bShowTutorialIn;
        SaveGameInstance->UPatterns = UPatternsIn;
        SaveGameInstance->LS_UmbrellaPattern = LS_UmbrellaPatternIn;
        SaveGameInstance->LS_Dog = LS_DogIn;
        SaveGameInstance->DogChoices = DogChoicesIn;

        // Save the data immediately.
        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex))
        {
            return true;
        }
    }
    return false;
}
