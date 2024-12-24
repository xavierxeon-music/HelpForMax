#ifndef PatchRefModelHeaderH
#define PatchRefModelHeaderH

#include "DelegatePatchType.h"
#include "PatchRefModelAbstract.h"

namespace PatchRef
{
   namespace Model
   {
      class Header : public Abstract, public Delegate::PatchType::Source
      {
         Q_OBJECT

      public:
         Header(QObject* parent, Ref::Structure& structure);

      private:
         void update() override;
         void rebuild() override;
         Ref::Structure::Digest* getDigest(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
         Ref::Structure::PatchType getPatchType(const int index) override;
      };
   } // namespace Model
} // namespace PatchRef

#endif // NOT PatchRefModelHeaderH
