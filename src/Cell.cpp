#include "Cell.h"
#include "Player.h"
#include <iostream>

Cell::Cell(int i, std::string c, bool s)
    : index{i}, coordinates{c}, is_start{s}
{}

bool Cell::is_property()
{
    return is_property_b; 
}

int Cell::get_lodging_price()
{
    if (building == ground)
        return 0;
    else if (building == house)
        return house_lodging_cost;
    else
        return hotel_lodging_cost;
}

int Cell::get_upgrade_price()
{
    if (building == ground)
        return house_price;
    else if (building == house)
        return hotel_price;

    return 0;
}

building_type Cell::get_building()
{
    return building;
}

int Cell::get_status() //mi seve lo status per capire cos'ho costruito su cell
{
	bool cmp = building == ground;
	if (cmp)
		return 0;

	cmp = building == house;
	if (cmp)
		return 1;
		
	return 2;
}

void Cell::upgrade() //faccio l'upgrade di price, building e lodging
{
  try {
   	int status = get_status();
   		
	if (status == 0)
		building = house;
	else if (status == 1)
		building = hotel;
		
	
  	else throw std::logic_error("You cannot upgrade an hotel");
	
   	} 
	catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
	}  
}

void Cell::reset()
{
    owner = nullptr;
    building = ground;
}

void Cell::set_owner(Player* o) {
    owner = o;
}

Player* Cell::get_owner() {
    return owner;
}

Economy::Economy(int i, std::string c)
    : Cell(i,c, false)
{	
    land_price = 6; 	
    house_price = 3;
    hotel_price = 3;
    house_lodging_cost = 2;
    hotel_lodging_cost = 4;
    is_property_b = true;
}

Standard::Standard(int i, std::string c)
    : Cell(i,c, false)
{
    land_price = 10;
    house_price = 5;
    hotel_price = 5;
    house_lodging_cost = 4;
    hotel_lodging_cost = 8;
    is_property_b = true;
}

Deluxe::Deluxe(int i, std::string c)
    : Cell(i,c, false)
{
    land_price = 20;
    house_price = 10;
    hotel_price = 10;
    house_lodging_cost = 7;
    hotel_lodging_cost = 14;
    is_property_b = true;
}