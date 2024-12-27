#ifndef MaxStyleH
#define MaxStyleH

#include <QColor>
#include <QMap>
#include <QString>

namespace Max
{
   struct Style
   {
      using Map = QMap<QString, Style>;

      QColor accentColor;
   };
} // namespace Max

#endif // NOT MaxStyleH
