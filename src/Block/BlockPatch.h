#ifndef BlockPatchH
#define BlockPatchH

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class Patch
   {
   public:
      Patch(Item* item, const QString& patchPath);

   public:
      const QString& getPath() const;
      void read();

   private:
      Structure::Output& findOrCreateOutput(const int id);

   private:
      Item* item;
      QString patchPath;
   };
} // namespace Block

#endif // NOT BlockPatchH
