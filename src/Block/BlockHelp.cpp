#include "BlockHelp.h"

#include <QFile>
#include <QJsonArray>

#include "Tools/Central.h"
#include "Tools/JSONModel.h"

Block::Help::Help(Block* block)
   : block(block)
   , helpPath()
{
   const QString packagePath = Central::getPackagePath();
   helpPath = packagePath + "/help/" + block->key + ".maxhelp";
}

void Block::Help::write()
{
   if (QFile::exists(helpPath))
      return;

   static const QJsonObject helpData = []()
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

      return helpData;
   }();

   JSON::toFile(helpPath, helpData);
}
