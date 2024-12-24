#ifndef SuggestModelAbstractH
#define SuggestModelAbstractH

#include <QStandardItemModel>

#include "MaxPatcher.h"
#include "RefModel.h"
#include "RefStructure.h"

namespace Suggest
{
   namespace Model
   {
      class Abstract : public ListedRefModel<"Suggest">
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, Max::Patcher& suggest, const Ref::Structure::PatchPart& part);

      signals:
         void signalDataEdited();

      public:
         virtual void transfer(const QList<int>& rowList) = 0;
         virtual void buildStructure() = 0;

      protected:
         Max::Patcher& suggest;
      };
   } // namespace Model
} // namespace Suggest

#endif // NOT SuggestModelAbstractH
