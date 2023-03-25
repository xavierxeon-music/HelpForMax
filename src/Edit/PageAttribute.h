#ifndef PageAttributeH
#define PageAttributeH

#include "PageAbstract.h"
#include "ui_PageAttribute.h"

#include <QStandardItemModel>

#include "Tools/TypeDelegate.h"

#include "DescriptionHighlighter.h"

namespace Page
{
   class Attribute : public Abstract,
                     public TypeDelegate::Proxy,
                     private Ui::Attribute
   {
      Q_OBJECT

   public:
      Attribute(MainWindow* mainWindow, const Block::Item::Marker& marker);

   private slots:
      void slotItemChanged(QStandardItem* item);

   private:
      void update(const QVariant& data) override;
      Block::Structure::Type getType(const int index) override;

   private:
      DescriptionHighlighter* highlighter;
      QString attributeName;
      QStandardItemModel* attributeModel;
   };
} // namespace Page

#endif // NOT PageAttributeH
