#include "SuggestWidget.h"

#include "PatchWidget.h"

Suggest::Widget::Widget(Patch::Widget* patchWidget)
   : QWidget(patchWidget)
   , patchWidget(patchWidget)
{
   setupUi(this);

   auto setIcon = [&](QLabel* iconLabel, Ref::Structure::PatchPart part)
   {
      iconLabel->setPixmap(patchWidget->maxRef.partIcon(part).pixmap(16, 16));
   };

   setIcon(argumentIcon, Ref::Structure::PatchPart::Argument);
   setIcon(typedMessageIcon, Ref::Structure::PatchPart::MessageTyped);
   setIcon(attributeIcon, Ref::Structure::PatchPart::Attribute);
   setIcon(nameMessageIcon, Ref::Structure::PatchPart::MessageNamed);

   // set suggest models
   argumentSuggestTree->init(patchWidget->argumentSuggestModel, argumnetTransferButton);
   typedMessageSuggestTree->init(patchWidget->typedMessageSuggestModel, typedMessageTransferButton);
   namedMessageSuggestTree->init(patchWidget->namedMessageSuggestModel, namedMessageTransferButton);

   // set reference models
   argumentPatchTree->setModel(patchWidget->argumentPatchModel);
   typedMessagePatchTree->setModel(patchWidget->typedMessagPatcheModel);
   namedMessagePatchTree->setModel(patchWidget->namedMessagePatchModel);
}

void Suggest::Widget::rebuild()
{
   Model::Abstract::rebuildAll();
}
