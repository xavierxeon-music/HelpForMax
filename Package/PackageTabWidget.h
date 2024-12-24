#ifndef PackageTabWidgetH
#define PackageTabWidgetH

#include "RecentTabWidget.h"
#include <QTabWidget>

#include <QLocalServer>
#include <QLocalSocket>
#include <QMenu>
#include <QPointer>
#include <QToolBar>

namespace Package
{
   class Info;

   class TabWidget : public RecentTabWidget
   {
      Q_OBJECT

   public:
      TabWidget(QWidget* parent);
      ~TabWidget();

   signals:
      void signalCloseAllPatches(const Info* info);
      void signalPatchSeleted(const QString& path, const Info* info);

   public slots:
      void slotRefWritten(const QString& patchPath);
      void slotCheckDirty();

   public:
      static Info* findOrCreate(const QString& someFileInPackage);
      void createActions();
      void init();

   private slots:
      void slotLoadPackage();
      void slotClosePackage();
      void slotNewConnection();
      void slotReceiveSocket();
      void slotLinkToMax(bool enabled);

   private:
      Info* get(const QString& packagePath);
      Entry creatreEntry(const QFileInfo& fileInfo) override;
      void updateCurrentList();

   private:
      static TabWidget* me;
      QMap<QString, Info*> packageInfoBuffer;
      QLocalServer* server;
      QPointer<QLocalSocket> socket;

      bool linkEnabled;
      QAction* linkAction;
      QMap<bool, QIcon> linkMap;
   };
} // namespace Package

#endif // NOT PackageTabWidgetH
