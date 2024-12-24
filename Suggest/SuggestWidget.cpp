#include "SuggestWidget.h"

#include "PatchWidget.h"

Suggest::Widget::Widget(Patch::Widget* patchWidget)
   : QWidget(patchWidget)
   , patchWidget(patchWidget)
{
   setupUi(this);

   static const QString transferArrow = QString::fromUtf8("\u27a4");
   static const QString styleSheet("text-align:left;");

   transferAllButton->setText("Transfer All " + transferArrow);
   transferAllButton->setStyleSheet(styleSheet);
   connect(transferAllButton, &QAbstractButton::clicked, this, &Widget::slotTransferAll);

   transferSelectedButton->setText("Transfer Selected " + transferArrow);
   transferSelectedButton->setStyleSheet(styleSheet);
   connect(transferSelectedButton, &QAbstractButton::clicked, this, &Widget::slotTransferSelected);

   auto setIcon = [&](QLabel* iconLabel, Ref::Structure::PatchPart part)
   {
      iconLabel->setPixmap(patchWidget->maxRef.partIcon(part).pixmap(16, 16));
   };

   setIcon(argumentIcon, Ref::Structure::PatchPart::Argument);
   setIcon(typedMessageIcon, Ref::Structure::PatchPart::MessageTyped);
   setIcon(attributeIcon, Ref::Structure::PatchPart::Attribute);
   setIcon(nameMessageIcon, Ref::Structure::PatchPart::MessageNamed);

   // set suggest models
   argumentSuggestTree->init(patchWidget->argumentSuggestModel);
   typedMessageSuggestTree->init(patchWidget->typedMessageSuggestModel);
   namedMessageSuggestTree->init(patchWidget->namedMessageSuggestModel);

   // set reference models
   argumentPatchTree->setModel(patchWidget->argumentPatchModel);
   typedMessagePatchTree->setModel(patchWidget->typedMessagPatcheModel);
   namedMessagePatchTree->setModel(patchWidget->namedMessagePatchModel);
}

void Suggest::Widget::rebuild()
{
   Model::Abstract::callOnAllInstances(&RefModel::rebuild);
}

void Suggest::Widget::slotTransferAll()
{
   argumentSuggestTree->transferAll();
   typedMessageSuggestTree->transferAll();
   namedMessageSuggestTree->transferAll();
}

void Suggest::Widget::slotTransferSelected()
{
   argumentSuggestTree->transferSelected();
   typedMessageSuggestTree->transferSelected();
   namedMessageSuggestTree->transferSelected();
}
