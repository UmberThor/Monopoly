// UMBERTO CREMA - 2066681

#ifndef PLAYER_H
#define PLAYER_h

#include "Board.h"
#include <fstream>

class Player  // non possono venir creati oggetti ti tipo Player ==> funzioni virtuali pure
{
  public:
    // COSTRUTTORI
    Player();
    Player(std::string n, std::string c);

    // GETTERS
    int get_money() const;
    int get_current_cell_index() const;
    const std::vector<Cell *>& get_properties() const;

    // FUNZIONI MEMBRO PER MUOVERE IL GIOCATORE E OPERARE SULLE PROPRIETÀ
    int advance(std::ofstream& log_file);
    virtual bool evaluate_cell(Cell *c, const Board &board, std::ofstream & log_file);

    // GAME OVER PER LIBERARE LE PROPRIETA
    void game_over();

    // PER IL MAIN PER SAPERE SE È UMANO O COMPUTER
    virtual char get_type() = 0;

    // const: una volta assegnati nome e colore, non possono essere modificati
    const std::string name;         // stringa se si vogliono mettere nomi quali "C1", "H2"
    const std::string color;        // colore identificativo del giocatore

    int first_throw;                // serve per determinare l'ordine iniziale dei giocatori

  private:
    int money = 100;                // iniziano tutti con 100 fiorini
    int current_cell_index = 14;    // la cella di partenza è la 14
    std::vector<Cell *> properties;

    bool pay(Cell *c, std::ofstream &log_file);
    void buy(Cell *c, const Board &board, std::ofstream &log_file);
    void upgrade(Cell *c, const Board &board, std::ofstream &log_file);

  protected:
    virtual bool make_decision(const Board &board, std::string msg = "") = 0;
};

class Human : public Player
{
  public:
    Human();
    Human(std::string n, std::string c);
    char get_type() override { return 'H'; }

  private:
    bool make_decision(const Board &board, std::string msg = "") override;
};

class Computer : public Player
{
  public:
    Computer();
    Computer(std::string n, std::string c);
    char get_type() override { return 'C'; }
  private:
    bool make_decision(const Board &board, std::string msg = "") override;
};

int throw_dice();

#endif