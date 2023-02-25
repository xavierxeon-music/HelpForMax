#ifndef MessageH
#define MessageH

#include <QTreeWidget>

namespace Message
{
   void info(const QString& message);
   void warning(const QString& message);
   void error(const QString& message);

   class Widget : public QTreeWidget
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent);

   public:
   };
} // namespace Message

#endif // NOT MessageH
