#ifndef FileInitH
#define FileInitH

#include "FileAbstract.h"

namespace File
{
   class Init : public Abstract
   {
   public:
      Init(const Package::Info* info, Ref::Structure& structure);

   public:
      void write(const Patch::Info& patchInfo) override;
      QString getFilePath(const Patch::Info& patchInfo) override;
   };
} // namespace File

#endif // NOT FileInitH
