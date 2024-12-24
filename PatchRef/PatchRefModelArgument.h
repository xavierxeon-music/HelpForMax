#ifndef PatchRefModelArgumentH
#define PatchRefModelArgumentH

#include "DelegateDataType.h"
#include "PatchRefModelAbstract.h"

namespace PatchRef
{
   namespace Model
   {
      class Argument : public Abstract, public Delegate::DataType::Source
      {
         Q_OBJECT

      public:
         Argument(QObject* parent, Ref::Structure& structure);

      private:
         void update() override;
         void rebuild() override;
         Ref::Structure::Digest* getDigest(const QModelIndex& index) override;
         void createBeforeItem(const QModelIndex& index) override;
         void removeItem(const QModelIndex& index) override;
         bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
         Max::DataType getDataType(const int index) override;
      };
   } // namespace Model
} // namespace PatchRef

#endif // NOT PatchRefModelArgumentH
