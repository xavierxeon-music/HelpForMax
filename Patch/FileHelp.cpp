#include "FileHelp.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "Package/PackageInfo.h"

static Ref::Structure dummy;

File::Help::Help(const Package::Info* info)
   : Abstract(info, dummy)
{
}

void File::Help::write(const Patch::Info& patchInfo)
{
   QFile file(getFilePath(patchInfo));
   if (file.exists())
      return;

   if (!file.open(QIODevice::WriteOnly))
      return;

   static const QByteArray content = []()
   {
      QJsonObject patcher;
      patcher["classnamespace"] = "box";
      patcher["description"] = "";
      patcher["digest"] = "";
      patcher["tags"] = "";
      patcher["style"] = "";
      patcher["boxes"] = QJsonArray();
      patcher["lines"] = QJsonArray();
      patcher["assistshowspatchername"] = 0;
      patcher["dependency_cache"] = QJsonArray();
      patcher["autosave"] = 0;

      QJsonObject helpData;
      helpData["patcher"] = patcher;

      QJsonDocument doc(helpData);
      const QByteArray data = doc.toJson();

      return data;
   }();

   file.write(content);
   file.close();
}

QString File::Help::getFilePath(const Patch::Info& patchInfo)
{
   QString helpPath = info->getPath() + "/help/";
   if (!patchInfo.folder.isEmpty())
      helpPath += patchInfo.folder + "/";
   helpPath += patchInfo.name + ".maxhelp";

   return helpPath;
}
