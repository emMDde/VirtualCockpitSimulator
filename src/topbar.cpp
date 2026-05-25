#include "topbar.h"
#include <qtranslator.h>

TopBar::TopBar(QWidget *parent, int height, int width) : ToolBar(parent, height, width, orange)
{
    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(height/4, height/4, height/4, height/4);
    topLayout->addWidget(_btnPage);
    topLayout->addWidget(_btnTheme);
    topLayout->addWidget(_btnLanguage);
    topLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    topLayout->addWidget(_statusLed);
}