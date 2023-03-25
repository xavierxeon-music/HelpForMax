#ifndef BlockHelpH
#define BlockHelpH

namespace Block
{
   class Item;

   class Help
   {
   public:
      Help(Item* item);

   private:
      Item* item;
   };
} // namespace Block

#endif // NOT BlockHelpH
