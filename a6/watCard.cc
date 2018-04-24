#include "watCard.h"

WATCard::WATCard() : balance(0) {}

void WATCard::deposit( unsigned int amount ) {
    WATCard::balance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    WATCard::balance -= amount;
}

unsigned int WATCard::getBalance() {
    return WATCard::balance;
}
