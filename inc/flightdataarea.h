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

class FlightDataArea : public QFrame
{
    Q_OBJECT
public:
    explicit FlightDataArea(int height, int width, QWidget *parent = nullptr);

signals:
    void updateSimData(float rotX, float rotY);

public slots:
    void changeTheme();

protected:
    /**
     * @brief Reaguje na zmiany systemowe, nadpisano funkcjonalność zmiany języka aplikacji.
     */
    void changeEvent(QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief Aktualizuje dane o kątach rotacji i wymusza przerysowanie widgetu.
     * * @param rotX Kąt przechylenia (Roll) wyrażony w stopniach.
     * @param rotY Kąt pochylenia (Pitch) wyrażony w stopniach.
     */
    void setData(float rotX, float rotY);

    void togglePause();
    void clearData();

private:
    VirtualHorizon *_horizon;
    QLabel* _titleLabel;         // Tytuł do łatwego tłumaczenia tr()
    QFrame* _layoutContainer;    // Główny kontener na szary panel
    QWidget* _xGraphSpace;    // Pusty widget rezerwujący miejsce na wykres Roll
    QWidget* _yGraphSpace;   // Pusty widget rezerwujący miejsce na wykres Pitch

    QList<float> _rollHistory;
    QList<float> _pitchHistory;
    long long _totalDataPoints = 0; // Liczy absolutnie wszystkie punkty od startu
    float _sampleRate = 50.0f;
    int _maxDataPoints;
    bool _isPaused;

    QPushButton* _pauseBtn;
    QPushButton* _clearBtn;

    QColor _bgColor;
    QColor _lineColor;
    QColor _groundColor;
    QColor _panelColor;
    QColor _textColor;
    QColor _graphBgColor;
    QColor _graphTextColor;


    /**
     * @brief Odświeża tytuł okna przy użyciu aktualnego translatora.
     */
    void reTranslate();

    void drawCloud(QPainter &painter, int x, int y, int cW, int cH);

    void drawGraph(QPainter &painter, const QRect& rect, const QList<float>& data, QColor color, float maxVal, const QString& topTxt, const QString& botTxt);
};

#endif // FLIGHTDATAAREA_H