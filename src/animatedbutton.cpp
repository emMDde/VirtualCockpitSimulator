#include "animatedbutton.h"

AnimatedButton::AnimatedButton(int height, int width, ButtonTheme theme, const QString &text, bool isAnimated, QWidget *parent) : QPushButton(text, parent), _circle(nullptr), _text(text), _animation(nullptr), _isAnimated(isAnimated)
{
    setupColors(theme);

    QString style = QString(
        "QPushButton { "
        "   background-color: %1; "
        "   border-radius: %2px; "
        "   color: white; "
        "   font-weight: bold; "
        "   border: none; "
        "}"
        "QPushButton:hover {"
        "   background-color: %3;"
        "   color: #CC6618;"
        "}"
        "QPushButton:pressed { "
        "   background-color: %4; "
        " color: white; "
    "}").arg(_buttonColor).arg(height/2.1).arg(_buttonHoverColor).arg(_buttonPressedColor);
    this->setStyleSheet(style);
    this->setFixedSize(width,height);

    if(_isAnimated) setupAnimation(height);
}

void AnimatedButton::setupAnimation(int height)
{
    QString circleBaseStyle = QString("QLabel { background-color: %1; border-radius: %2px; }").arg(_circleColor).arg(height/2);
    QString circlePressedStyle = QString("QLabel { background-color: %1; border-radius: %2px; }").arg(_circlePressedColor).arg(height/2);
    _circle = new QLabel(this);
    _circle->setObjectName("circle");
    _circle->setFixedSize(height, height);
    _circle->setStyleSheet(circleBaseStyle);
    _circle->setAttribute(Qt::WA_TransparentForMouseEvents);

    connect(this, &QPushButton::pressed, this, [=]() {
        _circle->setStyleSheet(circlePressedStyle);
    });

    connect(this, &QPushButton::released, this, [=]() {
        _circle->setStyleSheet(circleBaseStyle);
    });

    _animation = new QPropertyAnimation(_circle,"pos", this);
    _animation->setDuration(300);
    _animation->setEasingCurve(QEasingCurve::OutCubic);
}

void AnimatedButton::enterEvent(QEnterEvent *event)
{
    if(_isAnimated && this->isEnabled())
    {
        _animation->stop();
        _animation->setEndValue(QPoint(this->width() - _circle->width(), 0));
        _animation->start();
    }
    QPushButton::enterEvent(event);
}

void AnimatedButton::leaveEvent(QEvent *event)
{
    if(_isAnimated && this->isEnabled())
    {
        _animation->stop();
        _animation->setEndValue(QPoint(0, 0));
        _animation->start();
    }
    QPushButton::leaveEvent(event);
}

void AnimatedButton::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) reTranslate();
    QPushButton::changeEvent(event);
}

void AnimatedButton::reTranslate()
{
    this->setText(tr(_text.toUtf8().constData()));
    //qDebug() << " Wywołano retranslate dla przycisku!" << _text.toUtf8().constData();
}

void AnimatedButton::setupColors(const ButtonTheme &theme)
{
    if(theme==orange)
    {
        _buttonColor        = "#F0802C";
        _buttonHoverColor   = "#F08B3E";
        _buttonPressedColor = "#FA6F05";
        _circleColor        = "#B56021";
        _circlePressedColor = "#CC5902";
    }
    if(theme==grey)
    {
        _buttonColor        = "#49423D";
        _buttonHoverColor   = "#5B534D";
        _buttonPressedColor = "#38322E";
        _circleColor        = "#2E2926";
        _circlePressedColor = "#1A1715";
    }
}