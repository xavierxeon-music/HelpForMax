#ifndef PatchInfoH
#define PatchInfoH

#include <QString>

namespace Patch
{
   struct Info
   {
      QString patchPath;
      QString sourcePath;
      QString refPath;
      QString helpPath;
      QString initPath;

      using Map = QMap<QString, Info>;

      bool isOrphan() const;
      QString getName() const;
      QString getFolder() const;
   };
} // namespace Patch

#endif // NOT PatchInfoH
