#include "TestClient.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <Shared.h>

using HelpForMax = Shared<"HelpForMax">;

TestClient::TestClient()
   : QDialog(nullptr)
   , socket(nullptr)
{
   setupUi(this);

   selectTree->setHeaderLabels({"Patch"});
   selectTree->setRootIsDecorated(false);

   auto addItem = [&](const QString& patchPath)
   {
      QFileInfo patchInfo(patchPath);
      const QString patchName = patchInfo.fileName().replace(".maxpat", "");

      QTreeWidgetItem* item = new QTreeWidgetItem(selectTree);
      item->setText(0, patchName);
      item->setData(0, Qt::UserRole + 1, patchPath);
   };

#ifdef Q_OS_WIN
   const QFileInfo packageInfo("C:/dev/GitHub/MusicProjects/WaMax");
#else
   const QFileInfo packageInfo(QDir::homePath() + "/GitHub/MusicProjects/WaMax");
#endif

   const QString packageDir = packageInfo.canonicalFilePath();

   addItem(packageDir + "/patchers/audio/wa.wave_terrain~.maxpat");
   addItem(packageDir + "/patchers/oscilators/wa.ringbuffer.granular_mk1~.maxpat");
   addItem(packageDir + "/patchers/oscilators/wa.sound.image~.maxpat");

   connect(selectTree, &QTreeWidget::currentItemChanged, this, &TestClient::slotSelectItemChanged);
}

void TestClient::slotSelectItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
   Q_UNUSED(previous)

   const QString path = current->data(0, Qt::UserRole + 1).toString();

   sendData(path);
}

void TestClient::sendData(const QString& patchPath)
{
   if (!socket)
   {
      socket = new QLocalSocket(this);

      auto receiveFunction = std::bind(&TestClient::receiveData, this, socket);
      connect(socket, &QLocalSocket::readyRead, receiveFunction);

      socket->connectToServer(HelpForMax::socketName());
      socket->waitForConnected();
   }

   QJsonObject object;
   object["patch"] = patchPath;

   QJsonDocument doc(object);
   socket->write(doc.toJson(QJsonDocument::Compact));
}

void TestClient::receiveData(QLocalSocket* socket)
{
   const QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
   const QJsonObject object = doc.object();

   //const QString path = object["path"].toString();

   resultEdit->append(doc.toJson());
}
