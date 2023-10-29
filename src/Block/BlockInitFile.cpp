#include "BlockInitFile.h"

#include <QFile>
#include <QTextStream>

#include "Tools/Central.h"

Block::InitFile::InitFile(Block* block)
   : block(block)
   , initPath()
{
   const QString packagePath = Central::getPackagePath();
   initPath = packagePath + "/init/" + block->key + ".txt";
}

void Block::InitFile::write()
{
   QFile file(initPath);

   if (block->patch.patcherType == Structure::PatcherStandard) // delete file
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

      if (block->patch.patcherType == Structure::PatcherGui)
      {
         stream << "max objectfile " << block->key << " " << block->key << ";\n";
         stream << "max definesubstitution " << block->key << " bpatcher @name " << block->key << ".maxpat;\n";
      }
      else if (block->patch.patcherType == Structure::PatcherPoly)
      {
         stream << "max objectfile " << block->key << " " << block->key << ";\n";
         stream << "max definesubstitution " << block->key << " poly~ " << block->key << " 16;\n";
      }
      file.close();
   }
}
