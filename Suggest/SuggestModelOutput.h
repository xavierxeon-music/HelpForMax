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
         Output(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest);

      private:
         void rebuild() override;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelOutputH
