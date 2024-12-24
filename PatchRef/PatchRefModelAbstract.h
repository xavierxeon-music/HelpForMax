#ifndef PatchRefModelAbstractH
#define PatchRefModelAbstractH

#include <QStandardItemModel>

#include "RefStructure.h"

namespace PatchRef
{
   namespace Model
   {
      class Abstract : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Abstract(QObject* parent, Ref::Structure& structure, const Ref::Structure::PatchPart& part);
         ~Abstract();

      signals:
         void signalDataEdited();
         void signalUpdateDigest(const QModelIndex& index);

      public:
         virtual void update() = 0;
         virtual void rebuild() = 0;
         virtual Ref::Structure::Digest* getDigest(const QModelIndex& index) = 0;
         virtual void createBeforeItem(const QModelIndex& index);
         virtual void removeItem(const QModelIndex& index);
         const Ref::Structure::PatchPart& getPart() const;

         static void updateAll();
         static void rebuildAll();

      protected:
         void updateDigestItem(QStandardItem* digestItem, const Ref::Structure::Digest& digest);

      protected:
         Ref::Structure& structure;
         Ref::Structure::PatchPart part;

      private:
         static QList<Abstract*> instanceList;
      };
   } // namespace Model
} // namespace PatchRef

#endif // NOT PatchRefModelAbstractH
