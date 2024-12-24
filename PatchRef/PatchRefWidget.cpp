#include "PatchRefWidget.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QScrollArea>

#include "DelegateDataType.h"
#include "DelegatePatchType.h"
#include "DescriptionHighlighter.h"
#include "PackageInfo.h"
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
   headerPatchTree->init(this, patchWidget->headerPatchModel, 1);
   headerPatchTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, patchWidget->headerPatchModel));

   argumentPatchTree->init(this, patchWidget->argumentPatchModel);
   argumentPatchTree->setButtons(argumentAddButton, argumentRemoveButton);
   argumentPatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, patchWidget->argumentPatchModel));

   typedMessagePatchTree->init(this, patchWidget->typedMessagPatcheModel);

   namedMessagePatchTree->init(this, patchWidget->namedMessagePatchModel);
   namedMessagePatchTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
   namedMessagePatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, patchWidget->namedMessagePatchModel));

   outputPatchTree->init(this, patchWidget->outputPatchModel);
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
   Model::Abstract::callOnAllInstances(&RefModel::rebuild);
   setDigest(&patchWidget->maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void PatchRef::Widget::update()
{
   Model::Abstract::callOnAllInstances(&RefModel::update);
}

void PatchRef::Widget::setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part)
{
   iconLabel->setPixmap(patchWidget->maxRef.partIcon(part).pixmap(16, 16));
}
