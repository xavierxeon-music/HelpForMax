#ifndef CleanModelH
#define CleanModelH

#include <QStandardItemModel>

namespace Clean
{
   class Model : public QStandardItemModel
   {
      Q_OBJECT

   public:
      Model(QObject* parent, const QStringList& keyList);

   public:
      bool needCleaning() const;
      void apply();

   private:
      static const int PathRole = Qt::UserRole + 2;
      static const int KeyRole = Qt::UserRole + 3;

      struct Target
      {
         QString docPath;
         QString helpPath;
         QString initPath;

         using Map = QMap<QString, Target>; // key vs target
         using MemPtr = QString Target::*;
      };

   private:
      void slotItemChanged(QStandardItem* item);

   private:
      Target::Map targetMap;
   };
} // namespace Clean

#endif // NOT CleanModelH
