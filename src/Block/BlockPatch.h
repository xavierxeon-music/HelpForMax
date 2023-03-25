#ifndef BlockPatchH
#define BlockPatchH

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class Patch
   {
   public:
      Patch(Item* item);

   public:
      void addJSON(const QString& patchPath);

   private:
      Structure::Output& findOrCreateOutput(const int id);

   private:
      Item* item;
   };
} // namespace Block

#endif // NOT BlockPatchH
