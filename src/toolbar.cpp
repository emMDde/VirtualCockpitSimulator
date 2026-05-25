#include "toolbar.h"
#include <qtranslator.h>

QString ToolBar::LANGUAGE = "";

ToolBar::ToolBar(QWidget *parent, int height, int width, ButtonTheme theme) : QFrame(parent)
{
    this->setFixedHeight(height);
    this->setStyleSheet("TopBar { background-color: #17181F; }");

    _btnPage = new AnimatedButton(height/2, height*2, theme, tr("DANE"), this);
    connect(_btnPage, &AnimatedButton::clicked, this, &ToolBar::pageButtonClicked);
    _btnTheme = new AnimatedButton(height/2, height*2, theme, tr("MOTYW"), this);
    connect(_btnTheme, &AnimatedButton::clicked, this, &ToolBar::themeButtonClicked);
    _btnLanguage = new AnimatedButton(height/2, height*2, theme, tr("JĘZYK"), false, this);
    _btnLanguage->setMenu(createLangMenu());

    _statusLed = new QLabel(this);
    _ledRadius = height/4;
    _statusLed->setFixedSize(height/2, height/2);
    setConnectionStatus(false);
}

QMenu *ToolBar::createLangMenu()
{
    QMenu *langMenu = new QMenu(this);
    langMenu->setStyleSheet("QMenu { background-color: #222222; color: white; border: 1px solid #F0802C; }"
                            "QMenu::item:selected { background-color: #F0802C; }"); //                             "QMenu::indicator::checked{border: 0.5px solid #FFFFFF;}"

    QAction *actionPL = langMenu->addAction("Polski");
    actionPL->setCheckable(true);
    QAction *actionEN = langMenu->addAction("English");
    actionEN->setCheckable(true);
    QAction *actionDE = langMenu->addAction("Deutsch");
    actionDE->setCheckable(true);
    QString localLang = QLocale().name();
    if (localLang.startsWith("pl")) actionPL->setChecked(true);
    else if(localLang.startsWith("de")) actionDE->setChecked(true);
    else actionEN->setChecked(true);

    QActionGroup *langGroup = new QActionGroup(this);
    langGroup->addAction(actionPL);
    langGroup->addAction(actionEN);
    langGroup->addAction(actionDE);

    connect(langGroup, &QActionGroup::triggered, this, [=](QAction *action)
        {
            extern QTranslator* translator;
            if (action->text() == "Polski") qApp->removeTranslator(translator);
            else
            {
                QString locale = (action->text() == "English") ? "en" : "de";
                if (translator->load(":/translates/WDS_Projekt_" + locale + ".qm")) qApp->installTranslator(translator);
            }
            LANGUAGE=action->text();
        });

    return langMenu;
}

void ToolBar::setConnectionStatus(bool connected)
{
    QString color = connected ? "#00FF00" : "#FF0000";

    QString ledStyle = QString("QLabel { background-color: %1; border-radius: %2px; }").arg(color).arg(_ledRadius);
    _statusLed->setStyleSheet(ledStyle);
}

void ToolBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        for (QAction *action : _btnLanguage->menu()->actions())
        {
            if (action->text() == LANGUAGE) action->setChecked(true);
        }
    }
    QFrame::changeEvent(event);
}


// QString btnStyle = QString(
//                        "QPushButton { "
//                        "   border-image: url(button_orange.png) 0 80 0 80; "
//                        "   border-width: 0 %1px 0 %1px; "
//                        "   color: white; "
//                        "   font-weight: bold; "
//                        "}"
//                        "QPushButton:pressed { "
//                        "   color: #FF0000; "
//                        "}").arg(height/2);
