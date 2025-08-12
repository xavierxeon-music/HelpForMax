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
         void cleanup();
         QStandardItemModel* createModel();

      private:
         const Info* packageInfo;
      };
   } // namespace Clean
} // namespace Package

#endif // PackageCleanDialogH
