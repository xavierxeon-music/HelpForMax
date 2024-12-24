#include "RecentTabWidget.h"

#include <QMenu>
#include <QSettings>

RecentTabWidget::RecentTabWidget(QWidget* parent, const QString& settingsKey)
   : QTabWidget(parent)
   , settingsKey(settingsKey)
   , currentFileList()
   , recentFileList()
   , recentMenu(nullptr)
{
   setDocumentMode(true);

   recentMenu = new QMenu("Recent", parent);
   recentMenu->setObjectName(settingsKey + ".Recent");

   auto populateFunction = std::bind(&RecentTabWidget::populateRecentMenu, this);
   QObject::connect(recentMenu, &QMenu::aboutToShow, populateFunction);

   QSettings settings;
   currentFileList = settings.value(settingsKey + "/Current").toStringList();
   recentFileList = settings.value(settingsKey + "/Recent").toStringList();
}

RecentTabWidget::~RecentTabWidget()
{
   QSettings settings;

   settings.setValue(settingsKey + "/Current", currentFileList);
   settings.setValue(settingsKey + "/Recent", recentFileList);
}

QMenu* RecentTabWidget::getRecentMenu()
{
   return recentMenu;
}

const QStringList& RecentTabWidget::getCurrrentFiles() const
{
   return currentFileList;
}

void RecentTabWidget::addCurrentFile(const QString& fileName)
{
   if (!currentFileList.contains(fileName))
      currentFileList.append(fileName);
}

void RecentTabWidget::removeCurrentFile(const QString& fileName)
{
   currentFileList.removeAll(fileName);
}

void RecentTabWidget::addRecentFile(const QString& fileName)
{
   if (!recentFileList.contains(fileName))
      recentFileList.append(fileName);

   while (recentFileList.size() > 10)
      recentFileList.takeFirst();
}

void RecentTabWidget::populateRecentMenu()
{
   recentMenu->clear();
   for (const QString& fileName : recentFileList)
   {
      QFileInfo fileInfo(fileName);
      Entry entry = creatreEntry(fileInfo);

      recentMenu->addAction(entry.name, entry.openFunction);
   }

   auto clearFunction = [this]
   {
      this->recentFileList.clear();
   };

   recentMenu->addSeparator();
   recentMenu->addAction("Clear Recent", clearFunction);
}
