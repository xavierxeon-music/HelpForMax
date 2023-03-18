#include "CleanDialog.h"

#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

#include "CleanModel.h"

Clean::Dialog::Dialog(QWidget* parent, const QStringList& helpFileList)
   : QDialog(parent)
   , model(nullptr)
{
   setWindowTitle("Cleanup Help Files");
   setMinimumWidth(600);

   model = new Model(this, helpFileList);

   QTreeView* cleanView = new QTreeView(this);
   cleanView->setModel(model);
   cleanView->resizeColumnToContents(0);
   cleanView->resizeColumnToContents(1);

   QPushButton* applyButton = new QPushButton("Apply", this);
   connect(applyButton, &QPushButton::clicked, this, &Dialog::accept);

   QVBoxLayout* masterLayout = new QVBoxLayout(this);
   masterLayout->setContentsMargins(5, 0, 5, 0);
   masterLayout->addWidget(cleanView);
   masterLayout->addWidget(applyButton);
}

void Clean::Dialog::accept()
{
   model->apply();
   QDialog::accept();
}
