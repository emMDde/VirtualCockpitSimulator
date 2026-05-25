#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QScreen>
#include <QFrame>
#include <QPropertyAnimation>
#include <QActionGroup>
#include "animatedbutton.h"

class ToolBar : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy ToolBar.
     * * Inicjalizuje układ, przyciski i diodę LED oraz ustala ich rozmiary.
     * @param parent Wskaźnik na widget nadrzędny (domyślnie nullptr).
     * @param height Wysokość paska w pikselach.
     * @param width Szerokość paska w pikselach.
     */
    explicit ToolBar(QWidget *parent = nullptr, int height = 60, int width = 1280, ButtonTheme theme=orange);

public slots:
    /**
     * @brief Ustawia kolor diody statusu.
     * * @param connected Jeśli true - dioda świeci na zielono (połączono).
     * Jeśli false - dioda świeci na czerwono (brak połączenia).
     */
    void setConnectionStatus(bool connected);

signals:
    /**
     * @brief Sygnał emitowany po kliknięciu przycisku "MOTYW".
     */
    void themeButtonClicked();

    // /**
    //  * @brief Sygnał emitowany po kliknięciu przycisku "JĘZYK".
    //  */
    // void languageButtonClicked(QString language);

    /**
     * @brief Sygnał emitowany po kliknięciu przycisku "DANE".
     */
    void pageButtonClicked();

protected:

    void changeEvent(QEvent *event) override;

    static QString LANGUAGE;

    AnimatedButton *_btnPage;
    AnimatedButton *_btnTheme;
    AnimatedButton *_btnLanguage;
    QLabel *_statusLed; /**< Zmienna przechowująca diodę wskazującą status połączenia z kontrolerem. */
    int _ledRadius;     /**< Promień diody wskazującej status połączenia. */

private:
    /**
     * @brief Funkcja tworzy i konfiguruje menu wyboru języka aplikacji.
     * * Funkcja inicjalizuje obiekty QAction dla dostępnych języków (Polski, English, Deutsch),
     * ustawia domyślnie zaznaczony język na podstawie ustawień systemowych (QLocale)
     * oraz podpina logikę przełączania tłumaczeń w locie za pomocą obiektu QTranslator.
     * * @return Wskaźnik na skonfigurowany obiekt QMenu, gotowy do przypięcia do przycisku.
     */
    QMenu *createLangMenu();
};

#endif // TOOLBAR_H
