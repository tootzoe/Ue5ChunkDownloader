// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInst.h"

#include "ChunkDownloader.h"
#include "Misc/CoreDelegates.h"
#include "Serialization/JsonTypes.h"
#include "AssetRegistry/AssetRegistryModule.h"




void UMainGameInst::Init()
{
    Super::Init();

    const FString devName = TEXT("TootTestUe5ChunkDownloader");
    const FString cntBuildID = TEXT("TootDemoKey");

    TSharedRef<FChunkDownloader> dloader = FChunkDownloader::GetOrCreate();
    dloader->Initialize("Windows" , 8); // set to 8 the maximum number of downloads that ChunkDownloader will handle at once.


    dloader->LoadCachedBuild(devName);
    TFunction<void(bool)> updFinishedCb = [&](bool){ bIsDownloadManifestUpToData = true; };
    dloader->UpdateBuild(devName , cntBuildID , updFinishedCb);
}

void UMainGameInst::Shutdown()
{
    Super::Shutdown();

    FChunkDownloader::Shutdown();
}

void UMainGameInst::FireLoadingProgress(int32 &BytesDownloaded, int32 &TotalBytes, float &DownloadPercent, int32 &ChunkMounted, int32 &TotalChunkToMount, float &MountPercent) const
{
    TSharedRef<FChunkDownloader> dl  = FChunkDownloader::GetChecked();

    FChunkDownloader::FStats currStats = dl->GetLoadingStats();

    BytesDownloaded = currStats.BytesDownloaded;
    TotalBytes = currStats.TotalBytesToDownload;
    ChunkMounted = currStats.ChunksMounted;
    TotalChunkToMount = currStats.TotalChunksToMount;
    //

    DownloadPercent = ((float) BytesDownloaded / (float)TotalBytes) * 100.f;
    MountPercent = ((float) ChunkMounted / (float)TotalChunkToMount) * 100.f;

}

bool UMainGameInst::PatchGame()
{
    if(bIsDownloadManifestUpToData ){
        TSharedRef<FChunkDownloader> dl = FChunkDownloader::GetChecked();

        for (int32 id :  ChunkDownloadList) {
            int32 tmpStats = static_cast<int32>(dl->GetChunkStatus(id));
            UE_LOG(LogTemp, Warning, TEXT("Chunk %i  status : %i") , id , tmpStats);
        }

        TFunction<void(bool)> dlFinishedCb = [&](bool isSuccess){ OnDownloadFinished(isSuccess);};
        dl->DownloadChunks(ChunkDownloadList,  dlFinishedCb , 1);

        TFunction<void(bool)> loadingModeFinishedCb = [&](bool b){OnLoadingModeCompleted(b);};
        dl->BeginLoadingMode(loadingModeFinishedCb);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("Manifest update Failed........ Can't patch the game......."));

  return false;
}

void UMainGameInst::OnManifestUpdCompleted(bool isSuccess)
{
    bIsDownloadManifestUpToData = isSuccess;

}

void UMainGameInst::OnDownloadFinished(bool isSuccess)
{
    if(isSuccess){
        UE_LOG(LogTemp, Warning, TEXT("Download Finished......."));

        TSharedRef<FChunkDownloader> dl = FChunkDownloader::GetChecked();
        FJsonSerializableArrayInt dlChunks;

        for(int32 chunkID : ChunkDownloadList){
            dlChunks.Add(chunkID);
        }

        TFunction<void(bool)> mountFinishedCb = [&](bool b){OnMountCompleted(b);};
        dl->MountChunks(dlChunks , mountFinishedCb);

        OnPatchFinised.Broadcast(true);

    }else{
        UE_LOG(LogTemp, Warning, TEXT("Failed when load chunks......"));

        OnPatchFinised.Broadcast(false);

    }

}

void UMainGameInst::OnLoadingModeCompleted(bool isSuccess)
{
    OnDownloadFinished(isSuccess);

}

void UMainGameInst::OnMountCompleted(bool isSuccess)
{
    OnPatchFinised.Broadcast(isSuccess);
}
