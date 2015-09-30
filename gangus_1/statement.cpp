/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "tokenscanner.h"
#include "exp.h"
#include "evalstate.h"
#include "simpio.h"
#include "program.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

/*
 * Constructor: PrintStmt
 * -------------------------------------------------
 * Prints the expression
 */

PrintStmt::PrintStmt(TokenScanner & scanner) {
    //creates an expression with the scanner
    exp = readE(scanner, 0);
    if (scanner.hasMoreTokens()) {
        error("Extraneous token " + scanner.nextToken());
    }
}

/*
 * Destructor: PrintStmt
 * -------------------------------------------------
 * Deletes the exp pointer
 */

PrintStmt::~PrintStmt() {
    delete exp;
}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * prints the evaluated expression
 */

void PrintStmt::execute(EvalState & state) {
    cout << exp->eval(state) << endl;
};

/*
 * Constructor: LetStmt
 * -------------------------------------------------
 * Assigns a variable to an expression
 */

LetStmt::LetStmt(TokenScanner & scanner) {
    string firstWord = scanner.nextToken();
    char firstChar = firstWord[0];
    //checks if the word consists of letters
    if (!isalpha(firstChar)) {
        error ("Not valid input");
    }
    IdentifierExp *identifier = new IdentifierExp(firstWord);
    //puls the assignment operator
    string assignment = scanner.nextToken();
    if (assignment != "=") {
        error ("Not an assignment operator");
    }
    //creates an expression
    exp = readE(scanner, 0);
    if (scanner.hasMoreTokens()) {
        error("Extraneous token " + scanner.nextToken());
    }
    //sets the instance variable to the identifier
    variable = identifier;
}

/*
 * Destructor: LetStmt
 * -------------------------------------------------
 * Deletes exp and variable
 */

LetStmt::~LetStmt() {
    delete exp;
    delete variable;
}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * creates a value in the map
 */

void LetStmt::execute(EvalState & state) {
    state.setValue(variable->getName(),exp->eval(state));
};

/*
 * Constructor: RemStmt
 * -------------------------------------------------
 * Adds comments to the program
 */

RemStmt::RemStmt(TokenScanner & scanner) {}
RemStmt::~RemStmt() {}
void RemStmt::execute(EvalState & state) {};

/*
 * Constructor: InputStmt
 * -------------------------------------------------
 * Takes user input and assigns it to a variable
 */

InputStmt::InputStmt(TokenScanner & scanner) {
    string inputString = scanner.nextToken();
    char firstChar = inputString[0];
    //checks if it's a word
    if (!isalpha(firstChar)) {
        error ("Not valid input");
    }
    IdentifierExp * inputVariable = new IdentifierExp(inputString);
    if (scanner.hasMoreTokens()) {
        error("Extraneous token " + scanner.nextToken());
    }
    variable = inputVariable;
}

/*
 * Destructor: InputStmt
 * -------------------------------------------------
 * Deletes pointers constant and variable
 */

InputStmt::~InputStmt() {
    delete constant;
    delete variable;
}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * creates a value in the map with user input
 */

void InputStmt::execute(EvalState & state) {
    //takes user input
    int firstInt = getInteger(" ? ");
    ConstantExp * inputValue = new ConstantExp(firstInt);
    constant = inputValue;
    //puts the value in the map
    state.setValue(variable->getName(),constant->getValue());
};

/*
 * Constructor: EndStmt
 * -------------------------------------------------
 * Ends the program
 */

EndStmt::EndStmt() {}

EndStmt::~EndStmt() {}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * sets the line number to the end
 */

void EndStmt::execute(EvalState &state) {
    state.setCurrentLineNumber(-1);
};

/*
 * Constructor: GotoStmt
 * -------------------------------------------------
 * Jumps to a new line
 */

GotoStmt::GotoStmt(TokenScanner & scanner) {
    string potentialNumber = scanner.nextToken();
    newLineNumber = stringToInteger(potentialNumber);
}

GotoStmt::~GotoStmt() {}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * sets the line number to the new number
 */

void GotoStmt::execute(EvalState & state) {
    state.setCurrentLineNumber(newLineNumber);
};

/*
 * Constructor: IfStmt
 * -------------------------------------------------
 * a conditional that if true, goes to another line
 */

IfStmt::IfStmt(TokenScanner & scanner) {
    //gets the left side expression
    exp1 = readE(scanner, 0);
    //gets the operator
    op = scanner.nextToken();
    //gets the right side expression
    exp2 = readE(scanner, 0);
    //checks if there is a THEN after the expression
    if (toUpperCase(scanner.nextToken()) == "THEN") {
        newLineNumber = stringToInteger(scanner.nextToken());
    }
}

/*
 * Destructor: InputStmt
 * -------------------------------------------------
 * Deletes pointers exp1 and exp2
 */

IfStmt::~IfStmt() {
    delete exp1;
    delete exp2;
}

/*
 * Method: execute(state)
 * -------------------------------------------------
 * checks the boolean operator, goes to a new line
 */

void IfStmt::execute(EvalState & state) {
    //evaluates the left side
    int first = exp1->eval(state);
    //evaluates the right side
    int second = exp2->eval(state);
    if (op == "=") {
        if (first == second) {
            state.setCurrentLineNumber(newLineNumber);
        }
    }
    else if (op == ">") {
        if (first > second) {
            state.setCurrentLineNumber(newLineNumber);
        }
    }
    else if (op == "<") {
        if (first < second) {
            state.setCurrentLineNumber(newLineNumber);
        }
    }
    //moves it to the end
    else state.setCurrentLineNumber(-1);
};

