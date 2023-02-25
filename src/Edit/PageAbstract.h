#ifndef HelpPageAbstractH
#define HelpPageAbstractH

#include <QWidget>

#include <QLineEdit>
#include <QPlainTextEdit>

#include <Central.h>

#include "HelpPatchParser.h"
#include "HelpPersona.h"

namespace Help
{
   class Persona;

   namespace Page
   {
      class Abstract : public QWidget,
                       protected Persona::FunctionHub,
                       private Central::FunctionHub
      {
         Q_OBJECT

      public:
         Abstract(Persona* persona, const PatchParser::Marker& marker);
         virtual ~Abstract();

      protected:
         virtual void update(const QVariant& data) = 0;
         void monitor(QLineEdit* lineEdit, QString* variable);
         void monitor(QLineEdit* lineEdit, QStringList* variable);
         void monitor(QPlainTextEdit* textEdit, QString* variable);

      protected:
         Persona* persona;
         const PatchParser::Marker editMarker;

      private:
         using ConnectionMap = QMap<QWidget*, QMetaObject::Connection>;

      private:
         void componentSelected(PatchParser::Marker marker, QVariant data) override final;

      private:
         ConnectionMap connectionMap;
         bool blocked;
      };
   } // namespace Page
} // namespace Help

#endif // NOT HelpPageAbstractH
