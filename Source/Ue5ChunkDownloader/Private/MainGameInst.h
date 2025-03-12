// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInst.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPatchFinishedSignature, bool, Succeeded);



/**
 * 
 */
UCLASS()
class UE5CHUNKDOWNLOADER_API UMainGameInst : public UGameInstance
{
	GENERATED_BODY()
	
                                              // UGameInstance interface
   public:
        virtual void Init() override;
       virtual void Shutdown() override;

    UFUNCTION(BlueprintPure, Category = "Toot|Stats")
    void FireLoadingProgress(int32& BytesDownloaded, int32& TotalBytes , float& DownloadPercent ,
                              int32& ChunkMounted , int32& TotalChunkToMount ,
                             float& MountPercent  ) const ;

    //
    UFUNCTION(BlueprintCallable, Category = "TOOT")
    bool PatchGame();

    //
    UPROPERTY(BlueprintAssignable, Category="Toot")
    FPatchFinishedSignature OnPatchFinised;

protected:

    void OnManifestUpdCompleted(bool isSuccess);
    //
    void OnDownloadFinished(bool isSuccess);
    void OnLoadingModeCompleted(bool isSuccess);
    void OnMountCompleted(bool isSuccess);

    UPROPERTY(EditDefaultsOnly, Category="TOOT")
    TArray<int32> ChunkDownloadList;

    bool bIsDownloadManifestUpToData;


};
