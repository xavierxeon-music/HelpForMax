#ifndef FileHelpH
#define FileHelpH

#include "FileAbstract.h"

namespace File
{
   class Help : public Abstract
   {
   public:
      Help(const Package::Info* info);

   public:
      void write(const Patch::Info& patchInfo) override;
      QString getFilePath(const Patch::Info& patchInfo) override;
   };
} // namespace File

#endif // NOT FileHelpH
