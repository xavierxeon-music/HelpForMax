#include "PackageContainer.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include <Logger.h>
#include <PopulatedMainWindow.h>
#include <Shared.h>

#include "PackageCleanDialog.h"
#include "PackageInfo.h"
#include "PackageWidget.h"
#include "SuggestTransferDialog.h"

using HelpForMax = Shared<"HelpForMax">;

Package::Container* Package::Container::me = nullptr;

Package::Container::Container(QWidget* parent)
   : RecentTabWidget(parent, "Package")
   , packageInfoBuffer()
   , server(nullptr)
   , socket()
   , linkEnabled(false)
   , linkAction(nullptr)
   , linkMap()
{
   me = this;
   setMinimumWidth(200);

   linkMap[true] = QIcon(":/MaxLinkActive.svg");
   linkMap[false] = QIcon(":/MaxLinkInactive.svg");

   server = new QLocalServer(this);
   connect(server, &QLocalServer::newConnection, this, &Container::slotNewConnection);

   const QString serverName = HelpForMax::socketName();
   server->removeServer(serverName);
   server->listen(serverName);

   qInfo() << "Server @" << serverName;
}

Package::Container::~Container()
{
   me = nullptr;
}

void Package::Container::slotRefWritten(const QString& patchPath)
{
   if (socket.isNull())
      return;

   QJsonObject object;
   object["patch"] = patchPath;

   QJsonDocument doc(object);
   socket->write(doc.toJson(QJsonDocument::Compact));
}

void Package::Container::slotCheckDirty()
{
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* packageWidget = qobject_cast<Widget*>(widget(index));
      if (!packageWidget)
         continue;

      packageWidget->checkUpToDate();
   }
}

Package::Info* Package::Container::findOrCreate(const QString& someFileInPackage)
{
   if (!me)
      return nullptr;

   QFileInfo patchInfo(someFileInPackage);

   if (!patchInfo.exists())
      return nullptr;

   QString path = [&]()
   {
      for (QDir dir = patchInfo.dir(); !dir.isRoot(); dir.cdUp())
      {
         const QFileInfoList content = dir.entryInfoList(QDir::Files);
         for (const QFileInfo& contentInfo : content)
         {
            if ("package-info.json" != contentInfo.fileName())
               continue;

            return dir.absolutePath();
         }
      }

      return QString();
   }();

   if (path.isEmpty())
      return nullptr;

   return me->get(path);
}

void Package::Container::createActions()
{
   //
   PopulatedMainWindow::addAction(QIcon(":/PackageLoad.svg"), "Load", "Package.Load", this, &Container::slotLoadPackage);
   PopulatedMainWindow::addAction(QIcon(":/PackageClose.svg"), "Close", "Package.Close", this, &Container::slotClosePackage);

   PopulatedMainWindow::addAction(QIcon(), "Suggestions", "Package.OpenSuggestionsDialog", this, &Container::slotOpenSuggestions);
   PopulatedMainWindow::addAction(QIcon(), "Clean", "Package.Clean", this, &Container::slotCleanup);

   linkAction = PopulatedMainWindow::addAction(linkMap.value(false), "Link", "Package.Link", this, &Container::slotLinkToMax);
   linkAction->setCheckable(true);

   QSettings settings;
   linkEnabled = settings.value("Package/Link").toBool();
   linkAction->setChecked(linkEnabled);
}

void Package::Container::init()
{
   for (const QString& packagePath : getCurrrentFiles())
   {
      findOrCreate(packagePath + "/");
   }
}

Package::Info* Package::Container::get(const QString& packagePath)
{
   if (packageInfoBuffer.contains(packagePath))
      return packageInfoBuffer.value(packagePath);

   QFile file(packagePath + "/package-info.json");
   if (!file.open(QIODevice::ReadOnly))
      return nullptr;

   Info* info = new Info(packagePath);
   packageInfoBuffer[packagePath] = info;

   const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   file.close();

   const QJsonObject object = doc.object();
   if (!object.empty())
   {
      info->author = object["author"].toString();
      info->name = object["name"].toString();

      Logger::stream() << "LOADED PACKAGE " << info->name;
   }

   Widget* view = new Widget(this, info);
   connect(view, &Widget::signalPatchSeleted, this, &Container::signalPatchSeleted);

   addTab(view, info->name);
   addRecentFile(info->path);
   addCurrentFile(info->path);

   return info;
}

void Package::Container::slotLoadPackage()
{
   const QString packageFileName = QFileDialog::getOpenFileName(this, "MAX PACKAGE", QString(), "package-info.json");
   if (packageFileName.isEmpty())
      return;

   findOrCreate(packageFileName);
}

void Package::Container::slotClosePackage()
{
   Widget* view = qobject_cast<Widget*>(currentWidget());
   const Info* info = view->getPackageInfo();
   view->deleteLater();

   removeCurrentFile(info->path);
   emit signalCloseAllPatches(info);
}

void Package::Container::slotNewConnection()
{
   socket = server->nextPendingConnection();

   auto setLinkActive = [this](bool active)
   {
      QIcon icon = this->linkMap.value(active);
      this->linkAction->setIcon(icon);
   };

   auto inactiveFunction = std::bind(setLinkActive, false);
   connect(socket, &QLocalSocket::disconnected, inactiveFunction);

   connect(socket, &QIODevice::readyRead, this, &Container::slotReceiveSocket);

   setLinkActive(true);
}

void Package::Container::slotReceiveSocket()
{
   if (!linkEnabled)
      return;

   const QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
   const QJsonObject object = doc.object();

   const QString path = object["patch"].toString();
   Info* info = findOrCreate(path);
   if (!info)
   {
      Logger::stream(Qt::red) << "Unable to locate package of " << path;
      return;
   }

   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* view = qobject_cast<Widget*>(widget(index));
      if (view->getPackageInfo() != info)
         continue;

      setCurrentIndex(index);
      view->selectPatch(path);
      break;
   }
}

void Package::Container::slotLinkToMax(bool enabled)
{
   linkEnabled = enabled;

   QSettings settings;
   settings.setValue("Package/Link", linkEnabled);
}

void Package::Container::slotOpenSuggestions()
{
   Widget* view = qobject_cast<Widget*>(currentWidget());
   const Info* info = view->getPackageInfo();

   Suggest::Transfer::Dialog dialog(this, info);
   dialog.exec();
}

void Package::Container::slotCleanup()
{
   Widget* view = qobject_cast<Widget*>(currentWidget());
   const Info* info = view->getPackageInfo();

   Clean::Dialog dialog(this, info);
   dialog.exec();
}

RecentTabWidget::Entry Package::Container::creatreEntry(const QFileInfo& fileInfo)
{
   const QString name = fileInfo.baseName();
   auto openFunction = [this, fileInfo]()
   {
      const QString packagePath = fileInfo.absoluteFilePath() + "/";
      this->findOrCreate(packagePath);
   };

   Entry entry{name, openFunction};
   return entry;
}
