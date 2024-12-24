#ifndef SuggestModelTypedMessageH
#define SuggestModelTypedMessageH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class TypedMessage : public Abstract
      {
         Q_OBJECT

      public:
         TypedMessage(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest);

      private:
         void update() override;
         void rebuild() override;
         void transfer(const QList<int>& rowList) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelTypedMessageH
