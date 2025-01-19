#ifndef PackageCleanModelH
#define PackageCleanModelH

#include <QStandardItemModel>

namespace Package
{
   namespace Clean
   {
      class Model : public QStandardItemModel
      {
         Q_OBJECT

      public:
         Model(QObject* parent);
      };
   } // namespace Clean
} // namespace Package

#endif // NOT PackageCleanModelH
