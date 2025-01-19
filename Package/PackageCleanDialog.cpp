#include "PackageCleanDialog.h"

Package::Clean::Dialog::Dialog(QWidget* parent)
   : QDialog(parent)
   , model(nullptr)
{
   setupUi(this);

   connect(cleanupButton, &QPushButton::clicked, this, &Dialog::slotCleanup);
}

void Package::Clean::Dialog::slotCleanup()
{
}
