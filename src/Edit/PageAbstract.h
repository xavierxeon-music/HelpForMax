#ifndef PageAbstractH
#define PageAbstractH

#include <QWidget>

#include <QCheckBox>
#include <QLineEdit>
#include <QPlainTextEdit>

#include "Tools/Central.h"
#include "Tools/PatchParser.h"

namespace Page
{
   class Abstract : public QWidget,
                    protected FunctionHub
   {
      Q_OBJECT

   public:
      Abstract(MainWindow* mainWindow, const PatchParser::Marker& marker);
      virtual ~Abstract();

   protected:
      virtual void update(const QVariant& data) = 0;
      void monitor(QCheckBox* checkBox, bool* variable, const QString& patchKey);
      void monitor(QLineEdit* lineEdit, QString* variable, const QString& patchKey);
      void monitor(QLineEdit* lineEdit, QStringList* variable, const QString& patchKey);
      void monitor(QPlainTextEdit* textEdit, QString* variable, const QString& patchKey);

   protected:
      MainWindow* mainWindow;
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

#endif // NOT PageAbstractH
