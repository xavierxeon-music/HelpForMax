#ifndef PackageCleanDialogH
#define PackageCleanDialogH

#include "ui_PackageCleanDialog.h"
#include <QDialog>

namespace Package
{
   class Info;

   namespace Clean
   {
      class Model;

      class Dialog : public QDialog, private Ui::Dialog
      {
         Q_OBJECT

      public:
         Dialog(QWidget* parent, const Info* info);

      private slots:
         void slotCleanup();

      private:
         Model* model;
      };
   } // namespace Clean
} // namespace Package

#endif // PackageCleanDialogH
