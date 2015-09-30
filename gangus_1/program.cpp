/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "evalstate.h"
using namespace std;

Program::Program() {
    head = NULL;
    count = 0;

}

Program::~Program() {
    clear();
}

void Program::clear() {
    count = 0;
    map.clear();
}

bool Program::isEmpty() {
    return count == 0;
}

/*
 * Method: run
 * Usage: program.run(lineNumber, state);
 * -------------------------------------------------
 * runs the program and syncs the line numbers in state and program
 */

void Program::run(int lineNumber, EvalState & state) {
    //sets the state line number to the line number passed in
    state.setCurrentLineNumber(lineNumber);
    //executes the statement
    getParsedStatement(lineNumber)->execute(state);
    while (lineNumber != -1) {
        //checks if the numbers are in sync
        if (lineNumber == state.getCurrentLineNumber()) {
            //moves to next line number
            lineNumber = getNextLineNumber(lineNumber);
            //changes state to the new line number
            state.setCurrentLineNumber(lineNumber);
        }
        else {
            //syncs the numbers
            lineNumber = state.getCurrentLineNumber();
        }
        //double checks in case numbers are updated
        if (lineNumber != -1) {
            //executes statement
            getParsedStatement(lineNumber)->execute(state);
        }
    }
    //clears variables
    state.clear();
}

/*
 * Method: list
 * Usage: program.list();
 * -------------------------------------------------
 * lists the command lines in order
 */

void Program::list() {
    lineCommand *current = head;
    while (current != NULL) {
        if(map.containsKey(current->lineNumber)) {
            cout << current->line << endl;
        }
        current = current->link;
    }
}

/*
 * Method: addSourceLine
 * Usage: addSourceLine(lineNumber,line(;
 * -------------------------------------------------
 * adds command lines to the linked list
 */

void Program::addSourceLine(int lineNumber, string line) {
    //creates a new element that doesn't point to anything
    lineCommand *newCommand = new lineCommand;
    newCommand->lineNumber = lineNumber;
    //creates a new element that will be doing iterating
    lineCommand *current;
    //checks if the line is a command
    if (!isCommand(line)) {
        error("Not a command");
    }
    newCommand->line = line;
    newCommand->link = NULL;
    //removes code with the same line number
    if (map.containsKey(lineNumber)) {
        removeSourceLine(lineNumber);
    }
    //adds the command line to the map
    map.put(lineNumber,newCommand);
    //if its the first element, make it the head
    if (isEmpty()) {
        head = newCommand;
        current = head;
    }
    else {
        //creates a pointer that points at the front
        current = head;
        //if the new element's priority is more urgent than the head's, make it the head
        if (lineNumber < current->lineNumber) {
            newCommand->link = current;
            head = newCommand;
        }
        //else look for the spot where it belongs
        else {
            //while it isn't pointing at the end
            while (current->link != NULL) {
                //if the element's priority is more urgent than the current and less than the next
                if (lineNumber >= current->lineNumber && lineNumber < current->link->lineNumber) {
                    //set the new element's link to the current's link
                    newCommand->link = current->link;
                    //change the current link to the element
                    current->link = newCommand;
                    count++;
                    return;
                }
                //updates the iteration
                current = current->link;
            }
            //if it doesn't return, add it to the end
            current->link = newCommand;
        }
    }
    count++;
}

/*
 * Method: removeSourceLine
 * Usage: removeSourceLine(lineNumber);
 * -------------------------------------------------
 * removes command lines
 */

void Program::removeSourceLine(int lineNumber) {
    //creates an element at the beginning
    lineCommand *current = head;
    //checks if the map is empty or doesn't have the line number
    if (!map.containsKey(lineNumber) || count == 0) error("Cannot remove line");;
    //removes one from the total count
    count--;
    //condition if it's the first one
    if (current->lineNumber == lineNumber && current == head) {
        //moves head to the next
        head = current->link;
        map.remove(lineNumber);
        delete current;
        return;
    }
    while (current->link != NULL) {
        //condition if it's the last one
        if (current->link->lineNumber == lineNumber && current->link->link == NULL) {
            map.remove(lineNumber);
            //deletes the next one
            current->link = NULL;
            delete current->link;
            return;
        }
        //condition if it's the middle one
        else if (current->link->lineNumber == lineNumber) {
            lineCommand *remove = current->link;
            lineCommand *new_next = remove->link;
            //links the current to the one after the next
            current->link = new_next;
            remove->link = NULL;
            map.remove(lineNumber);
            return;
        }
        //updates the iteration
        current = current->link;
    }
}

/*
 * Method: getSourceLine
 * Usage: getSourceLine(lineNumber);
 * -------------------------------------------------
 * gets the source line at the number given
 */

string Program::getSourceLine(int lineNumber) {
    return map.get(lineNumber)->line;
}

/*
 * Method: setParsedStatement
 * Usage: setParsedStatement(int lineNumber, stmt);
 * -------------------------------------------------
 * sets the statement
 */

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    //checks if the number exists in the map
    if (map.containsKey(lineNumber)) {
        map.get(lineNumber)->stmt = stmt;
    }
    else {
        error("Cannot access key");
    }
}

/*
 * Method: getParsedStatement
 * Usage: getParsedStatement(int lineNumber);
 * -------------------------------------------------
 * gets the statement from the map
 */

Statement *Program::getParsedStatement(int lineNumber) {
    Statement *parsedStatement;
    //checks if the number exists in the map
    if (map.containsKey(lineNumber)) {
        parsedStatement = map.get(lineNumber)->stmt;
    }
    else {
        error("Cannot access key");
    }
    return parsedStatement;
}

/*
 * Method: getFirstLineNumber
 * Usage: getFirstLineNumber();
 * -------------------------------------------------
 * gets the first line number
 */

int Program::getFirstLineNumber() {
    return head->lineNumber;
}

/*
 * Method: getNextLineNumber
 * Usage: getNextLineNumber();
 * -------------------------------------------------
 * gets the line after the first
 */

int Program::getNextLineNumber(int lineNumber) {
    int nextLineNumber;
    //checks if the number exists in the map
    if (map.containsKey(lineNumber)) {
        //if it's the last one, make it -1
        if (map.get(lineNumber)->link == NULL) {
            return -1;
        }
        //assigns the next number to the link
        nextLineNumber = map.get(lineNumber)->link->lineNumber;
        return nextLineNumber;
    }
    return -1;
}

/*
 * Method: isCommand
 * Usage: isCommand(line);
 * -------------------------------------------------
 * checks that the line is a command
 */

bool Program::isCommand(string line) {
    TokenScanner scanner;
    scanner.setInput(line);
    scanner.ignoreWhitespace();
    scanner.nextToken();
    string command = toUpperCase(scanner.nextToken());
    if (command == "PRINT") return true;
    if (command == "LET") return true;
    if (command == "REM") return true;
    if (command == "INPUT") return true;
    if (command == "GOTO") return true;
    if (command == "IF") return true;
    if (command == "END") return true;
    return false;
}
