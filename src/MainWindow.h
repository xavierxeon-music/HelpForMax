#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include <Central.h>

class SelectModel;
class ComponentsModel;

class MainWindow : public QMainWindow,
                   public Central
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotOpenPackage();
   void slotReload();
   void slotSave();

private:
   void setPackagePath(QString packageDir) override;
   void setModified(bool enabled) override;
   void closeEvent(QCloseEvent* ce) override;

private:
   SelectModel* selectModel;
   ComponentsModel* componentsModel;
};

#endif // NOT MainWindowH
