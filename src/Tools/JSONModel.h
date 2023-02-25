#ifndef JSONModelH
#define JSONModelH

#include <QStandardItemModel>

#include <QJsonObject>

#include <ModelItem.h>

namespace JSON
{
   QJsonObject fromFile(const QString& fileName);

   class Model : public QStandardItemModel
   {
      Q_OBJECT
   public:
      Model(QObject* parent);

   public:
      void readFile(const QString& filePath);

      static void iterateObject(const QJsonObject& object, QStandardItem* parent);
      static void iterateArray(const QJsonArray& array, QStandardItem* parent);
      static void addToModel(const QString& key, const QJsonValue& value, QStandardItem* parent);
   };
} // namespace JSON

#endif // NOT JSONModelH
