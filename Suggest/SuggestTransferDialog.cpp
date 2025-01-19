#include "SuggestTransferDialog.h"

Suggest::Transfer::Dialog::Dialog(QWidget* parent)
   : QDialog(parent)
   , model(nullptr)
{
   setupUi(this);

   connect(analyseButton, &QPushButton::clicked, this, &Dialog::slotAnalyse);
   connect(transferButton, &QPushButton::clicked, this, &Dialog::slotTransfer);
}

void Suggest::Transfer::Dialog::slotAnalyse()
{
}

void Suggest::Transfer::Dialog::slotTransfer()
{
}
