 /*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"
#include "map.h"
using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
   currentLineNumber = 0;
}

EvalState::~EvalState() {
}

void EvalState::setValue(string var, int value) {
   symbolTable.put(var, value);
}

int EvalState::getValue(string var) {
   return symbolTable.get(var);
}

bool EvalState::isDefined(string var) {
   return symbolTable.containsKey(var);
}

/*
* Method: getCurrentLineNumber
* Usage: int lineNumber = state.getCurrentLineNumber();
* ---------------------------------------
* Returns instance variable currentLineNumber
*/

int EvalState::getCurrentLineNumber() {
    return currentLineNumber;
};

/*
* Method: setCurrentLineNumber
* Usage: setCurrentLineNumber(newLineNumber);
* ---------------------------------------
* Changes instance variable currentLineNumber
*/

void EvalState::setCurrentLineNumber(int newLineNumber) {
    currentLineNumber = newLineNumber;
};

/*
* Method: clear
* Usage: clear();
* ---------------------------------------
* Clears map symbolTable
*/

void EvalState::clear(){
    symbolTable.clear();
}
