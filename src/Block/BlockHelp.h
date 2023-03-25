#ifndef BlockHelpH
#define BlockHelpH

#include <QString>

namespace Block
{
   class Item;

   class Help
   {
   public:
      Help(Item* item);

   public:
      void write();

   private:
      Item* item;
      QString helpPath;
   };
} // namespace Block

#endif // NOT BlockHelpH
