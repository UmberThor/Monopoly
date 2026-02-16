// UMBERTO CREMA - 2066681

#include "Cell.h"
#include "Player.h"
#include "Board.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

void game(std::vector<Player *> &players, Board &board, std::ofstream &log_file);
void money_based_win(std::vector<Player *> &players, Board &board, std::ofstream &log_file);
void order(std::vector<Player *> &players);
std::string file_name();

int main(int argc, char *argv[])
{
    // CONTROLLO DEGLI ARGOMENTI DA LINEA DI COMANDO
    if (argc != 2 || (std::string(argv[1]) != "computer" && std::string(argv[1]) != "human"))
    {
        std::cerr << "Il tipo di giocatore deve essere \"computer\" o \"human\"" << '\n';
        return 1;
    }

    // SETTAGGIO RANDOM
    srand(time(nullptr));

    // DICHIARAZIONE E RIMEPIMENTO DI PLAYERS
    std::vector<Player *> players = {
        new Computer("1", "\033[31m"),
        new Computer("2", "\033[32m"),
        new Computer("3", "\033[34m"),
    };
    if (std::string(argv[1]) == "computer")
        players.push_back(new Computer("4", "\033[33m"));
    else
        players.push_back(new Human("4", "\033[33m"));

    // ORDINAMENTO DI PLAYERS
    order(players);
    std::cout << "ORDINE DEI GIOCATORI IN SEGUITO AL LANCIO DEI DADI:" << '\n';
    for (Player *p : players)
        std::cout << "Giocatore " << p->name << ": " << p->first_throw << '\n';

    // CREAZIONE DI BOARD
    Board board(players);

    // CREAZIONE DEL FILE DI LOG E AVVIO GIOCO
    std::ofstream log_file;
    try
    {
        log_file.open(file_name() + ".txt");
        
        if (log_file.is_open())
            game(players, board, log_file);
        else
            throw std::runtime_error("Non ho potuto aprire il file.");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Eccezione: " << e.what() << '\n';
    }

    // LIBERAZIONE DELLA MEMORIA E CHIUSURA DEL FILE
    for (Player *player : players)
        delete player;
    // il distruttore di board viene chiamato in automatico

    log_file.close();

    return 0;
}

// GAME
// cuore del gioco, gestisce i turni dei giocatori
void game(std::vector<Player *> &players, Board &board, std::ofstream &log_file)
{
    // NUMERO MASSIMO E CONTATORE  DI TURNI
    int max_rounds = 200;
    int count_rounds = 0;

    while (players.size() > 1) // ripeti fino a quando c'è più di un giocatore: quando ce ne è solo uno, abbiamo il vincitore
    {
        count_rounds++;
        std::cout << "\033[95m" << "* * * * * * TURNO " << count_rounds << " * * * * * *" << "\033[0m" << '\n';

        for (int i = 0; i < players.size(); i++) // non minore di 4 perche la dimensione cambierà quando si eliminano i giocatori
        {
            int cell_index = players.at(i)->advance(log_file); // posizione del giocatore, servirà per evaluate_cell

            // chiedi se si vuole mostrare il tabellone solo se è il turno del giocatore umano
            if (players.at(i)->get_type() == 'H')
            {
                std::cout << "Digitare " << "\033[95m" << "show" << "\033[0m" << " per visualizzare il gioco, altro per continuare: ";
                std::string cmd = "";
                std::getline(std::cin, cmd);
                if (cmd == "show")
                {
                    board.show_board();
                    board.show_players_properties();
                    board.show_players_money();
                }
            }

            bool can_play = players.at(i)->evaluate_cell(board.get_cell_at(cell_index), board, log_file); // true se il giocatore può continuare a giocare, false altrimenti

            // se il giocatore non può giocare, viene tolto dal vector. essendo players di Board una reference, viene tolto anche da li
            if (!can_play)
            {
                players.at(i)->game_over();
                players.erase(players.begin() + i);
            }

            // se è rimasto un solo giocatore, allora ha vinto
            if (players.size() == 1)
            {
                std::cout << "Giocatore " << players.at(0)->name << " ha vinto la partita!" << '\n';
                log_file << "Giocatore " << players.at(0)->name << " ha vinto la partita!" << '\n';
                board.show_winner(players.at(0)->name);
            }
        }

        // siamo arrivati al numero massimo di turni, mostra il vincitore sulla base di chi ha più fiorini e poi termina
        if (count_rounds >= max_rounds)
        {
            money_based_win(players, board, log_file);
            return;
        }

        std::cout << '\n';
    }
}

// MONEY BASED WIN
// trova il giocatore con più fiorini, o i giocatori con più fiorini a pareggio e mostra il vincitore
void money_based_win(std::vector<Player *> &players, Board &board, std::ofstream &log_file)
{
    std::vector<int> winners; // vector di indici dei giocatori vincitori
    int max_money = -10;      // i giocatori avranno sicuramente money > -10
    for (int i = 0; i < players.size(); i++)
    {
        if (players.at(i)->get_money() > max_money)
        {
            max_money = players.at(i)->get_money(); // abbiamo trovato un nuovo massimo
            winners.clear();                        // i vincitori trovati prima vanno scartati
            winners.push_back(i);
        }
        else if (players.at(i)->get_money() == max_money) // abbiamo trovato un giocatore che pareggia con il massimo
            winners.push_back(i);
    }

    if (winners.size() == 1)
    {
        std::cout << "Giocatore " << players.at(winners.at(0))->name << " ha vinto la partita!" << '\n';
        log_file << "Giocatore " << players.at(winners.at(0))->name << " ha vinto la partita!" << '\n';
        board.show_winner(players.at(winners.at(0))->name);
    }
    else
    {
        std::cout << "I giocatori ";
        for (int winner : winners)
            std::cout << players.at(winner)->name << ", ";
        std::cout << "hanno vinto la partita ex-equo." << '\n'; // al punto 1.6 del testo del progetto questo messaggio non viene loggato
        board.show_winner("");
    }
}

// ORDER
// ordina i giocatori in base al lancio fatto
void order(std::vector<Player *> &players)
{
    for (Player *player : players)
        player->first_throw = throw_dice();

    bool has_dups = false;
    std::vector<int> dups;  // vector con gli indici dei giocatori che hanno pareggiato
    do
    {
        sort(players.begin(), players.end(), [](const Player *a, const Player *b)
             { return a->first_throw > b->first_throw; });

        has_dups = false;
        dups.clear();
        for (int i = 0; i < players.size() - 1; i++)
        {
            if (players.at(i)->first_throw == players.at(i + 1)->first_throw)
            {
                has_dups = true;
                if (dups.size() == 0 || dups.back() != i)
                    dups.push_back(i);
                dups.push_back(i + 1);
            }
        }

        for (int i : dups)
            players.at(i)->first_throw = throw_dice();

    } while (has_dups);
}

// FILE NAME
// restituisce una stringa con il nome del file di log in base a data e ora in cui viene eseguito il programma
std::string file_name()
{
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm time = *std::localtime(&current_time);
    int year = time.tm_year + 1900;
    int month = time.tm_mon + 1;
    int day = time.tm_mday;
    int hour = time.tm_hour;
    int minutes = time.tm_min;
    std::string file_name = "Monopoly_" + std::to_string(day) + "_" + std::to_string(month) + "_" + std::to_string(year) + "_" + std::to_string(hour) + "_" + std::to_string(minutes);
    return file_name;
}
