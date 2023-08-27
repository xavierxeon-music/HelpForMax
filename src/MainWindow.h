#ifndef MainWindowH
#define MainWindowH

#include <QWidget>

#include <QSplitter>

#include "Tools/Central.h"

class MainWindow : public QWidget,
                   public FunctionHub
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotWriteAllInit();

private:
   void setPackagePath(QString packageDir) override;
   void setModified(bool enabled, QString key) override;
   void closeEvent(QCloseEvent* ce) override;

private:
   QSplitter* splitter;
   Central central;
};

#endif // NOT MainWindowH
