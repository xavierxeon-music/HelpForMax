#include "SuggestTransferDialog.h"

#include "SuggestTransferModel.h"

Suggest::Transfer::Dialog::Dialog(QWidget* parent, const Package::Info* packageInfo)
   : QDialog(parent)
   , model(nullptr)
{
   setupUi(this);

   model = new Model(this, packageInfo);
   packageTree->setModel(model);

   connect(analyseButton, &QPushButton::clicked, this, &Dialog::slotAnalyse);
   connect(transferButton, &QPushButton::clicked, this, &Dialog::slotTransfer);
}

void Suggest::Transfer::Dialog::slotAnalyse()
{
}

void Suggest::Transfer::Dialog::slotTransfer()
{
}
