//
// Created by gugue_u on 12/05/2021.
//


#include "FileData.h"
#include "SaveState.h"
#include <utils/storage/Set.h>
#include <views/gamelist/IGameListView.h>

class GameFilesUtils
{
  public:
    static HashSet<String> GetGameSubFiles(FileData& game);
    static HashSet<String> GetGameExtraFiles(FileData& fileData);
    static bool HasAutoPatch(const FileData* fileData);
    static Path GetSubDirPriorityPatch(const FileData* fileData);
    static std::list<Path> GetSoftPatches(const FileData* fileData);
    static HashSet<std::string> GetGameSaveFiles(FileData& game);
    static std::list<SaveState> GetGameSaveStateFiles(FileData& game);
    static HashSet<std::string> GetMediaFiles(FileData& fileData);
    static HashSet<String> GetGameSaveFiles(FileData& game);
    static HashSet<String> GetMediaFiles(FileData& fileData);

    static bool ContainsMultiDiskFile(const String& extensions)
    {
      return Strings::Contains(extensions, ".m3u") || Strings::Contains(extensions, ".cue") ||
             Strings::Contains(extensions, ".ccd") || Strings::Contains(extensions, ".gdi");
    }

    static void ExtractUselessFiles(const Path& path, HashSet<String>& list);
    static void ExtractUselessFilesFromCue(const Path& path, HashSet<String>& list);
    static void ExtractUselessFilesFromCcd(const Path& path, HashSet<String>& list);
    static void ExtractUselessFilesFromM3u(const Path& path, HashSet<String>& list);
    static void ExtractUselessFilesFromGdi(const Path& path, HashSet<String>& list);
    static String ExtractFileNameFromLine(const String& line);
    static void AddIfExist(const Path& path, HashSet<String>& list);
    static constexpr int sMaxGdiFileSize = (10 << 10); // 10 Kb

    static void DeleteSelectedFiles(FileData& fileData, HashSet<String>&, HashSet<String>&);
    static void DeleteAllFiles(FileData& fileData);

    static bool IsMediaShared(FileData& system, const Path& mediaPath);

    static void DeleteFoldersRecIfEmpty(FolderData* folderData);
};


