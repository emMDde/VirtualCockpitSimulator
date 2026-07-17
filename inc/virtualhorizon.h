#ifndef VIRTUALHORIZON_H
#define VIRTUALHORIZON_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include "toolbar.h"

/**
 * @class VirtualHorizon
 * @brief Widget wizualizujący sztuczny horyzont (wskaźnik położenia).
 *
 * Klasa odpowiedzialna za graficzne rysowanie wirtualnego lotniczego przyrządu pokładowego (HUD).
 * Prezentuje przestrzenną orientację statku powietrznego, symulując ruch tarczy żyroskopowej
 * i pokazując kąty przechylenia oraz pochylenia na podstawie przetworzonych
 * danych telemetrycznych.
 */
class VirtualHorizon : public QWidget
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy VirtualHorizon.
     * Inicjalizuje parametry wskaźnika, ustawia domyślne kolory oraz zapamiętuje bazowe wymiary gabarytowe.
     * @param[in] height -- Początkowa wysokość przyrządu w pikselach.
     * @param[in] width -- Początkowa szerokość przyrządu w pikselach.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit VirtualHorizon(int height, int width, QWidget *parent = nullptr);

public slots:

    /**
     * @brief Aktualizuje dane o kątach rotacji i wymusza natychmiastowe przerysowanie widgetu (update()).
     * @param[in] rotX -- Kąt przechylenia (Roll) wyrażony w stopniach.
     * @param[in] rotY -- Kąt pochylenia (Pitch) wyrażony w stopniach.
     */
    void setData(float rotX, float rotY);

    /**
     * @brief Slot aktualizujący kolory nieba, ziemi oraz linii podziałki przyrządu po zmianie motywu graficznego.
     */
    void changeTheme();

signals:

protected:

    /**
     * @brief Metoda odpowiedzialna za renderowanie grafiki przyrządu.
     * Implementuje rysowanie ruchomej tarczy, linii horyzontu, drabinki pochylenia oraz stałego
     * wskaźnika sylwetki samolotu, stosując transformacje obrotu i przesunięcia malarza.
     * @param[in] event -- Wskaźnik na obiekt zdarzenia rysowania QPaintEvent.
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Zwraca sugerowany, optymalny rozmiar widgetu dla systemów zarządzania układem (Layouts).
     * @return Obiekt QSize zawierający szerokość bazową i wysokość bazową przyrządu.
     */
    QSize sizeHint() const override;

private:

    float _rotX;         /**< Aktualny kąt pochylenia w stopniach, sterujący rotacją układu współrzędnych malarza. */
    float _rotY;         /**< Aktualny kąt przechylenia w stopniach, sterujący przesunięciem pionowym tarczy horyzontu. */
    int _baseWidth;      /**< Bazowa szerokość widgetu, wykorzystywana podczas obliczeń skalowania grafiki. */
    int _baseHeight;     /**< Bazowa wysokość widgetu, wykorzystywana podczas obliczeń skalowania grafiki. */

    QColor _topColor;    /**< Kolor górnej części przyrządu, symbolizującej niebo. */
    QColor _bottomColor; /**< Kolor dolnej części przyrządu, symbolizującej ziemię. */
};

#endif // VIRTUALHORIZON_H
