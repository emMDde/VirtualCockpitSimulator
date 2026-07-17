#ifndef SIMULATORAREA_H
#define SIMULATORAREA_H

#include <QWidget>
#include <QFrame>
#include <QPainter>
#include <QPixmap>
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QTimer>
#include "toolbar.h"

/**
 * @class SimulatorArea
 * @brief Obszar graficzny odpowiedzialny za renderowanie wizualizacji lotu samolotu.
 *
 * Klasa dziedziczy po QFrame i odpowiada za rysowanie ruchomego otoczenia (niebo, ziemia, chmury)
 * oraz samego modelu samolotu wraz z animowanym śmigłem. Renderowanie odbywa się w oparciu
 * o systemowe mechanizmy QPainter i obiekty QPixmap, a płynność animacji zapewnia wewnętrzny timer.
 */
class SimulatorArea : public QFrame
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy SimulatorArea.
     * Inicjalizuje domyślne parametry środowiska, ładuje zasoby graficzne i konfiguruje timer renderu.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit SimulatorArea(QWidget *parent = nullptr);

public slots:

    /**
     * @brief Kontroluje stan działania animacji i timera odświeżania ekranu.
     * @param[in] status -- Wartość true uruchamia renderowanie; false zatrzymuje/zamraża animację.
     */
    void simulationStatus(bool status);

    /**
     * @brief Aktualizuje wewnętrzne zmienne położenia samolotu, wpływając na jego pozycję na ekranie.
     * @param[in] rotX -- Kąt pochylenia przekazany z modelu matematycznego.
     * @param[in] rotY -- Kąt przechylenia przekazany z modelu matematycznego.
     */
    void setData(float rotX, float rotY);

    /**
     * @brief Slot aktualizujący paletę kolorów nieba, ziemi oraz elementów otoczenia zgodnie z wybranym motywem.
     */
    void changeTheme();

signals:

protected:

    /**
     * @brief Główna metoda rysująca cały kadr symulacji (tło, chmury, samolot i śmigło).
     * @param[in] event -- Wskaźnik na obiekt zdarzenia rysowania QPaintEvent.
     */
    void paintEvent(QPaintEvent *event) override;

private slots:

    /**
     * @brief Slot wywoływany cyklicznie przez timer, odpowiedzialny za wymuszenie przerysowania klatki (update()).
     */
    void frameTimerTick();

private:
    QTimer* _frameTimer;         /**< Timer odpowiedzialny za taktowanie i wymuszanie odświeżania klatek animacji. */
    float _rotX;                 /**< Bieżący kąt przechylenia (Roll) samolotu używany do transformacji graficznych. */
    float _rotY;                 /**< Bieżący kąt pochylenia (Pitch) samolotu używany do pozycjonowania obiektu. */
    float _animSpeed;            /**< Prędkość poruszania się elementów otoczenia (np. chmur). */
    float _propellerRot;         /**< Aktualny kąt obrotu grafiki śmigła (w stopniach). */
    float _simTime;              /**< Licznik czasu symulacji używany do generowania płynnych animacji falowania. */
    QPixmap _planeImage;         /**< Załadowana do pamięci tekstura kadłuba samolotu. */
    QPixmap _treeImage;          /**< Załadowana do pamięci tekstura drzewa (element otoczenia). */
    QPixmap _propellerImage;     /**< Załadowana do pamięci tekstura śmigła samolotu. */

    QColor _skyColorTop;         /**< Kolor górnej krawędzi gradientu nieba. */
    QColor _skyColorBottom;      /**< Kolor dolnej krawędzi gradientu nieba. */
    QColor _groundColor;         /**< Główny kolor powierzchni ziemi. */
    QColor _pathColor;           /**< Kolor główny pasa startowego lub drogi. */
    QColor _pathWalkColor;       /**< Kolor krawędzi/pobocza pasa startowego. */
    QColor _cloudColor;          /**< Kolor używany do wypełnienia generowanych chmur. */
    QColor _lineColor;           /**< Kolor pomocniczych linii konturowych. */

    /**
     * @brief Rysuje statyczne i dynamiczne elementy tła, takie jak gradientowe niebo i ziemię.
     * @param[in, out] painter -- Referencja do obiektu malarza obsługującego bieżący kadr.
     */
    void drawBackground(QPainter &painter);

    /**
     * @brief Metoda pomocnicza generująca i rysująca chmurę o zadanych wymiarach w określonym miejscu.
     * @param[in, out] painter -- Referencja do obiektu malarza.
     * @param[in] x -- Pozycja na osi X lewego marginesu chmury.
     * @param[in] y -- Pozycja na osi Y górnego marginesu chmury.
     * @param[in] cW -- Szerokość całkowita rysowanej chmury.
     * @param[in] cH -- Wysokość całkowita rysowanej chmury.
     */
    void drawCloud(QPainter &painter, double x, double y, double cW, double cH);
};

#endif // SIMULATORAREA_H
