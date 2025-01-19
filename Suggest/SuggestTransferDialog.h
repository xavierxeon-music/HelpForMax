#ifndef SuggestTransferDialogH
#define SuggestTransferDialogH

#include "ui_SuggestTransferDialog.h"
#include <QDialog>

namespace Suggest
{
   namespace Transfer
   {
      class Model;

      class Dialog : public QDialog, private Ui::Dialog
      {
         Q_OBJECT

      public:
         Dialog(QWidget* parent);

      private:
         void slotAnalyse();
         void slotTransfer();

      private:
         Model* model;
      };
   } // namespace Transfer
} // namespace Suggest

#endif // NOT SuggestTransferDialogH
