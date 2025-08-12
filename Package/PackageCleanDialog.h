#ifndef PackageCleanDialogH
#define PackageCleanDialogH

#include "ui_PackageCleanDialog.h"
#include <QDialog>

#include <QStandardItemModel>

namespace Package
{
   class Info;

   namespace Clean
   {

      class Dialog : public QDialog, private Ui::Dialog
      {
         Q_OBJECT

      public:
         Dialog(QWidget* parent, const Info* packageInfo);

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
         void cleanup();
         QStringList compileFiles() const;
         QStandardItemModel* createModel();
         void fillEntryMap(const QString& path, const bool scanSubFolders, const QString& extension, Entry::Variable entryVarialble);
         void fillEntrySources(const QString& path);

      private:
         const Info* packageInfo;

         Entry::Map entryMap;
      };
   } // namespace Clean
} // namespace Package

#endif // PackageCleanDialogH
