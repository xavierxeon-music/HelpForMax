#ifndef BlockPatcherH
#define BlockPatcherH

#include "BlockStructure.h"

namespace Block
{
   class Item;

   class Patcher
   {
   public:
      Patcher(Item* item, const QString& patchPath);

   public:
      const QString& getPath() const;
      void read();

   private:
      Structure::Output& findOrCreateOutput(const int id);
      void readPatcherargs(QString text);

   private:
      Item* item;
      QString patchPath;
   };
} // namespace Block

#endif // NOT BlockPatcherH
