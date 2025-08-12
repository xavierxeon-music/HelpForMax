#include "PatchWidget.h"

#include <QDesktopServices>

#include "FileHelp.h"
#include "FileInit.h"
#include "FileRef.h"
#include "PackageInfo.h"

Patch::Widget::Widget(Patch::Container* container, const Package::Info* packageInfo, const QString& patchFileName)
   : QSplitter(container)
   , container(container)
   , stackWidget(nullptr)
   , refWidget(nullptr)
   , suggestWidget(nullptr)
   , structureWidget(nullptr)
   , maxRef()
   , maxPatch()
   , headerPatchModel(nullptr)
   , argumentPatchModel(nullptr)
   , typedMessagPatcheModel(nullptr)
   , namedMessagePatchModel(nullptr)
   , outputPatchModel(nullptr)
   , argumentSuggestModel(nullptr)
   , namedMessageSuggestModel(nullptr)
   , typedMessageSuggestModel(nullptr)
   , packageInfo(packageInfo)
   , path(patchFileName)
   , patchInfo{}
{
   // models
   headerPatchModel = new PatchRef::Model::Header(this, maxRef);
   argumentPatchModel = new PatchRef::Model::Argument(this, maxRef);
   typedMessagPatcheModel = new PatchRef::Model::TypedMessage(this, maxRef);
   namedMessagePatchModel = new PatchRef::Model::NamedMessage(this, maxRef);
   outputPatchModel = new PatchRef::Model::Output(this, maxRef);

   argumentSuggestModel = new Suggest::Model::Argument(this, maxRef, maxPatch);
   namedMessageSuggestModel = new Suggest::Model::NamedMessage(this, maxRef, maxPatch);
   typedMessageSuggestModel = new Suggest::Model::TypedMessage(this, maxRef, maxPatch);

   // gui
   stackWidget = new QStackedWidget(this);

   refWidget = new PatchRef::Widget(this);
   suggestWidget = new Suggest::Widget(this);
   stackWidget->addWidget(refWidget);
   stackWidget->addWidget(suggestWidget);

   structureWidget = new Max::Widget(this);

   addWidget(stackWidget);
   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->findPatchInfo(path);
   propagateDirty(false);

   maxPatch.readPatch(path);
   Suggest::Model::Abstract::callOnAllInstances(this, &Suggest::Model::Abstract::buildStructure);

   structureWidget->load(maxPatch);

   maxRef.dirtyHook = std::bind(&Widget::setDirty, this);

   File::RefXML(packageInfo, maxRef).read(patchInfo);

   refWidget->rebuild();
   suggestWidget->rebuild();
}

Patch::Widget::~Widget()
{
   container->emitSignalCheckDirty();
}

const QString& Patch::Widget::getPath() const
{
   return path;
}

const Package::Info* Patch::Widget::getPacakgeInfo() const
{
   return packageInfo;
}

const Patch::Info& Patch::Widget::getPatchInfo() const
{
   return patchInfo;
}

void Patch::Widget::writeRef()
{
   File::RefXML(packageInfo, maxRef).write(patchInfo);
   File::Help(packageInfo).write(patchInfo);
   File::Init(packageInfo, maxRef).write(patchInfo);
   propagateDirty(false);
}

void Patch::Widget::openInMax()
{
   QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void Patch::Widget::openXML()
{
   const QString refPath = File::RefXML(packageInfo, maxRef).getFilePath(patchInfo);
   QDesktopServices::openUrl(QUrl::fromLocalFile(refPath));
}

void Patch::Widget::tranferAllSuggestions()
{
   suggestWidget->slotTransferAll();
}

bool Patch::Widget::isDirty() const
{
   return dirty;
}

void Patch::Widget::setToolsVisible(Container::ToolsVisible toolsVisible)
{
   const bool showSuggestions = Container::ToolVisibility::Suggestions & toolsVisible;
   if (showSuggestions)
      stackWidget->setCurrentWidget(suggestWidget);
   else
      stackWidget->setCurrentWidget(refWidget);

   structureWidget->setVisible(Container::ToolVisibility::Structure & toolsVisible);
}

void Patch::Widget::setDirty()
{
   propagateDirty(true);
   refWidget->update();
   suggestWidget->update();
}

void Patch::Widget::propagateDirty(bool isDirty)
{
   dirty = isDirty;
   container->emitSignalCheckDirty();
}
