#ifndef VIRTUALHORIZON_H
#define VIRTUALHORIZON_H

#include <QObject>
#include <QWidget>
#include <QPainter>

/**
 * @class VirtualHorizon
 * @brief Widget wizualizujący sztuczny horyzont.
 *
 * Klasa odpowiedzialna za graficzne rysowanie wirtualnego przyrządu pokładowego.
 * Prezentuje orientację w przestrzeni, symulując ruch tarczy żyroskopu i pokazując
 * przechylenie (Roll - rotX) oraz pochylenie (Pitch - rotY) w oparciu o dane z kontrolera.
 */
class VirtualHorizon : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor klasy VirtualHorizon.
     * * Inicjalizuje widget sztucznego horyzontu i ustala jego stałe rozmiary.
     * @param height Wysokość widgetu w pikselach.
     * @param width Szerokość widgetu w pikselach.
     * @param parent Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit VirtualHorizon(int height, int width, QWidget *parent = nullptr);

public slots:
    /**
     * @brief Aktualizuje dane o kątach rotacji i wymusza przerysowanie widgetu.
     * * @param rotX Kąt przechylenia (Roll) wyrażony w stopniach.
     * @param rotY Kąt pochylenia (Pitch) wyrażony w stopniach.
     */
    void setData(float rotX, float rotY);

signals:

protected:
    /**
     * @brief Główna funkcja rysująca sztuczny horyzont.
     * * Przeciążona metoda z klasy QWidget. Odpowiada za nałożenie wyrysowanie
     * kształtów oraz nałożenie odpowiednich transformacji matematycznych zgodnie z danymi obrotu.
     * * @param event Wskaźnik na obiekt zdarzenia rysowania (QPaintEvent).
     */
    void paintEvent(QPaintEvent *event) override;

    QSize sizeHint() const override;

private:
    float _rotX; /**< Przechowuje aktualny kąt przechylenia. */
    float _rotY; /**< Przechowuje aktualny kąt pochylenia. */
    int _baseWidth;
    int _baseHeight;
};

#endif // VIRTUALHORIZON_H
