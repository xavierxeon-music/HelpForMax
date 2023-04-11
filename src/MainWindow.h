#ifndef MainWindowH
#define MainWindowH

#include <QSplitter>

#include "Tools/Central.h"

class MainWindow : public QSplitter,
                   public FunctionHub
{
   Q_OBJECT

public:
   MainWindow();

private:
   void setPackagePath(QString packageDir) override;
   void setModified(bool enabled, QString key) override;
   void closeEvent(QCloseEvent* ce) override;

private:
   Central central;
};

#endif // NOT MainWindowH
