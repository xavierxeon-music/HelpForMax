#ifndef BlockSettingsH
#define BlockSettingsH

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class Settings
   {
   public:
      Settings(Item* item);

   public:
      void write();

   private:
      Item* item;
      QString initPath;
   };
} // namespace Block

#endif // NOT BlockSettingsH
