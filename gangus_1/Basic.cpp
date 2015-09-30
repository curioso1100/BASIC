/*
 * File: Basic.cpp
 * ---------------
 * Name: GEOFFREY ANGUS
 * Section: ADITYA BANDHARI
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "console.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
#include "statement.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void help();

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Welcome to BASIC!" << endl;
   while (true) {
      try {
           //processes the line
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
           //expressions thrown within the program are handled
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
    //creates a scanner that takes in input
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   //ensures that the token is uppercase
   string next = toUpperCase(scanner.nextToken());
   //if it is a number or symbol
   if (scanner.getTokenType(next) != WORD) {
       //assigns the command line number
       int nextNumber = stringToInteger(next);
       if (!scanner.hasMoreTokens()) {
           //removes the saved memory in line if only the number is called
           program.removeSourceLine(nextNumber);
           return;
       }
       //stores the source line in the list
       program.addSourceLine(nextNumber,line);
       //sets the statement
       program.setParsedStatement(nextNumber,parseStatement(scanner));
   }
   else if (next == "RUN") {
       //error if there is nothing to run
       if (program.isEmpty()) {
           error("Program cannot be run");
       }
       //assigns the first line number
       int firstLineNumber = program.getFirstLineNumber();
       //sets the state line number to what is assigned
       state.setCurrentLineNumber(firstLineNumber);
       //runs the program
       program.run(firstLineNumber, state);
   }
   else if (next == "LIST") program.list();
   else if (next == "HELP") help();
   else if (next == "CLEAR") {
       //clears the program map
       program.clear();
       //clears the state map
       state.clear();
   }
   else if (next == "QUIT") exit(0);
   else {
       if (next == "REM") {
           cout << "Line number required" << endl;
           return;
       }
       //restores the first token
       scanner.saveToken(next);
       parseStatement(scanner)->execute(state);
   }
}

void help() {
    cout << "Available commands:" << endl;
    cout << "  RUN - Runs the program" << endl;
    cout << "  LIST - Lists the program" << endl;
    cout << "  CLEAR - Clears the program" << endl;
    cout << "  HELP -- Prints this message" << endl;
    cout << "  QUIT - Exits from the BASIC interpreter" << endl;
}
