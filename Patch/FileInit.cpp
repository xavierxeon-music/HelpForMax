#include "FileInit.h"

#include "Package/PackageInfo.h"

File::Init::Init(const Package::Info* info, Ref::Structure& structure)
   : Abstract(info, structure)
{
}

void File::Init::write(const Patch::Info& patchInfo)
{
   const QString initPath = getFilePath(patchInfo);

   QFile file(initPath);

   if (structure.header.patcherType == Ref::Structure::PatchType::Standard) // delete file
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

      if (structure.header.patcherType == Ref::Structure::PatchType::Gui)
      {
         stream << "max objectfile " << patchInfo.name << " " << patchInfo.name << ";\n";
         stream << "max definesubstitution " << patchInfo.name << " bpatcher @name " << patchInfo.name << ".maxpat;\n";
      }
      else if (structure.header.patcherType == Ref::Structure::PatchType::Poly)
      {
         stream << "max objectfile " << patchInfo.name << " " << patchInfo.name << ";\n";
         stream << "max definesubstitution " << patchInfo.name << " poly~ " << patchInfo.name << " 16;\n";
      }
      file.close();
   }
}

QString File::Init::getFilePath(const Patch::Info& patchInfo)
{
   // do not use folder here!
   QString initPath = info->getPath() + "/init/";
   initPath += patchInfo.name + ".txt";

   return initPath;
}
