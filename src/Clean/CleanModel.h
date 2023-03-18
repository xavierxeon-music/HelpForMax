#ifndef CleanModelH
#define CleanModelH

#include <QStandardItemModel>

namespace Clean
{
   class Model : public QStandardItemModel
   {
      Q_OBJECT

   public:
      Model(QObject* parent, const QStringList& helpFileList);

   public:
      void apply();

   private:
      static const int PathRole = Qt::UserRole + 2;
      static const int KeyRole = Qt::UserRole + 3;

   private:
      void slotItemChanged(QStandardItem* item);
   };
} // namespace Clean

#endif // NOT CleanModelH
