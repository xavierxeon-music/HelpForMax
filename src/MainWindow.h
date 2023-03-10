#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include <Central.h>

class MainWindow : public QMainWindow,
                   public Central
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotOpenPackage();
   void slotSavePatches();

private:
   void setPackagePath(QString packageDir) override;
   void setModified(bool enabled) override;
   void closeEvent(QCloseEvent* ce) override;
};

#endif // NOT MainWindowH
