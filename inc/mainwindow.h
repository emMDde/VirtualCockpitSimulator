#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QScreen>
#include <QStackedWidget>
#include "mainpage.h"
#include "datapage.h"
#include "controllermanager.h"

/**
 * @class MainWindow
 * @brief Główne okno aplikacji.
 *
 * Klasa odpowiedzialna zarządzanie aplikacją, jej wydzielonymi częściami.
 * Obsługuje ona komunikację z urządzniem zewnętrznym - kotrnolerem i obłsuguje sygnały.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    /**
     * @brief Konstruktor klasy MainWindow.
     * * Inicjalizuje układ okienka, rozmiary i elementy pochodne.
     * @param parent Wskaźnik na widget nadrzędny (domyślnie nullptr).
     */
    explicit MainWindow(QWidget *parent = nullptr);

private slots:


protected:

    void changeEvent(QEvent *event) override;

private:

    int _windowWidth;
    int _windowHeight;
    ControllerManager *_controller;

    /**
     * @brief Funkcja deklarująca główne składowe okna głównego
     * Odpowiada ułożenie elementów, utworzenie górnego i dolnego paska
     * oraz przestrzeni symulacyjnej.
     */
    void createLayout();

    /**
     * @brief Odświeża tytuł okna przy użyciu aktualnego translatora.
     */
    void reTranslate();


};

#endif // MAINWINDOW_H
