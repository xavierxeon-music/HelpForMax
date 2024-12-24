#ifndef PackageModelH
#define PackageModelH

#include <QStandardItemModel>

namespace Package
{
   class Info;

   class Model : public QStandardItemModel
   {
      Q_OBJECT

   public:
      enum DataRoles
      {
         RolePath = Qt::UserRole + 1,
         RoleInfo = Qt::UserRole + 2,
         RoleExpanded = Qt::UserRole + 3
      };

   public:
      Model(QObject* parent, const Info* packageInfo);

   public:
      void create();
      void updateIcons();
      QModelIndex find(const QString& patchFileName) const;

   private:
      const Info* packageInfo;
   };
} // namespace Package

#endif // NOT PackageModelH
