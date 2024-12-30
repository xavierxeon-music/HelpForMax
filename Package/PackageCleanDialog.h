#ifndef PackageCleanDialogH
#define PackageCleanDialogH

#include "ui_PackageCleanDialog.h"
#include <QDialog>

namespace Package
{
   class CleanDialog : public QDialog, private Ui::CleanDialog
   {
      Q_OBJECT

   public:
      CleanDialog(QWidget* parent);
   };
} // namespace Package

#endif // PackageCleanDialogH
