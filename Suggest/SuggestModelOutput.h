#ifndef SuggestModelOutputH
#define SuggestModelOutputH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class Output : public Abstract
      {
         Q_OBJECT

      public:
         Output(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest);

      private:
         void update() override;
         void rebuild() override;
         void buildStructure() override;
         void transfer(const QList<int>& rowList) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelOutputH
