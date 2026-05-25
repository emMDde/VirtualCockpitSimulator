#include "bottombar.h"
#include <qtranslator.h>

BottomBar::BottomBar(QWidget *parent, int height, int width) : ToolBar(parent, height, width, grey)
{
    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(height/4, height/4, height/4, height/4);
    topLayout->addWidget(_statusLed);
    topLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    topLayout->addWidget(_btnLanguage);
    topLayout->addWidget(_btnTheme);
    topLayout->addWidget(_btnPage);
}
