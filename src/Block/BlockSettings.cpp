#include "BlockSettings.h"

#include <QFile>
#include <QTextStream>

#include "Tools/Central.h"

Block::Settings::Settings(Item* item)
   : item(item)
   , initPath()
{
   const QString packagePath = Central::getPackagePath();
   initPath = packagePath + "/init/" + item->key + ".txt";
}

void Block::Settings::write()
{
   auto writeFile = [&]()
   {
      if (item->extras.openAsBPatcher)
         return true;

      return false;
   };

   QFile file(initPath);

   if (!writeFile()) // delete file
   {
      if (!file.exists()) // nothing to delete
         return;

      file.remove();
   }
   else
   {
      if (!file.open(QIODevice::WriteOnly))
         return;

      {
         QTextStream stream(&file);

         stream << "max objectfile " << item->key << " " << item->key << ";\n";
         stream << "max definesubstitution " << item->key << " bpatcher @name " << item->key << ".maxpat;\n";
      }
      file.close();
   }
}
