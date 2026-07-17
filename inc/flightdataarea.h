#ifndef FLIGHTDATAAREA_H
#define FLIGHTDATAAREA_H

#include <QFrame>
#include <QPainter>
#include <QList>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QLabel>
#include <QPainterPath>
#include <QGraphicsEffect>
#include "virtualhorizon.h"
#include "toolbar.h"

/**
 * @class FlightDataArea
 * @brief Obszar interfejsu odpowiedzialny za wizualizację parametrów lotu.
 *
 * Klasa dziedziczy po QFrame. Zarządza wyświetlaniem dynamicznych wykresów telemetrycznych
 * dla kątów przechylenia (rotY) i pochylenia (rotX), a także integruje w sobie widok
 * wirtualnego horyzontu. Pozwala na pauzowanie oraz czyszczenie historii zebranych danych.
 */
class FlightDataArea : public QFrame
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy FlightDataArea.
     * @param[in] height -- Preferowana wysokość widgetu w pikselach.
     * @param[in] width -- Preferowana szerokość widgetu w pikselach.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit FlightDataArea(int height, int width, QWidget *parent = nullptr);

signals:

    /**
     * @brief Sygnał przekazujący dalej aktualne parametry symulacji.
     * @param[out] rotX -- Kąt pochylenia w stopniach.
     * @param[out] rotY -- Kąt przechylenia w stopniach.
     */
    void updateSimData(float rotX, float rotY);

public slots:

    /**
     * @brief Slot aktualizujący kolory i motyw wizualny całego obszaru oraz jego podelementów.
     */
    void changeTheme();

    //void startSimulation();

protected:

    /**
     * @brief Reaguje na zmiany systemowe, nadpisano funkcjonalność zmiany języka aplikacji.
     * @param[in] event -- Wskaźnik na obiekt zdarzenia QEvent.
     */
    void changeEvent(QEvent *event) override;

    /**
     * @brief Główna metoda rysująca tło, chmury oraz obsługująca niestandardowe rysowanie wykresów.
     * @param[in] event -- Wskaźnik na zdarzenie odrysowania ekranu QPaintEvent.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Obsługuje zmianę rozmiaru obszaru, dostosowując układy wewnętrznych widgetów.
     * @param[in] event -- Wskaźnik na zdarzenie zmiany rozmiaru QResizeEvent.
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:

    /**
     * @brief Zapisuje nowe dane do historii i wymusza przerysowanie wykresów, jeśli odczyt nie jest zapauzowany.
     * @param[in] rotX -- Kąt pochylenia wyrażony w stopniach.
     * @param[in] rotY -- Kąt przechylenia wyrażony w stopniach.
     */
    void setData(float rotX, float rotY);

    /**
     * @brief Slot zatrzymujący lub wznawiający dopisywanie nowych punktów do wykresów.
     */
    void togglePause();

    /**
     * @brief Slot usuwający dotychczasową historię punktów wykresu z pamięci.
     */
    void clearData();

private:

    VirtualHorizon *_horizon;        /**< Wskaźnik na zagnieżdżony widok wirtualnego horyzontu (HUD). */
    QLabel* _titleLabel;             /**< Etykieta przechowująca tytuł sekcji, przystosowana do tłumaczenia przez tr(). */
    QFrame* _layoutContainer;        /**< Główny widżet kontenera, na który nakładany jest styl tła panelu. */
    QWidget* _xGraphSpace;           /**< Pusty widżet rezerwujący przestrzeń roboczą do ręcznego rysowania wykresu Roll. */
    QWidget* _yGraphSpace;           /**< Pusty widżet rezerwujący przestrzeń roboczą do ręcznego rysowania wykresu Pitch. */

    QList<float> _rollHistory;       /**< Historia zarejestrowanych wartości kąta przechylenia (Roll). */
    QList<float> _pitchHistory;      /**< Historia zarejestrowanych wartości kąta pochylenia (Pitch). */
    long long _totalDataPoints = 0;  /**< Licznik absolutnie wszystkich punktów pomiarowych odebranych od startu. */
    float _sampleRate = 50.0f;       /**< Częstotliwość próbkowania sygnału wejściowego wyrażona w hercach (Hz). */
    int _maxDataPoints;              /**< Maksymalna liczba punktów mieszcząca się jednocześnie na wykresie. */
    bool _isPaused;                  /**< Flaga wstrzymująca dopisywanie nowych danych i odświeżanie wykresów. */

    QPushButton* _pauseBtn;          /**< Przycisk służący do zatrzymywania i wznawiania rejestracji danych. */
    QPushButton* _clearBtn;          /**< Przycisk służący do czyszczenia dotychczasowej historii wykresów. */

    QColor _bgColor;                 /**< Kolor tła głównego obszaru widżetu. */
    QColor _lineColor;               /**< Kolor linii elementów konstrukcyjnych interfejsu. */
    QColor _groundColor;             /**< Kolor reprezentujący powierzchnię ziemi. */
    QColor _panelColor;              /**< Kolor tła dla kontenerów wykresów. */
    QColor _textColor;               /**< Główny kolor czcionek dla etykiet tekstowych. */
    QColor _graphBgColor;            /**< Kolor tła siatki wykresu liniowego. */
    QColor _graphTextColor;          /**< Kolor tekstu i linii siatki bezpośrednio na wykresie. */


    /**
     * @brief Odświeża tytuł okna oraz opisy przycisków przy użyciu aktualnego translatora.
     */
    void reTranslate();

    /**
     * @brief Metoda pomocnicza rysująca pojedynczą chmurę dekoracyjną w tle paneli.
     * @param[in, out] painter -- Referencja do obiektu malarza rysującego komponent.
     * @param[in] x -- Pozycja X lewego górnego rogu obszaru chmury.
     * @param[in] y -- Pozycja Y lewego górnego rogu obszaru chmury.
     * @param[in] cW -- Szerokość chmury.
     * @param[in] cH -- Wysokość chmury.
     */
    void drawCloud(QPainter &painter, int x, int y, int cW, int cH);

    /**
     * @brief Wykreśla linie telemetrii wraz z osiami, opisanymi krańcami i siatką w podanym obszarze.
     * @param[in, out] painter -- Referencja do obiektu malarza.
     * @param[in] rect -- Obszar (prostokąt), w którym zostanie narysowany wykres.
     * @param[in] data -- Lista punktów telemetrycznych z pamięci podręcznej do narysowania.
     * @param[in] color -- Kolor dla kreślonej linii wykresu.
     * @param[in] maxVal -- Graniczna/maksymalna wartość dla skalowania pionowego (oś Y).
     * @param[in] topTxt -- Tekst reprezentujący górną granicę wartości (np. "90").
     * @param[in] botTxt -- Tekst reprezentujący dolną granicę wartości (np. "-90").
     */
    void drawGraph(QPainter &painter, const QRect& rect, const QList<float>& data, QColor color, float maxVal, const QString& topTxt, const QString& botTxt);
};

#endif // FLIGHTDATAAREA_H