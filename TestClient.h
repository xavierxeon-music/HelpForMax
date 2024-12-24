#ifndef TestClientH
#define TestClientH

#include "_Forms/ui_TestClient.h"
#include <QDialog>

#include <QLocalSocket>

class TestClient : public QDialog, private Ui::TestClient
{
   Q_OBJECT

public:
   TestClient();

private slots:
   void slotSelectItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);

private:
   void sendData(const QString& patchPath);
   void receiveData(QLocalSocket* socket);

private:
   QLocalSocket* socket;
};

#endif // NOT TestClientH
