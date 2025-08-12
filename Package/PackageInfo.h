#ifndef PackageInfoH
#define PackageInfoH

#include <QFileInfo>
#include <QString>

#include "PatchInfo.h"

namespace Package
{
   class Info
   {
   public:
      struct Entry
      {
         QString patchPath;
         QString sourcePath;
         QString refPath;
         QString helpPath;
         QString initPath;

         using Map = QMap<QString, Entry>;
         using Variable = QString Entry::*;

         bool isOrphan() const;
      };

   public:
      const Entry::Map& getEntryMap() const;
      void updateEntries();
      QStringList compileOrphanFiles() const;

      Patch::Info extractPatchInfo(const QString& patchFileName) const;
      const QString& getPath() const;
      const QString& getName() const;
      const QString& getAuthor() const;

   private:
      friend class Container;

   private:
      Info(const QString& path);

   private:
      void fillEntryMap(const QString& path, const bool scanSubFolders, const QString& extension, Entry::Variable entryVarialble);
      void fillEntrySources(const QString& path);

   private:
      const QString path;
      QString name;
      QString author;

      Entry::Map entryMap;
   };
} // namespace Package

#endif // NOT PackageInfoH
