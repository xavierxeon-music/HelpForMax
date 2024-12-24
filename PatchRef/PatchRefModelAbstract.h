#ifndef PatchRefModelAbstractH
#define PatchRefModelAbstractH

#include <QStandardItemModel>

#include "RefModel.h"
#include "RefStructure.h"

namespace PatchRef
{
   namespace Model
   {
      class Abstract : public ListedRefModel<"PatchRef">
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part);

      signals:
         void signalDataEdited();
         void signalUpdateDigest(const QModelIndex& index);

      public:
         virtual Ref::Structure::Digest* getDigest(const QModelIndex& index) = 0;
         virtual void createBeforeItem(const QModelIndex& index);
         virtual void removeItem(const QModelIndex& index);

      protected:
         void updateDigestItem(QStandardItem* digestItem, const Ref::Structure::Digest& digest);
      };
   } // namespace Model
} // namespace PatchRef

#endif // NOT PatchRefModelAbstractH
