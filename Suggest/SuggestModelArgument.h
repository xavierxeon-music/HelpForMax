#ifndef SuggestModelArgumentH
#define SuggestModelArgumentH

#include "SuggestModelAbstract.h"

namespace Suggest
{
   namespace Model
   {
      class Argument : public Abstract
      {
         Q_OBJECT

      public:
         Argument(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest);

      private:
         void update() override;
         void rebuild() override;
         void buildStructure() override;
         void transfer(const QList<int>& rowList) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelArgumentH
