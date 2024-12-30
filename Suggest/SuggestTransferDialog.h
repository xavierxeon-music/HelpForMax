#ifndef SuggestTransferDialogH
#define SuggestTransferDialogH

#include "ui_SuggestTransferDialog.h"
#include <QDialog>

namespace Suggest
{
   class TransferDialog : public QDialog, private Ui::TransferDialog
   {
      Q_OBJECT

   public:
      TransferDialog(QWidget* parent);
   };
} // namespace Suggest

#endif // NOT SuggestTransferDialogH
