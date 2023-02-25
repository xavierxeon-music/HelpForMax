#ifndef SettingsH
#define SettingsH

#include <QJsonObject>

class Settings
{
public:
   Settings(const QString& groupName = QString());
   ~Settings();

public:
   void write(const QString& key, const QVariant& value, bool init = false);
   void write(const QString& key, const QVariantList& value, bool init = false);
   void write(const QString& key, const QString& value, bool init = false);
   void write(const QString& key, const QStringList& value, bool init = false);
   void write(const QString& key, const QList<int>& value, bool init = false);
   void write(const QString& key, const QByteArray& value, bool init = false);
   void write(const QString& key, const int& value, bool init = false);
   void write(const QString& key, const double& value, bool init = false);
   void write(const QString& key, bool value, bool init = false);
   void write(const QString& key, const QJsonObject& object, bool init = false);
   void write(const QString& key, const QJsonArray& array, bool init = false);

   QVariant variant(const QString& key, const QVariant defaultValue = QVariant()) const;
   QVariantList variantList(const QString& key) const;
   QString string(const QString& key, const QString defaultValue = QString()) const;
   QStringList stringList(const QString& key) const;
   QList<int> intList(const QString& key) const;
   QByteArray bytes(const QString& key, const QByteArray defaultValue = QByteArray()) const;
   int integer(const QString& key, const int defaultValue = 0) const;
   double real(const QString& key, const double defaultValue = 0.0) const;
   bool boolean(const QString& key, const bool defaultValue = false) const;
   QJsonObject object(const QString& key, const QJsonObject& defaultValue = QJsonObject()) const;
   QJsonArray array(const QString& key) const;

   bool hasValue(const QString& key) const;
   static const QString& fileName();

private:
   QJsonValue getValue(const QString& key) const;
   void setValue(const QString& key, const QJsonValue& value);

private:
   static QJsonObject docObject;
   static quint32 useCount;
   const QString groupName;
};

#endif // NOT SettingsH
