#include "PatchContainer.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QTabBar>
#include <QTimer>

#include <XXLogger.h>
#include <XXPopulatedMainWindow.h>
#include <XXSettings.h>

#include "PackageContainer.h"
#include "PackageInfo.h"
#include "PatchWidget.h"

Patch::Container::Container(QWidget* parent)
   : RecentTabWidget(parent, "Patch")
   , toolsVisible(ToolVisibility::None)
   , splitterSizes()
{
   {
      XX::Settings settings;

      toolsVisible = settings.value("Patch/Tools", static_cast<int>(ToolVisibility::None)).value<ToolsVisible>();

      const QJsonArray splitterWidth = settings.jsonValue("Patch/Splitter").toArray();
      for (int index = 0; index < splitterWidth.size(); index++)
      {
         const int value = splitterWidth.at(index).toInt();
         splitterSizes.append(value);
      }
   }
}

void Patch::Container::createActions()
{
   //
   XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchLoad.svg"), "Load", "Patch.Load", this, &Container::slotPromptLoadPatch);

   QAction* saveAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchSave.svg"), "Save", "Patch.Save", this, &Container::slotWriteRef);
   saveAction->setShortcut(QKeySequence::Save);

   QAction* saveAllAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchSaveAll.svg"), "Save All", "Patch.SaveAll", this, &Container::slotWriteAllRefs);
   saveAllAction->setShortcut(Qt::ShiftModifier | Qt::ControlModifier | Qt::Key_S);

   QAction* closeAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchClose.svg"), "Close", "Patch.Close", this, &Container::slotClosePatch);
   closeAction->setShortcut(QKeySequence::Close);

   //
   QAction* suggestAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchSuggest.svg"), "Suggestions", "Patch.ShowSuggesions", this, &Container::slotShowSuggestions);
   suggestAction->setCheckable(true);
   suggestAction->setChecked(toolsVisible & ToolVisibility::Suggestions);
   suggestAction->setShortcut(QKeySequence::Find);

   QAction* structureAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/OverviewGeneral.svg"), "Structure", "Patch.ShowStructure", this, &Container::slotShowStructure);
   structureAction->setCheckable(true);
   structureAction->setChecked(toolsVisible & ToolVisibility::Structure);
   structureAction->setShortcut(QKeySequence::Print);

   QAction* openInMaxAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchOpenInMax.svg"), "Open In Max", "Patch.Max", this, &Container::slotOpenInMax);
   openInMaxAction->setShortcuts(QKeySequence::Italic);

   QAction* showXMLAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchOpenRef.svg"), "Open XML", "Patch.XML", this, &Container::slotOpenXML);
   showXMLAction->setShortcut(QKeySequence::Open);

   QAction* transferAllAction = XX::Populated::Abstract::addAction(QIcon(":/Icons/PatchTransferAll.svg"), "Use All Suggestions", "Patch.TransferAllSuggestions", this, &Container::slotTranferAllSuggestions);
   //transferAllAction->setShortcut(Qt::ShiftModifier | Qt::ControlModifier | Qt::Key_T);
   transferAllAction->setShortcut(Qt::ControlModifier | Qt::Key_T);
}

void Patch::Container::init()
{
   for (const QString& patchPath : getCurrrentFiles())
   {
      slotShowPatch(patchPath);
   }
}

void Patch::Container::emitSignalCheckDirty()
{
   QTimer::singleShot(200, this, &Container::updateTabNames);
}

void Patch::Container::slotPromptLoadPatch()
{
   const QString patchFileName = QFileDialog::getOpenFileName(this, "MAX PATCH", QString(), "*.maxpat");
   if (patchFileName.isEmpty())
      return;

   slotShowPatch(patchFileName);
}

void Patch::Container::slotShowPatch(const QString& patchFileName)
{
   Package::Info* info = Package::Container::findOrCreate(patchFileName);
   if (!info)
   {
      XX::Logger::stream(Qt::red) << "PATCH does not belong to a package" << patchFileName;
      this->deleteLater();
      return;
   }

   const Patch::Info patchInfo = info->findPatchInfo(patchFileName);
   for (int index = 0; index < tabBar()->count(); index++)
   {
      if (patchInfo.getName() == tabText(index))
      {
         setCurrentIndex(index);
         return;
      }
   }

   Widget* patchWidget = new Patch::Widget(this, info, patchFileName);
   if (!splitterSizes.isEmpty())
      patchWidget->setSizes(splitterSizes);

   patchWidget->setToolsVisible(toolsVisible);

   connect(patchWidget, &QSplitter::splitterMoved, this, &Container::slotTabSplitterChanged);

   const int index = addTab(patchWidget, patchInfo.getName());
   setCurrentIndex(index);

   addRecentFile(patchFileName);
   addCurrentFile(patchFileName);
}

void Patch::Container::slotWriteRef()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   emit signalRefWritten(patchWidget->getPath());
   patchWidget->writeRef();
}

void Patch::Container::slotWriteAllRefs()
{
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (!patchWidget)
         continue;

      patchWidget->writeRef();
      emit signalRefWritten(patchWidget->getPath());
   }
}

void Patch::Container::slotClosePatch()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   removeCurrentFile(patchWidget->getPath());
   patchWidget->deleteLater();
}

void Patch::Container::slotCloseAllPatches(const Package::Info* packageInfo)
{
   QList<Widget*> deleteList;
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (packageInfo && packageInfo != patchWidget->getPacakgeInfo())
         continue;

      deleteList.append(patchWidget);
   }

   for (Widget* patchWidget : deleteList)
   {
      removeCurrentFile(patchWidget->getPath());
      patchWidget->deleteLater();
   }
}

void Patch::Container::slotOpenInMax()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openInMax();
}

void Patch::Container::slotOpenXML()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openXML();
}

void Patch::Container::slotTranferAllSuggestions()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->tranferAllSuggestions();
}

void Patch::Container::slotShowSuggestions(bool enabled)
{
   toggleVisibility(enabled, ToolVisibility::Suggestions);
   writeSettings();
}

void Patch::Container::slotShowStructure(bool enabled)
{
   toggleVisibility(enabled, ToolVisibility::Structure);
   writeSettings();
}

void Patch::Container::slotTabSplitterChanged()
{
   if (SplitterLocker::engaged())
      return;

   SplitterLocker locker;

   Widget* sourcePatchWidget = qobject_cast<Widget*>(sender());
   if (!sourcePatchWidget)
      return;

   splitterSizes = sourcePatchWidget->sizes();
   writeSettings();

   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (!patchWidget)
         continue;

      if (patchWidget == sourcePatchWidget)
         continue;

      patchWidget->setSizes(splitterSizes);
   }
}

RecentTabWidget::Entry Patch::Container::creatreEntry(const QFileInfo& fileInfo)
{
   const QString patchName = fileInfo.completeBaseName();
   auto openFunction = std::bind(&Container::slotShowPatch, this, fileInfo.absoluteFilePath());

   Entry entry{patchName, openFunction};
   return entry;
}

void Patch::Container::updateTabNames()
{
   static const QString bullet = QString::fromUtf8("\u25cf") + QString(" ");

   for (int index = 0; index < tabBar()->count(); index++)
   {
      QWidget* content = widget(index);
      Widget* patchWidget = qobject_cast<Widget*>(content);
      if (!patchWidget)
         continue;

      QString name = patchWidget->getPatchInfo().getName();
      if (patchWidget->isDirty())
         name = bullet + name;

      setTabText(index, name);
   }

   emit signalCheckDirty();
}

void Patch::Container::toggleVisibility(bool enabled, const ToolVisibility& value)
{
   if (enabled)
      toolsVisible |= value;
   else
   {
      toolsVisible &= ~(int)value;
   }

   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (!patchWidget)
         continue;

      patchWidget->setToolsVisible(toolsVisible);
   }
}

void Patch::Container::writeSettings()
{
   XX::Settings settings;

   settings.setValue("Patch/Tools", static_cast<int>(toolsVisible));

   QJsonArray splitterWidth;
   for (int index = 0; index < splitterSizes.count(); index++)
      splitterWidth.append(splitterSizes.at(index));

   settings.setJsonValue("Patch/Splitter", splitterWidth);
}
