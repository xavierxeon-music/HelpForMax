#ifndef MaxDataTypeH
#define MaxDataTypeH

#include <QObject>

#include <QMap>
#include <QString>

namespace Max
{
   Q_NAMESPACE

   enum class DataType
   {
      Undefined,
      Anything,
      Symbol,
      Float,
      Integer,
      Bang,
      List,
      Signal,
      MultiSignal,
      Dictionary,
      Matrix
   };
   Q_ENUM_NS(DataType)

   using DataTypeNameMap = QMap<DataType, QString>;

   // message type
   QString dataTypeName(const DataType& type);
   DataType toDataType(const QString& name);
   DataType tagDataType(const QChar& tag);
   QList<DataType> dataTypeList();

} // namespace Max

#endif // NOT MaxDataTypeH
