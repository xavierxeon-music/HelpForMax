#include "PatchRefWidget.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QScrollArea>

#include "DelegateDataType.h"
#include "DelegatePatchType.h"
#include "DescriptionHighlighter.h"
#include "PackageInfo.h"
#include "PatchRefModelArgument.h"
#include "PatchRefModelHeader.h"
#include "PatchRefModelNamedMessage.h"
#include "PatchRefModelOutput.h"
#include "PatchRefModelTypedMessage.h"
#include "PatchWidget.h"

PatchRef::Widget::Widget(Patch::Widget* patchWidget)
   : QWidget(patchWidget)
   , patchWidget(patchWidget)
   , digest(nullptr)
{
   setupUi(this);

   // left: scroll area
   setIcon(patchIcon, Ref::Structure::PatchPart::Header);
   setIcon(argumentIcon, Ref::Structure::PatchPart::Argument);
   setIcon(typedMessageIcon, Ref::Structure::PatchPart::MessageTyped);
   setIcon(attributeIcon, Ref::Structure::PatchPart::Attribute);
   setIcon(nameMessageIcon, Ref::Structure::PatchPart::MessageNamed);
   setIcon(outputIcon, Ref::Structure::PatchPart::Output);
   setIcon(otherIcon, Ref::Structure::PatchPart::Other);

   // set reference models
   PatchRef::Model::Header* headerPatchModel = new PatchRef::Model::Header(this, patchWidget->maxRef);
   headerPatchTree->init(this, headerPatchModel, 1);
   headerPatchTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerPatchModel));

   PatchRef::Model::Argument* argumentPatchModel = new PatchRef::Model::Argument(this, patchWidget->maxRef);
   argumentPatchTree->init(this, argumentPatchModel);
   argumentPatchTree->setButtons(argumentAddButton, argumentRemoveButton);
   argumentPatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentPatchModel));

   PatchRef::Model::TypedMessage* typedMessagPatcheModel = new PatchRef::Model::TypedMessage(this, patchWidget->maxRef);
   typedMessagePatchTree->init(this, typedMessagPatcheModel);

   PatchRef::Model::NamedMessage* namedMessagePatchModel = new PatchRef::Model::NamedMessage(this, patchWidget->maxRef);
   namedMessagePatchTree->init(this, namedMessagePatchModel);
   namedMessagePatchTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
   namedMessagePatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessagePatchModel));

   PatchRef::Model::Output* outputPatchModel = new PatchRef::Model::Output(this, patchWidget->maxRef);
   outputPatchTree->init(this, outputPatchModel);
   outputPatchTree->setButtons(outputAddButton, outputRemoveButton);

   // right: digest area
   new DescriptionHighlighter(descriptionEdit->document());
   descriptionIcon->setPixmap(QIcon(":/DocDescription.svg").pixmap(16, 16));

   connect(digestEdit, &QLineEdit::editingFinished, this, &Widget::slotSaveDigestText);
   connect(descriptionEdit, &QTextEdit::textChanged, this, &Widget::slotSaveDigestDescription);
}

void PatchRef::Widget::slotSetPatchDigest()
{
   setDigest(&patchWidget->maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void PatchRef::Widget::slotSaveDigestText()
{
   digest->text = digestEdit->text();
   patchWidget->setDirty();
}

void PatchRef::Widget::slotSaveDigestDescription()
{
   digest->description = descriptionEdit->toPlainText();
   patchWidget->setDirty();
}

void PatchRef::Widget::setDigest(Ref::Structure::Digest* newDigest, const Ref::Structure::PatchPart& part)
{
   digest = newDigest;
   if (!digest)
   {
      topicInfo->clear();
      digestEdit->clear();
      descriptionEdit->clear();
      return;
   }

   setIcon(topicIcon, part);

   topicInfo->setText(patchWidget->maxRef.partName(part));
   digestEdit->setText(digest->text);

   descriptionEdit->blockSignals(true);
   descriptionEdit->setText(digest->description);
   descriptionEdit->blockSignals(false);
}

void PatchRef::Widget::rebuild()
{
   PatchRef::Model::Abstract::rebuildAll();
   setDigest(&patchWidget->maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void PatchRef::Widget::update()
{
   PatchRef::Model::Abstract::updateAll();
}

void PatchRef::Widget::setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part)
{
   iconLabel->setPixmap(patchWidget->maxRef.partIcon(part).pixmap(16, 16));
}
