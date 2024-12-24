#ifndef FileAbstractH
#define FileAbstractH

#include "RefStructure.h"
#include "PatchInfo.h"

namespace Package
{
   class Info;
}

namespace File
{
   class Abstract
   {
   public:
      Abstract(const Package::Info* info, Ref::Structure& structure);

   public:
      virtual void read(const Patch::Info& patchInfo);
      virtual void write(const Patch::Info& patchInfo);
      virtual QString getFilePath(const Patch::Info& patchInfo) = 0;

   protected:
      const Package::Info* info;
      Ref::Structure& structure;
   };
} // namespace File

#endif // NOT FileAbstractH
