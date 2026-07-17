#ifndef ENTRYOVERLAY_H
#define ENTRYOVERLAY_H

#include <QFrame>
#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QWidget>
#include <QVBoxLayout>
#include "animatedbutton.h"

/**
 * @class EntryOverlay
 * @brief Klasa reprezentująca nakładkę startową oraz ekran utraty połączenia z kontrolerem.
 *
 * Klasa służy do wyświetlania ekranu startowego oraz zarządza uruchomieniem wszystkich
 * elementów odpowiedzialnych za symulację (za pomocą przycisku). Wyświetla
 * animowany komunikat o stanie połączenia oraz zarządza przyciskiem startu/wznowienia gry.
 */
class EntryOverlay : public QFrame
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy EntryOverlay.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    EntryOverlay(QWidget *parent = nullptr);

public slots:

    /**
     * @brief Slot aktualizujący interfejs graficzny nakładki w zależności od stanu połączenia.
     * Odpowiednio blokuje/odblokowuje przycisk startowy oraz zmienia treść etykiety statusu.
     * @param[in] connected -- Stan połączenia: true oznacza podłączony kontroler, false -- brak sygnału.
     */
    void handleConnection(bool connected);

signals:

    /**
     * @brief Sygnał informujący o żądaniu zmiany stanu działania symulacji.
     * @param[out] status -- Wartość true oznacza start/wznowienie; false oznacza zatrzymanie/pauzę.
     */
    void simulationStatus(bool status);

protected:

    /**
     * @brief Reaguje na zmiany systemowe, nadpisano funkcjonalność zmiany języka aplikacji (LanguageChange).
     * @param[in] event -- Wskaźnik na obiekt zdarzenia QEvent.
     */
    void changeEvent(QEvent *event) override;

private:

    /**
     * @brief Odświeża teksty etykiet i przycisków przy użyciu aktualnego translatora Qt.
     */
    void reTranslate();

    bool _isConnected;           /**< Flaga przechowująca aktualny stan połączenia z fizycznym kontrolerem. */
    int _dotCount;               /**< Licznik wyświetlanych kropek (0-3) wykorzystywany w animacji oczekiwania. */
    QLabel *_statusLabel;        /**< Wskaźnik na etykietę tekstową prezentującą komunikaty o stanie systemu. */
    AnimatedButton *_startBtn;   /**< Wskaźnik na animowany przycisk służący do uruchamiania symulacji. */
    QTimer *_dotTimer;           /**< Timer odpowiedzialny za cykliczne odmierzanie czasu i aktualizację animacji kropek. */
};

#endif // ENTRYOVERLAY_H
