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
#include <qtranslator.h>
#include "animatedbutton.h"

/**
 * @class ToolBar
 * @brief Pasek narzędziowy / panel nawigacyjny aplikacji.
 *
 * Klasa dziedziczy po QFrame i reprezentuje uniwersalny panel (górny lub dolny) interfejsu.
 * Zawiera przyciski nawigacji między podstronami, przełącznik motywów, rozwijalne menu wyboru
 * języka aplikacji (z obsługą dynamicznego ładowania plików .qm) oraz wizualną diodę LED
 * informującą użytkownika o stanie połączenia z kontrolerem UART.
 */
class ToolBar : public QFrame
{
    Q_OBJECT

public:

    /**
     * @brief Konstruktor klasy ToolBar.
     * Inicjalizuje układ paska, tworzy przyciski nawigacyjne, konfiguruje diodę LED oraz menu językowe.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     * @param[in] height -- Wysokość paska narzędziowego wyrażona w pikselach (domyślnie 60).
     * @param[in] width -- Szerokość paska narzędziowego wyrażona w pikselach (domyślnie 1280).
     * @param[in] pageText -- Klucz tekstowy (do translatora) dla przycisku zmiany strony.
     * @param[in] theme -- Początkowy motyw kolorystyczny przycisków (domyślnie orange).
     */
    explicit ToolBar(QWidget *parent = nullptr, int height = 60, int width = 1280,  const char* pageText = nullptr, ButtonTheme theme=orange);

    /**
     * @brief Zwraca aktualny, globalny stan motywu graficznego.
     * @return true, jeśli aktywny jest motyw ciemny/szary; false dla motywu jasnego/pomarańczowego.
     */
    static const bool getTheme();

public slots:

    /**
     * @brief Zmienia kolor diody statusu na podstawie stanu połączenia.
     * @param[in] connected -- Jeśli true - dioda świeci na zielono (połączono). Jeśli false - na czerwono (brak połączenia).
     */
    void setConnectionStatus(bool connected);

    /**
     * @brief Slot aktualizujący arkusz stylów (StyleSheet) paska i jego podelementów po zmianie motywu.
     */
    void changeTheme();

signals:

    /**
     * @brief Sygnał emitowany po kliknięciu przycisku zmiany motywu graficznego ("MOTYW").
     */
    void themeButtonClicked();

    /**
     * @brief Sygnał emitowany po kliknięciu przycisku nawigacji ("DANE" / "SYMULATOR").
     */
    void pageButtonClicked();

    // /**
    //  * @brief Sygnał emitowany po kliknięciu przycisku "JĘZYK".
    //  */
    // void languageButtonClicked(QString language);

protected:

    /**
     * @brief Przechwytuje i obsługuje zdarzenia systemowe, w tym dynamiczną zmianę języka aplikacji (LanguageChange).
     * @param[in] event -- Wskaźnik na obiekt zdarzenia QEvent.
     */
    void changeEvent(QEvent *event) override;

    static QString LANGUAGE;        /**< Statyczna zmienna przechowująca identyfikator aktualnie załadowanego języka. */

    AnimatedButton *_btnPage; /**< Przycisk zmiany strony apliakcji. */
    AnimatedButton *_btnTheme; /**< Przycisk zmiany motywu apliakcji. */
    AnimatedButton *_btnLanguage; /**< Przycisk zmiany języka apliakcji. */
    QLabel *_statusLed; /**< Zmienna przechowująca diodę wskazującą status połączenia z kontrolerem. */
    int _ledRadius;     /**< Promień diody wskazującej status połączenia. */

private slots:

    /**
     * @brief Zmienia wewnętrzny stan statycznej flagi motywu na przeciwny.
     */
    static void setTheme();

private:

    static bool THEME;  /**< Statyczna flaga globalnego stanu motywu (false = orange, true = grey). */

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
