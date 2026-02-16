#ifndef CELL_H
#define CELL_H

#include <string>

class Player;

enum building_type
{
  ground,
  house,
  hotel
};

class Cell
{
  public:

    const int index;
    const std::string coordinates;

    Cell(int i, std::string c, bool s);  // i è l'indice. costruttore pubblico perchè possiamo inzializzare oggetti Cell che non sono proprietà

    virtual bool is_property(); 
    virtual char get_type() const {

      if (is_start)
        return 'P';
      else
        return ' ';
    }

    int get_lodging_price();
    int get_upgrade_price();
    building_type get_building();
    int get_status(); // restituisce 0, 1, 2 in base al building:ground, house, hotel

    void upgrade(); // migliora la casa
    void reset();

    void set_owner(Player* o);
    Player *get_owner();

    // tutte le proprietà specificate al punto 1.4, che ogni sottoclasse deve ridefinire
    int land_price;
    int house_price;
    int hotel_price;
    int house_lodging_cost;
    int hotel_lodging_cost;

    bool is_property_b;
    bool is_start;

  protected:
    Player *owner = nullptr;
    building_type building = ground; // "ground", "house", "hotel"
};

class Economy : public Cell
{
	public:
    Economy(int i, std::string c);
    char get_type() const { return 'E'; };
};

class Standard : public Cell
{
	public:
    Standard(int i, std::string c);
    char get_type() const { return 'S'; };
};

class Deluxe : public Cell
{
	public:
    Deluxe(int i, std::string c);
    char get_type() const { return 'L'; };
};

#endif