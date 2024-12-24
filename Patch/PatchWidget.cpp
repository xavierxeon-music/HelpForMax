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
   , packageInfo(packageInfo)
   , path(patchFileName)
   , patchInfo{}
{
   stackWidget = new QStackedWidget(this);

   refWidget = new PatchRef::Widget(this);
   suggestWidget = new Suggest::Widget(this);
   stackWidget->addWidget(refWidget);
   stackWidget->addWidget(suggestWidget);

   structureWidget = new Max::Widget(this);

   addWidget(stackWidget);
   addWidget(structureWidget);

   // load content
   patchInfo = packageInfo->extractPatchInfo(path);
   propagateDirty(false);

   maxPatch.readPatch(path);
   structureWidget->load(maxPatch);

   maxRef.dirtyHook = std::bind(&Widget::setDirty, this);

   File::RefXML(packageInfo, maxRef).read(patchInfo);

   refWidget->rebuild();
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
}

void Patch::Widget::propagateDirty(bool isDirty)
{
   dirty = isDirty;
   container->emitSignalCheckDirty();
}
