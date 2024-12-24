#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "RefModel.h"
#include "RefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public ListedRefModel<"PatchRef">
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure& suggest, const Ref::Structure::PatchPart& part);

      signals:
         void signalDataEdited();

      public:
         virtual void transfer(const QList<int>& rowList) = 0;

      protected:
         const Ref::Structure& suggest;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
