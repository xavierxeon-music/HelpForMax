#ifndef SuggestModelNamedMessageH
#define SuggestModelNamedMessageH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class NamedMessage : public Abstract
      {
         Q_OBJECT

      public:
         NamedMessage(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest);

      private:
         void update() override;
         void rebuild() override;
         void buildStructure() override;
         void transfer(const QList<int>& rowList) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelNamedMessageH
