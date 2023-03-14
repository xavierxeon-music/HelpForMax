#ifndef MessageH
#define MessageH

#include <QTreeWidget>

namespace Message
{
   void info(const QString& message);
   void error(const QString& message);
   void undocumented(const QString& patchName, const QString& text);

   class Widget : public QTreeWidget
   {
      Q_OBJECT

   public:
      Widget(QWidget* parent);
      ~Widget();

   private:
      enum class Type
      {
         Info,
         Error,
         Undocumented
      };

   private:
      friend void info(const QString& message);
      friend void error(const QString& message);
      friend void undocumented(const QString& patchName, const QString& text);

   private:
      void addMessage(const Type& type, const QString& message, const QString& where = QString());

   private:
      static Widget* me;
   };

} // namespace Message

#endif // NOT MessageH
