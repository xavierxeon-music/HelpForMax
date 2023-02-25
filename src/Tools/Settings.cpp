#include "Settings.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

QJsonObject Settings::docObject = QJsonObject();
quint32 Settings::useCount = 0;

Settings::Settings(const QString& groupName)
   : groupName(groupName)
{
   if (0 == useCount) // read new object
   {
      QFile file(fileName());
      if (file.open(QIODevice::ReadOnly)) // file exists
      {
         QJsonParseError error;
         QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
         file.close();

         docObject = doc.object();
      }
   }
   useCount++;
}

Settings::~Settings()
{
   if (0 == useCount)
      Q_ASSERT(false); // this should not happen!

   useCount--;
   if (0 < useCount) // object still in use
      return;

   // save object
   QFile file(fileName());
   if (!file.open(QIODevice::WriteOnly))
      return;

   QJsonDocument doc(docObject);
   file.write(doc.toJson());
   file.close();
}

void Settings::write(const QString& key, const QVariant& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue::fromVariant(value));
}

void Settings::write(const QString& key, const QVariantList& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const QVariant& content : value)
      array.append(QJsonValue::fromVariant(content));

   setValue(key, array);
}

void Settings::write(const QString& key, const QString& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void Settings::write(const QString& key, const QStringList& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const QString& content : value)
      array.append(content);

   setValue(key, array);
}

void Settings::write(const QString& key, const QList<int>& value, bool init)
{
   if (init && hasValue(key))
      return;

   QJsonArray array;
   for (const int& content : value)
      array.append(content);

   setValue(key, array);
}

void Settings::write(const QString& key, const QByteArray& value, bool init)
{
   if (init && hasValue(key))
      return;

   QByteArray asciiValue = value.toBase64();
   QJsonValue jv(QString::fromUtf8(asciiValue));
   setValue(key, jv);
}

void Settings::write(const QString& key, const int& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void Settings::write(const QString& key, const double& value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void Settings::write(const QString& key, bool value, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, QJsonValue(value));
}

void Settings::write(const QString& key, const QJsonObject& object, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, object);
}

void Settings::write(const QString& key, const QJsonArray& array, bool init)
{
   if (init && hasValue(key))
      return;

   setValue(key, array);
}

QVariant Settings::variant(const QString& key, const QVariant defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toVariant();
}

QVariantList Settings::variantList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QVariantList();

   QVariantList list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toVariant());

   return list;
}

QString Settings::string(const QString& key, const QString defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toString();
}

QStringList Settings::stringList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QStringList();

   QStringList list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toString());

   return list;
}

QList<int> Settings::intList(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QList<int>();

   QList<int> list;
   for (const QJsonValue& value : value.toArray())
      list.append(value.toInt());

   return list;
}

QByteArray Settings::bytes(const QString& key, const QByteArray defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   QByteArray asciiValue = value.toString().toUtf8();
   return QByteArray::fromBase64(asciiValue);
}

int Settings::integer(const QString& key, const int defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toInt();
}

double Settings::real(const QString& key, const double defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toDouble();
}

bool Settings::boolean(const QString& key, const bool defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toBool();
}

QJsonObject Settings::object(const QString& key, const QJsonObject& defaultValue) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return defaultValue;

   return value.toObject();
}

QJsonArray Settings::array(const QString& key) const
{
   QJsonValue value = getValue(key);
   if (value.isNull())
      return QJsonArray();

   return value.toArray();
}

bool Settings::hasValue(const QString& key) const
{
   if (groupName.isEmpty())
      return docObject.contains(key);

   if (docObject[groupName].isNull() || !docObject[groupName].isObject())
      return false;

   QJsonObject groupObject = docObject[groupName].toObject();
   return groupObject.contains(key);
}

const QString& Settings::fileName()
{
   // do not make variable a class static, because QApplication needs to be constructed first!
   static const QString settingsFileName = [&]()
   {
      const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/eu.schweinesystem.MaxPackageAdmin";
      QDir().mkpath(path);
      QString fileName = path + "/settings.json";

      qDebug() << "settings file @ " << fileName;

      return fileName;
   }();

   return settingsFileName;
}

QJsonValue Settings::getValue(const QString& key) const
{
   if (groupName.isEmpty())
      return docObject[key];

   if (docObject[groupName].isNull() || !docObject[groupName].isObject())
      return QJsonValue();

   QJsonObject groupObject = docObject[groupName].toObject();
   return groupObject[key];
}

void Settings::setValue(const QString& key, const QJsonValue& value)
{
   if (groupName.isEmpty())
   {
      docObject[key] = value;
      return;
   }

   if (docObject[groupName].isNull())
      docObject[groupName] = QJsonObject();

   QJsonObject groupObject = docObject[groupName].toObject();
   groupObject[key] = value;

   docObject[groupName] = groupObject; // write object back, since it is not a ref!
}
