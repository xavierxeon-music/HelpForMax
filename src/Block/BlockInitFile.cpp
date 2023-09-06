#include "BlockInitFile.h"

#include <QFile>
#include <QTextStream>

#include "Tools/Central.h"

Block::InitFile::InitFile(Item* item)
   : item(item)
   , initPath()
{
   const QString packagePath = Central::getPackagePath();
   initPath = packagePath + "/init/" + item->key + ".txt";
}

void Block::InitFile::write()
{
   QFile file(initPath);

   if (item->patch.patcherType == Block::Structure::PatcherStandard) // delete file
   {
      if (!file.exists()) // nothing to delete
         return;

      file.remove();
   }
   else
   {
      if (!file.open(QIODevice::WriteOnly))
         return;

      QTextStream stream(&file);

      if (item->patch.patcherType == Block::Structure::PatcherGui)
      {
         stream << "max objectfile " << item->key << " " << item->key << ";\n";
         stream << "max definesubstitution " << item->key << " bpatcher @name " << item->key << ".maxpat;\n";
      }
      else if (item->patch.patcherType == Block::Structure::PatcherPoly)
      {
         stream << "max objectfile " << item->key << " " << item->key << ";\n";
         stream << "max definesubstitution " << item->key << " poly " << item->key << " 16;\n";
      }
      file.close();
   }
}
