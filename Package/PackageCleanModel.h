#ifndef PackageCleanModelH
#define PackageCleanModelH

#include <QStandardItemModel>

namespace Package
{
   class Info;

   namespace Clean
   {
      class Model : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Model(QObject* parent, const Info* packageInfo);

      public:
         QStringList compileFiles() const;

      private:
         struct Entry
         {
            QString patchPath;
            QString refPath;
            QString helpPath;
            QString initPath;

            using Map = QMap<QString, Entry>;

            using Variable = QString Entry::*;
         };

      private:
         void create();
         void fillEntryMap(const QString& path, const bool scanSubFolders, const QString& extension, Entry::Variable entryVarialble);
         void fillEntrySources(const QString& path);

      private:
         const Info* packageInfo;
         Entry::Map entryMap;
      };
   } // namespace Clean
} // namespace Package

#endif // NOT PackageCleanModelH
