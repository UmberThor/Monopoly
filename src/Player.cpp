// UMBERTO CREMA - 2066681

#include "Player.h"
#include "Cell.h"
#include <iostream>
#include <limits>

// COSTRUTTORI

Player::Player(std::string n, std::string c)
    : name{n}, color{c}
{
}

Player::Player()
{
}

Computer::Computer(std::string n, std::string c)
    : Player{n, c}
{
}

Human::Human(std::string n, std::string c)
    : Player{n, c}
{
}

// GETTERS

int Player::get_money() const
{
    return money;
}

int Player::get_current_cell_index() const
{
    return current_cell_index;
}

const std::vector<Cell*>& Player::get_properties() const
{
    return properties;
}

// ADVANCE
// il giocatore lancia il dado e avanza. Se passa per la casella 14, ossia quella di partenza, riceve 20 fiorini
int Player::advance(std::ofstream &log_file)
{
    int dice = throw_dice();
    std::cout << "Giocatore " << name << " ha tirato i dadi ottenendo un valore di " << dice << '\n';
    log_file << "Giocatore " << name << " ha tirato i dadi ottenendo un valore di " << dice << '\n';

    // è in una cella prima della partenza e avanzando andrà nella partenza o dopo
    if (current_cell_index < 14 && current_cell_index+dice >= 14)
    {
        money += 20;
        std::cout << "Giocatore " << name << " è passato dal via e ha ritirato 20 fiorini" << '\n';
        log_file << "Giocatore " << name << " è passato dal via e ha ritirato 20 fiorini" << '\n';
    }

    // aumenta l'indice del risultato del dado, se maggiore di 28 allora hai completato un giro
    current_cell_index += dice;
    current_cell_index %= 28;

    std::cout << "Giocatore " << name << " è arrivato alla casella " << convert_coordinates(current_cell_index) << '\n';
    log_file << "Giocatore " << name << " è arrivato alla casella " << convert_coordinates(current_cell_index) << '\n';
    return current_cell_index;
}

// GAME OVER
// resetta tutte le celle proprietà del giocatore
void Player::game_over()
{
    for (Cell *cell : properties)
        cell->reset();
}

// EVALUATE CELL
// in base alla cella ricevuta, compie diverse azioni
bool Player::evaluate_cell(Cell *cell, const Board& board, std::ofstream &log_file)
{
    // la cella è angolare
    if (!cell->is_property())
    {
        std::cout << "Giocatore " << name << " ha finito il turno" << "\n";
        log_file << "Giocatore " << name << " ha finito il turno" << "\n";
        return true;
    }

    // la proprietà non ha un proprietario
    if (cell->get_owner() == nullptr)
    {
        if (money > cell->land_price)
            buy(cell, board, log_file);
    }

    // la proprietà è del giocatore
    else if (cell->get_owner() == this)
    {
        if ((cell->get_building() == ground || cell->get_building() == house) && money > cell->get_upgrade_price())
            upgrade(cell, board, log_file);
    }

    // la proprietà è di un altro giocatore
    else
        return pay(cell, log_file);

    std::cout << "Giocatore " << name << " ha finito il turno" << "\n";
    log_file << "Giocatore " << name << " ha finito il turno" << "\n";
    return true;
}

// PAY
// paga al proprietario della cella il prezzo del pernottamento. Se non riesce a pagare, da tutti i suoi fiorini al proprietario e perde la partita 
bool Player::pay(Cell *cell, std::ofstream &log_file)
{
    // riesce a pagare
    if (money >= cell->get_lodging_price())
    {
        if (cell->get_building() != ground) // solo se c'è una casa o un albergo
        {
            money -= cell->get_lodging_price();
            cell->get_owner()->money += cell->get_lodging_price();
            std::cout << "Giocatore " << name << " ha pagato " << cell->get_lodging_price() << " fiorini a giocatore " << cell->get_owner()->name << " per pernottamento nella casella " << cell->coordinates << "\n";
            log_file << "Giocatore " << name << " ha pagato " << cell->get_lodging_price() << " fiorini a giocatore " << cell->get_owner()->name << " per pernottamento nella casella " << cell->coordinates << "\n";
        }
        return true;    // il giocatore ha pagato e può continuare a giocare
    }

    // non riesce a pagare
    else
    {
        cell->get_owner()->money += money;
        std::cout << "Giocatore " << name << " ha pagato " << money << " fiorini a giocatore " << cell->get_owner()->name << " per pernottamento nella casella " << cell->coordinates << "\n";
        log_file << "Giocatore " << name << " ha pagato " << money << " fiorini a giocatore " << cell->get_owner()->name << " per pernottamento nella casella " << cell->coordinates << "\n";
        money = 0;
        std::cout << "Giocatore " << name << " è stato eliminato" << "\n";
        log_file << "Giocatore " << name << " è stato eliminato" << "\n";
        return false; // il giocatore non riesce a pagare e viene eliminato
    }
}

// BUY
// l'utente decide se acquistare o meno il terreno senza proprietario
void Player::buy(Cell *cell, const Board &board, std::ofstream &log_file)
{
    if (this->make_decision(board, "Vuoi acquistare il terreno? "))
    {
        money -= cell->land_price;
        cell->set_owner(this);
        properties.push_back(cell);
        std::cout << "Giocatore " << name << " ha acquistato il terreno " << cell->coordinates << "\n";
        log_file << "Giocatore " << name << " ha acquistato il terreno " << cell->coordinates << "\n";
    }
}

// UPGRADE
// l'utente decide se migliorare o meno la proprietà
void Player::upgrade(Cell *cell, const Board &board, std::ofstream &log_file)
{
    std::string msg = "Vuoi migliorare la proprietà? ";
    if (cell->get_building() == ground)
        msg = "Vuoi costruire una casa? ";
    else if (cell->get_building() == house)
        msg = "Vuoi migliorare la casa in albergo? ";

    if (this->make_decision(board, msg))
    {
        money -= cell->get_upgrade_price();
        cell->upgrade();
        if (cell->get_building() == house)
        {
            std::cout << "Giocatore " << name << " ha costruito una casa sul terreno " << cell->coordinates << "\n";
            log_file << "Giocatore " << name << " ha costruito una casa sul terreno " << cell->coordinates << "\n";
        }
        else if (cell->get_building() == hotel)
        {
            std::cout << "Giocatore " << name << " ha migliorato una casa in albergo sul terreno " << cell->coordinates << "\n";
            log_file << "Giocatore " << name << " ha migliorato una casa in albergo sul terreno " << cell->coordinates << "\n";
        }
    }
}

// MAKE DECISION
// Human: mostra il messaggio per l'utente e aspetta il suo input. Se l'input è "show", mostra il gioco
bool Human::make_decision(const Board &board, std::string msg)
{
    std::cout << msg;
    std::string yn = "0";
    std::cin >> yn;

    while (yn.size() != 1 || (std::tolower(yn[0]) != 's' && std::tolower(yn[0]) != 'n'))
    {

        if (yn == "show")
        {
            board.show_board();
            board.show_players_properties();
            board.show_players_money();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (yn != "show")
            std::cout << "Risposta non valida, richiesto s/n : " << '\n';
        std::cout << msg;
        std::cin >> yn;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');     // ripulisce il buffer di cin

    return (std::tolower(yn[0]) == 's');
}

// Computer: ritorna true con probabilità del 25%
bool Computer::make_decision(const Board &board, std::string msg)
{
    int r = rand() % 100;
    return (r % 4 == 0); // 25%;
}

// THROW DICE
// restituisce la somma dei due dadi (numeri casuali da 1 a 6)
int throw_dice()
{
    int dice_1 = rand() % 6 + 1;
    int dice_2 = rand() % 6 + 1;
    return dice_1 + dice_2;
}