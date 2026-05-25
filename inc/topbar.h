#ifndef TOPBAR_H
#define TOPBAR_H

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
#include "toolbar.h"
/**
 * @class TopBar
 * @brief Górny pasek nawigacyjny aplikacji.
 *
 * Klasa odpowiedzialna za wyświetlanie i obsługę górnego paska interfejsu.
 * Zawiera przyciski sterujące motywem, językiem oraz wizualny wskaźnik (diodę)
 * statusu połączenia z kontrolerem STM32.
 */
class TopBar : public ToolBar
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor klasy TopBar.
     * * Inicjalizuje układ, przyciski i diodę LED oraz ustala ich rozmiary.
     * @param parent Wskaźnik na widget nadrzędny (domyślnie nullptr).
     * @param height Wysokość paska w pikselach.
     * @param width Szerokość paska w pikselach.
     */
    explicit TopBar(QWidget *parent = nullptr, int height = 60, int width = 1280);

private:

};

#endif // TOPBAR_H
