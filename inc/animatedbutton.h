#ifndef ANIMATEDBUTTON_H
#define ANIMATEDBUTTON_H

#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QEnterEvent>

/**
 * @enum ButtonTheme
 * @brief Definiuje dostępne motywy kolorystyczne przycisku.
 */
enum ButtonTheme {
    orange, /**< Motyw pomarańczowy. */
    grey    /**< Motyw szary. */
};


/**
 * @class AnimatedButton
 * @brief Przycisk z customowym stylem i opcjonalną animacją kółka.
 *
 * Klasa definiuje bazowy styl przycisku, efekty przy najechaniu na niego kursorem i kliknięciu.
 * Rozszerza ona QPushButton o możliwość wyświetlania animowanego elementu dekoracyjnego
 * (kółka), który przesuwa się po najechaniu myszą. Obsługuje dynamiczną zmianę języka
 * oraz dwa predefiniowane motywy graficzne.
 */
class AnimatedButton : public QPushButton {
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy AnimatedButton.
     * @param[in] height -- Wysokość przycisku w pikselach.
     * @param[in] width -- Szerokość przycisku w pikselach.
     * @param[in] style --  Wybrany motyw kolorystyczny (ButtonTheme).
     * @param[in] text -- Tekst wyświetlany na przycisku (klucz do tłumaczeń).
     * @param[in] isAnimated -- Jeśli true - przycisk posiada animowane kółko.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny.
     */
    explicit AnimatedButton(int height, int width, ButtonTheme style, const QString &text, bool isAnimated=true, QWidget *parent = nullptr);

protected:
    /**
     * @brief Obsługuje zdarzenie najechania myszą na przycisk.
     * Inicjuje animację przesunięcia kółka do prawej krawędzi.
     * @param[in] event -- rodzaj zdarzenia, obietku QEnterEvent
     */
    void enterEvent(QEnterEvent *event) override;

    /**
     * @brief Obsługuje zdarzenie opuszczenia obszaru przycisku przez mysz.
     * Inicjuje animację powrotu kółka na pozycję początkową.
     * @param[in] event -- zmienna zdarzenia.
     */
    void leaveEvent(QEvent *event) override;

    /**
     * @brief Reaguje na zmiany systemowe, nadpisano funkcjonalność zmiany języka aplikacji.
     * @param[in] event -- zmienna zdarzenia.
     */
    void changeEvent(QEvent *event) override;

private:
    QString _text;                  /**< Przechowuje źródłowy tekst przycisku (klucz translatora). */
    QString _buttonColor;           /**< Kolor tła przycisku. */
    QString _buttonHoverColor;      /**< Kolor tła po najechaniu myszą. */
    QString _buttonPressedColor;    /**< Kolor tła po kliknięciu. */
    QString _circleColor;           /**< Kolor elementu animowanego (kółka). */
    QString _circlePressedColor;    /**< Kolor kółka w stanie kliknięcia. */

    QLabel *_circle;                /**< Widget reprezentujący wizualne kółko. */
    QPropertyAnimation *_animation; /**< Silnik animacji przesunięcia kółka. */
    bool _isAnimated;               /**< Flaga określająca, czy animacja jest aktywna dla danej instancji. */

    /**
     * @brief Konfiguruje paletę kolorów na podstawie wybranego motywu.
     * @param[in] theme -- Referencja do wybranego motywu kolorystycznego.
     */
    void setupColors(const ButtonTheme &theme);

    /**
     * @brief Inicjalizuje obiekt kółka oraz konfiguruje parametry animacji.
     * @param[in] height -- Wysokość w pikselach używana do obliczenia rozmiarów kółka.
     */
    void setupAnimation(int height);

    /**
     * @brief Odświeża tekst przycisku przy użyciu aktualnego translatora systemu Qt.
     */
    void reTranslate();

};

#endif // ANIMATEDBUTTON_H
