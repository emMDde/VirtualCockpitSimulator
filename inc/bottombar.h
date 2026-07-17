#ifndef BOTTOMBAR_H
#define BOTTOMBAR_H

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
 * @class BottomBar
 * @brief Dolny pasek nawigacyjny aplikacji, dla zakładki wizualizacji danych.
 *
 * Klasa odpowiedzialna za wyświetlanie i obsługę dolnego paska interfejsu.
 * Zawiera przyciski sterujące motywem, językiem oraz wizualny wskaźnik (diodę)
 * statusu połączenia z kontrolerem STM32.
 */
class BottomBar : public ToolBar
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy BottomBar.
     * Przekazuje parametry konfiguracyjne bezpośrednio do konstruktora klasy bazowej ToolBar,
     * inicjalizuje dedykowany układ dolnego panelu oraz przypisuje klucz tekstowy dla przycisku powrotu.
     * @param[in, out] parent -- Wskaźnik na widget nadrzędny (domyślnie nullptr).
     * @param[in] height -- Wysokość paska wyrażona w pikselach (domyślnie 60).
     * @param[in] width -- Szerokość paska wyrażona w pikselach (domyślnie 1280).
     */
    explicit BottomBar(QWidget *parent = nullptr, int height = 60, int width = 1280);


private:

};

#endif // BOTTOMBAR_H
