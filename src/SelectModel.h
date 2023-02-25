#ifndef SelectModelH
#define SelectModelH

#include <Central.h>
#include <QStandardItemModel>

#include <Central.h>
#include <QFileInfo>

class SelectModel : public QStandardItemModel,
                    private FunctionHub
{
   Q_OBJECT

public:
   enum Role
   {
      RolePatchPath = Qt::UserRole + 1,
      RoleKey = Qt::UserRole + 2
   };

public:
   SelectModel(QObject* parent);

private:
   using InfoMap = QMap<QString, QFileInfo>;

private:
   void setPackagePath(QString packageDir) override;
   void recursiveSearch(const QString& path, InfoMap& infoMap);
};

#endif // NOT SelectModelH
