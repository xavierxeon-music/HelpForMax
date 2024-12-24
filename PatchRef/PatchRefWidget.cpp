#include "PatchRefWidget.h"

#include <QDesktopServices>
#include <QHBoxLayout>
#include <QScrollArea>

#include "DelegateDataType.h"
#include "DelegatePatchType.h"
#include "DescriptionHighlighter.h"
#include "FileHelp.h"
#include "FileInit.h"
#include "FileRef.h"
#include "PackageInfo.h"
#include "PatchRefModelArgument.h"
#include "PatchRefModelHeader.h"
#include "PatchRefModelNamedMessage.h"
#include "PatchRefModelOutput.h"
#include "PatchRefModelTypedMessage.h"

PatchRef::Widget::Widget(Patch::TabWidget *tabWidget, const Package::Info* packageInfo, const QString& patchFileName)
   : QSplitter(tabWidget)
   , maxRef()
   , maxPatch()
   , tabWidget(tabWidget)
   , structureWidget(nullptr)
   , packageInfo(packageInfo)
   , path(patchFileName)
   , patchInfo{}
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
   PatchRef::Model::Header* headerPatchModel = new PatchRef::Model::Header(this, maxRef);
   headerPatchTree->init(this, headerPatchModel, 1);
   headerPatchTree->setItemDelegateForColumn(0, new Delegate::PatchType(this, headerPatchModel));

   PatchRef::Model::Argument* argumentPatchModel = new PatchRef::Model::Argument(this, maxRef);
   argumentPatchTree->init(this, argumentPatchModel);
   argumentPatchTree->setButtons(argumentAddButton, argumentRemoveButton);
   argumentPatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, argumentPatchModel));

   PatchRef::Model::TypedMessage* typedMessagPatcheModel = new PatchRef::Model::TypedMessage(this, maxRef);
   typedMessagePatchTree->init(this, typedMessagPatcheModel);

   PatchRef::Model::NamedMessage* namedMessagePatchModel = new PatchRef::Model::NamedMessage(this, maxRef);
   namedMessagePatchTree->init(this, namedMessagePatchModel);
   namedMessagePatchTree->setButtons(namedMessageAddButton, namedMessageRemoveButton);
   namedMessagePatchTree->setItemDelegateForColumn(1, new Delegate::DataType(this, namedMessagePatchModel));

   PatchRef::Model::Output* outputPatchModel = new PatchRef::Model::Output(this, maxRef);
   outputPatchTree->init(this, outputPatchModel);
   outputPatchTree->setButtons(outputAddButton, outputRemoveButton);

   // right: digest area
   new DescriptionHighlighter(descriptionEdit->document());
   descriptionIcon->setPixmap(QIcon(":/DocDescription.svg").pixmap(16, 16));

   connect(digestEdit, &QLineEdit::editingFinished, this, &Widget::slotSaveDigestText);
   connect(descriptionEdit, &QTextEdit::textChanged, this, &Widget::slotSaveDigestDescription);

   structureWidget = new Max::Widget(this);

   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->extractPatchInfo(path);
   propagateDirty(false);

   maxPatch.readPatch(path);
   structureWidget->load(maxPatch);

   maxRef.dirtyHook = std::bind(&Widget::setDirty, this);

   File::RefXML(packageInfo, maxRef).read(patchInfo);
   rebuild();
}

PatchRef::Widget::~Widget()
{
   tabWidget->emitSignalCheckDirty();
}

const QString& PatchRef::Widget::getPath() const
{
   return path;
}

const Package::Info* PatchRef::Widget::getPacakgeInfo() const
{
   return packageInfo;
}

const Patch::Info& PatchRef::Widget::getPatchInfo() const
{
   return patchInfo;
}

void PatchRef::Widget::writeRef()
{
   File::RefXML(packageInfo, maxRef).write(patchInfo);
   File::Help(packageInfo).write(patchInfo);
   File::Init(packageInfo, maxRef).write(patchInfo);
   propagateDirty(false);
}

void PatchRef::Widget::openInMax()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void PatchRef::Widget::openXML()
{
   const QString refPath = File::RefXML(packageInfo, maxRef).getFilePath(patchInfo);
   QDesktopServices::openUrl(QUrl::fromLocalFile(refPath));
}

bool PatchRef::Widget::isDirty() const
{
   return dirty;
}

void PatchRef::Widget::setToolsVisible(TabWidget::ToolsVisible toolsVisible)
{
   const bool showSuggestions = TabWidget::ToolVisibility::Suggestions & toolsVisible;
   //Suggest::TreeView::setAllVisible(showSuggestions);

   structureWidget->setVisible(TabWidget::ToolVisibility::Structure & toolsVisible);
}

void PatchRef::Widget::slotSetPatchDigest()
{
   setDigest(&maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void PatchRef::Widget::slotSaveDigestText()
{
   digest->text = digestEdit->text();
   setDirty();
}

void PatchRef::Widget::slotSaveDigestDescription()
{
   digest->description = descriptionEdit->toPlainText();
   setDirty();
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

   topicInfo->setText(maxRef.partName(part));
   digestEdit->setText(digest->text);

   descriptionEdit->blockSignals(true);
   descriptionEdit->setText(digest->description);
   descriptionEdit->blockSignals(false);
}

void PatchRef::Widget::rebuild()
{
   PatchRef::Model::Abstract::rebuildAll();
   //Suggest::Model::Abstract::rebuildAll();
   setDigest(&maxRef.header.digest, Ref::Structure::PatchPart::Header);
}

void PatchRef::Widget::update()
{
   PatchRef::Model::Abstract::updateAll();
}

void PatchRef::Widget::setDirty()
{
   propagateDirty(true);
   update();
}

void PatchRef::Widget::propagateDirty(bool isDirty)
{
   dirty = isDirty;
   tabWidget->emitSignalCheckDirty();
}

void PatchRef::Widget::setIcon(QLabel* iconLabel, Ref::Structure::PatchPart part)
{
   iconLabel->setPixmap(maxRef.partIcon(part).pixmap(16, 16));
}
