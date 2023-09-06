#ifndef BlockInitFileH
#define BlockInitFileH

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class InitFile
   {
   public:
      InitFile(Item* item);

   public:
      void write();

   private:
      Item* item;
      QString initPath;
   };
} // namespace Block

#endif // NOT BlockInitFileH
