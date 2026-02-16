//2073910 Fogale Andrea

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <array>
#include <string>

class Cell;   // Forward declaration for Cell
class Player; // Forward declaration for Player


// BOARD È UN INSIEME DI CELLE E DI GICOCATORI
class Board
{
  public:
  
  //Costruttori/Distruttore
    Board();    
    Board(std::vector<Player*>& p);
    ~Board();

    void show_board() const;                                       	//Funzioni che permettono di visualizzare il tabellone di gioco, le proprieta' e i fiorini di ciascun giocatore (come indicato al punto 1.5 del file guida)
    void show_players_properties() const;    							
    void show_players_money() const;         							
    void show_winner(std::string s) const;							//Funzione per la stampa a video dei Graffiti contenenti il vincitore

    void initialize_cells(int c_e, int c_s, int c_d);      		//Funzioni utili all'inizializzazione e gestione delle caselle
    void set_property_type(Cell c, int c_e, int c_s, int c_d); 
    void set_cell_display(int i, bool tabulated);
    Cell* get_cell_at(int i);

  private:
    std::vector<Player *> &players;
    std::vector<Cell*> cells;
};

std::array<int, 2> get_coordinates(int i);
std::string convert_coordinates(int pos);

#endif
