




#TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#
#
PRJNAMETOOT = UE5CHUNKDOWNLOADER
DEFINES += "UE5CHUNKDOWNLOADER_API="
DEFINES += "UE5CHUNKDOWNLOADER_API(...)="
#
DEFINES += "UCLASS()=UE5CHUNKDOWNLOADER_API"
DEFINES += "UCLASS(...)=UE5CHUNKDOWNLOADER_API"
#
# this is true during development with unreal-editor...

DEFINES += "WITH_EDITORONLY_DATA=1"

## this project only

##


INCLUDEPATH += ../Intermediate/Build/Win64/UnrealEditor/Inc/$$PRJNAMETOOT/UHT
INCLUDEPATH += $$PRJNAMETOOT/Public $$PRJNAMETOOT/Private
#INCLUDEPATH += ../Plugins/NNEPostProcessing/Source/NNEPostProcessing/Public
# we should follow UE project struct to include files, start from prj.Build.cs folder
#
#  The Thirdparty libs
#
#
#
include(defs.pri)
include(inc.pri)
#
## this project only
# INCLUDEPATH += $$UESRCROOT/Runtime/Renderer/Private
##
#
#

DISTFILES += \
    Ue5ChunkDownloader.Target.cs \
    Ue5ChunkDownloader/Ue5ChunkDownloader.Build.cs \
    Ue5ChunkDownloaderEditor.Target.cs

HEADERS += \
    Ue5ChunkDownloader/Private/MainGameInst.h \
    Ue5ChunkDownloader/Ue5ChunkDownloader.h

SOURCES += \
    Ue5ChunkDownloader/Private/MainGameInst.cpp \
    Ue5ChunkDownloader/Ue5ChunkDownloader.cpp
