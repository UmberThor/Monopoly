# Monopoly
Final project for the Laboratorio di Programmazione course


COLORI DEI GIOCATORI
Durante la fase di sviluppo, ci è tornato utile assegnare a ciascun giocatore un colore, con il quale sarebbero state identificate le sue proprietà e gli altri dati relativi al giocatore. Abbiamo deciso di mantenere questa caratteristica nella versione finale del codice.
Per una migliore visualizzazione anche dei colori, consigliamo di eseguire il progetto con [specificare che tema].

NUMERO DEI FIORINI SBILANCIATO
Come riportato sul forum del progetto finale da altri studenti, i +20 fiorini a ogni passaggio dal via risultano troppi per avere una partita bilanciata. Con questa somma diventa infatti assai raro che un giocatore venga eliminato, praticamente impossibile che ne rimanga in gioco solo uno. La situazione che si instaura più spesso è quella invece che tutti i giocatori si arricchischino e guadagnino a ogni turno, in media, più di quanto spendono. Per questo, in fase di sviluppo, abbiamo assegnato la somma che viene data a un giocatore nel momento in cui passa dalla partenza a +3 fiorini, aumentando la probabilità di avere un vincitore.

ORDINAMENTO DEI GIOCATORI IN BASE AL LANCIO DEL DADO
La funzione order() è stata questione di dibattito, e ne sono state sviluppate nel tempo tre diverse versioni:
- versione 1: tutti i giocatori tirano i dadi. Se c'è un pareggio, fai lanciare di nuovo a tutti i giocatori
- versione 2 (corrente): tutti i giocatori tirano i dadi una prima volta, se ci sono pareggi, rilanciano i giocatori che hanno pareggiato. Si ripete finche non ci sono pareggi
 -versione 3 (implementata ma dubbia): tutti i giocatori tirano i dadi. I giocatori che non si trovano in pareggio mantengono fissa la loro posizione, il lanciare nuovamente i dadi va a cambiare l'ordine dei soli giocatori che hanno pareggiato

Nonostante la versione 3 è più affine alle regole ufficiali del gioco, la versione 2 è più conforme al testo del progetto, per questo la abbiamo lasciata come versione ufficiale. Riportiamo di seguito la versione 3:

void order(std::vector<Player*>& players, int from, int to)
{

    for (Player* p : players)
        p->first_throw = throw_dice();
    
    sort(players.begin() + from, players.begin() + to + 1, [](const Player *a, const Player *b)
         { return a->first_throw > b->first_throw; });

    // cerca se ci sono pareggi nella zona da from a to e li sistema
    bool has_duplicates = false;
    for (int i = from; i < to; i++)
    {
        if (!has_duplicates && players.at(i)->first_throw == players.at(i+1)->first_throw) {
            from = i;
            has_duplicates = true;
        }
        if (has_duplicates && players.at(i)->first_throw != players.at(i + 1)->first_throw)
            to = i;
    }
    if (has_duplicates)
        order(players, from, to);

    // cerca se ci sono pareggi dopo il to (utile quando ci sono due pareggi, ad esempio: 4 4 2 2)
    has_duplicates = false;
    from = to + 1;
    to = players.size() - 1;    // non ci possono essere più di due pareggi
    for (int i = from; i < to; i++)
    {
        if (!has_duplicates && players.at(i)->first_throw == players.at(i + 1)->first_throw)
        {
            from = i;
            has_duplicates = true;
        }
        if (has_duplicates && players.at(i)->first_throw != players.at(i + 1)->first_throw)
            to = i;
    }
    if (has_duplicates)
        order(players, from, to);
}
