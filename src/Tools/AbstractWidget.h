#ifndef AbstractWidgetH
#define AbstractWidgetH

#include <QWidget>

#include <QToolBar>

#include "Tools/Central.h"

class QVBoxLayout;

namespace Abstract
{
   class Widget : public QWidget
   {
   public:
      Widget(QWidget* parent, Central* central);

   protected:
      void setPayload(QWidget* widget);

      template <typename... Args>
      QAction* addToToolBar(const QIcon& icon, const QString& text, Args&&... args)
      {
         return toolBar->addAction(icon, text, std::forward<Args>(args)...);
      }

   protected:
      Central* central;

   private:
      QToolBar* toolBar;
      QVBoxLayout* masterLayout;
   };
} // namespace Abstract

#endif // NOT AbstractWidgetH
