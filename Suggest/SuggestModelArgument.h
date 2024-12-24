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
         Argument(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest);

      private:
         void rebuild() override;
         void transfer(const QList<int>& rowList) override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelArgumentH
