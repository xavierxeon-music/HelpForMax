#ifndef SuggestTransferModelH
#define SuggestTransferModelH

#include <QStandardItemModel>

namespace Package
{
   class Info;
}

namespace Suggest
{
   namespace Transfer
   {
      class Model : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Model(QObject* parent, const Package::Info* packageInfo);
      };
   } // namespace Transfer
} // namespace Suggest

#endif // NOT SuggestTransferModelH
