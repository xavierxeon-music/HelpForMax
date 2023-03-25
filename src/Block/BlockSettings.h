#ifndef BlockSettingsH
#define BlockSettingsH

namespace Block
{
   class Item;

   class Settings
   {
   public:
      Settings(Item* item);

   private:
      Item* item;
   };
} // namespace Block

#endif // NOT BlockSettingsH
