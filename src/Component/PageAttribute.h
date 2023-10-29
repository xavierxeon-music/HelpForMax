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
      Attribute(QWidget* parent, Central* central, const Block::Marker& marker);

   private slots:
      void slotItemChanged(QStandardItem* item);

   private:
      void update(const QVariant& data) override;
      Structure::Type getType(const int index) override;

   private:
      DescriptionHighlighter* highlighter;
      QString attributeName;
      QStandardItemModel* attributeModel;
      bool updating;
   };
} // namespace Page

#endif // NOT PageAttributeH
