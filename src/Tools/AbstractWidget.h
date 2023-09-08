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
      Widget(QWidget* parent, Central* central, const QString tilte);

   protected:
      void setPayload(QWidget* widget);

      template <typename... Args>
      QAction* toolBarAction(const QIcon& icon, const QString& text, Args&&... args)
      {
         return toolBar->addAction(icon, text, std::forward<Args>(args)...);
      }

      void showShortcuts();

   protected:
      Central* central;

   private:
      QToolBar* toolBar;
      QVBoxLayout* masterLayout;
   };
} // namespace Abstract

#endif // NOT AbstractWidgetH
