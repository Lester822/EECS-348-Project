/*
17 Calculator Co.
Version 1.0
Last Updated: 03 December 2023
*/

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


// Declarations of function usage can be found in each function's docstring
string collectInput();
bool isValid(string userInput);
vector<char> operatorVectorify(string userInput);
vector<double> doubleVectorify(string userInput);
double solver(string userInput);
double loopSolver(vector<char>& keyVector, vector<double>& doubleVector);
int nextStep(vector<char>& keyVec, vector<double>& doubVec, int startIndex, int endIndex);
void solveStep(vector<double>& numbers, vector<char>& key, int start_index);
void matcher(vector<char>& operatorVector, vector<double>& doubleVector);

void partial_print(vector<char>& key, int start, int end) {
    /* Debug function used to print a vector from start to end index */
    cout << "\nPRINTING PART: ";
    for (int i = start; i <= end; i++) {
        cout << key[i] << " ";
    }
    cout << "\n";
}

void print_expression(vector<double>& numVector, vector<char>& charVector) {
    /* Debug function that prints an expression given the key and number vectors */
    cout << "\n";
    for (int i = 0; i < charVector.size(); i++) {
        if (charVector[i] == '0') {
            cout << numVector[i] << " ";
        } else {
            cout << charVector[i] << " ";
        }
    }
    cout << "\n";
}

void print_vector(vector<double>& myVector) {
    /* Debug function that prints a double vector */
    cout << "\nPRINTING: ";
    for (int i = 0; i < myVector.size(); i++) {
        cout << myVector[i] << " ";
    }
    cout << "\n";
}

void print_vector(vector<char>& myVector) {
    /* Debug function that prints a character vector */
    cout << "\nPRINTING: ";
    for (int i = 0; i < myVector.size(); i++) {
        cout << myVector[i] << " ";
    }
    cout << "\n";
}

// MAIN CODE STARTS HERE

int main(){
    /* Main function that runs the program */
    string userInput;  // Var that will hold the user's input
    while (true){
        try {
            userInput = collectInput();
            if (userInput == "exit"){
                break;
            }
            if (isValid(userInput) == true){
                //cout << "Valid" << endl;
                cout << solver(userInput) << "\n\n";
            }  
        } catch (domain_error) { // This is to handle division by zero
            cout << "Error - Division by zero" << endl << endl;
        } catch (invalid_argument) { // This is for roots of negatives
            cout << "Error - Root of a negative number" << endl << endl;
        } catch (out_of_range) { // This is to handle if no input is provided
            cout << "Error - Input must be provided" << endl << endl;
        } catch (logic_error) { // This is to handle if input is all spaces
            cout << "Error - Input cannot be all spaces" << endl << endl;
        }
        
    }
    return 0;
}

string collectInput(){
    /* Function that collects the mathematical expression from the user and cleans it. Returns a string. */
    string input;
    cout << "Enter an expression (or 'exit'): ";
    getline(cin, input);
    if (input.empty()) { // If the string is empty, raise an error
        throw out_of_range("Error - Input must be provided");
    }
    if (input.find_first_not_of(' ') > input.length()) { // If the first non-space character is after the end of the string, then we know the string is all spaces
        throw logic_error("Error - Input cannot be all spaces");
    }
    input.erase(remove(input.begin(), input.end(), ' '), input.end());

    return input;
}

bool isValid(string userInput){
    /* Checks for the validity of statements provided by the user */
    string validCharString = "0123456789+-*/()^% ";
    for (int i = 0; i < userInput.size(); i++){
        if (validCharString.find(userInput[i]) == string::npos){
            cout << "Error - Invalid character(s) " << userInput[i] << endl;
            return false;
        }
    }

    vector<char> inputVector;

    for (int i = 0; i < userInput.size(); i++){
        if (userInput[i] == '1' || userInput[i] == '2' || userInput[i] == '3' || userInput[i] == '4' || userInput[i] == '5' || userInput[i] == '6' || userInput[i] == '7' || userInput[i] == '8' || userInput[i] == '9'){
            inputVector.push_back('0');  // Pushes 0s in the char vecetor wherever a number is
        }else if( userInput[i] != ' '){  // If it's not a number, and not a space
            inputVector.push_back(userInput[i]);
        }
    }

    for (int i = inputVector.size() - 1; i > 0; i--){
        if (inputVector[i] == inputVector[i-1] && inputVector[i] == '0'){  // Checks for multile numbers next to each other
            inputVector.erase(inputVector.begin() + i);
            i--;
        }
    }

    for (int i = inputVector.size() -1; i > 0; i--){
        if (inputVector[i] == ' '){  // Checks for spaces
            inputVector.erase(inputVector.begin() + i);
            i--;
        }
    }

    int openCount = 0;
    int closeCount = 0;

    for (int i = 0; i < inputVector.size(); i++){ // Checks for matching number of parantheses
        if (inputVector[i] == ')'){
            closeCount++;
        }
        if (inputVector[i] == '('){
            openCount++;
        }
    }
    if (openCount != closeCount){
        cout << "Error - Unmatched Paranetheses"<<endl;
        return false;
        
    }

    for(int i = 0; i < inputVector.size(); i++) {  // Handles checking for paranthesis with no internal statement
        if (inputVector[i] == '(' && inputVector[i+1] == ')') {
            cout << "Error - Invalid Paranetheses. Must have statement inside." << endl;
            return false;
        }
    }

    for(int i = 0; i < inputVector.size(); i++){
        if (inputVector[i] == '*' || inputVector[i] == '/' || inputVector[i] == '%' || inputVector[i] == '^'){  // Operator usage checking
            if (i == 0){
                cout << "Error - Invalid usage of operator *, /, %, or ^" << endl;
                return false;
            }
            if (i == inputVector.size() - 1){
                cout << "Error - Invalid usage of operator *, /, %, or ^" << endl;
                return false;
            }
            if (inputVector[i-1] != '0' && inputVector[i-1] != ')'){
                cout << "Error - Invalid usage of operator *, /, %, or ^" << endl;
                return false;
            }
            if (inputVector[i+1] != '0' && inputVector[i+1] != '(' && inputVector[i+1] != '+' && inputVector[i+1] != '-'){
                cout << "Error - Invalid usage of operator *, /, %, or ^" << endl;
                return false;
            }
        }
    }
    for (int i = 0; i < inputVector.size(); i++){
        if (inputVector[i] == '+' || inputVector[i] == '-'){
            if (i == inputVector.size() - 1){
                cout << "Error - Invalid usage of operator + or -" << endl;
                return false;
            }
            if (inputVector[i+1] != '0' && inputVector[i+1] != '(' && inputVector[i+1] != '-' && inputVector[i+1] != '+'){
                cout << "Error - Invalid usage of operator + or -" << endl;
                return false;
            }
            // if (i != 0){
            //     if (inputVector[i-1] != '0' && inputVector[i-1] != ')' && inputVector[i-1] != '(' && inputVector[i-1] != '-' && inputVector[i-1] != '+' && inputVector[i+1] != '*' && inputVector[i+1] != '/'){
            //         cout << "Invalid usage of operator + or -" << endl;
            //         return false;
            //     }
            // }
        }
    }
    for (int i = 0; i < inputVector.size(); i++){  // Checks for valid parentheses usage
        if (inputVector[i] == '(' && i != 0){
            if (inputVector[i-1] == '0'){
                cout << "Error - Missing operator before (" << endl;
                return false;
            }
        }
        if (inputVector[i] == ')' && i != inputVector.size() - 1){
            if (inputVector[i+1] == '0' || inputVector[i+1] == '('){
                cout << "Error - Missing operator after )" << endl;
                return false;
            }
        }
    } 
    //cout<< "Valid Input!" << endl;
    return true;
}

vector<char> operatorVectorify(string userInput){
    /* Function takes in a string (user's input) and returns a character vector with the operations */
    string digits = "0123456789 ";
    vector<char> operatorVector;
    for (int i = 0; i < userInput.size(); i++){
        if (digits.find(userInput[i]) != string::npos) {
            operatorVector.push_back('0');
        } else {
            operatorVector.push_back(userInput[i]);
        }
    }

    for (int i = operatorVector.size() - 1; i > 0; i--){
        if (operatorVector[i] == operatorVector[i-1] && operatorVector[i] == '0'){
            operatorVector.erase(operatorVector.begin() + i);
            //i--;
        }
    }
    return operatorVector;
}

vector<double> doubleVectorify(string userInput) {
    /* Function takes in a string (user's input) and returns a double vector with values */
    vector<double> doubleVector;
    string digits = "0123456789";

    double tempNum = 0;
    bool isNumber = false;

    for (int i = 0; i < userInput.size(); i++) {
        if (digits.find(userInput[i]) != string::npos) {
            tempNum = tempNum * 10 + (userInput[i] - '0');
            isNumber = true;
        } else if (isNumber) {
            doubleVector.push_back(tempNum);
            tempNum = 0;
            isNumber = false;
        }
    }

    if (isNumber) {  // To handle number at the end of the string
        doubleVector.push_back(tempNum);
    }

    return doubleVector;
}

void matcher(vector<char>& operatorVector, vector<double>& doubleVector){
    /* Ensure that the operator and double vectors are matched in terms of indicies */
    vector<double> tempDoubles;
    int doubleIndex = 0;
    for (int i = 0; i < operatorVector.size(); i++){
        if (operatorVector[i] == '0'){
            tempDoubles.push_back(doubleVector[doubleIndex]);
            doubleIndex++;
        }else{
            tempDoubles.push_back(0);
        }
    }
    doubleVector = tempDoubles;
}

double solver(string userInput){
    /* Handles the starting of the solving of the expression */
    vector<char> operatorVector = operatorVectorify(userInput);
    vector<double> doubleVector = doubleVectorify(userInput);
    //print_vector(operatorVector);
    //print_vector(doubleVector);
    matcher(operatorVector, doubleVector);
    //print_vector(operatorVector);
    //print_vector(doubleVector);
    return loopSolver(operatorVector, doubleVector);
}

double loopSolver(vector<char>& keyVector, vector<double>& doubleVector){
    /*
    operatorVector contains all operators in input order. All interger chars are replaces with zero. This does not include negative signs. 
    all negative signs should be treated as unary operators.

    doubleVector contains all numbers, as positive integers in input order.

    This function should start by calling on the lowest priority operators with slices of the operatorVector and doubleVector until base cases are met.

    Then the base cases should return up the stack, solving the higher priority operators until the final answer is returned. 


    addition / subtraction (right to left)
    multiplication / division / mod (right to left)
    exponents
    parentheses
    */

    /* Uses a while loop to continually simplify the expresion step by step until a final solution is reached */
    while (doubleVector.size() >= 1) { // Checks if the size of the integer vector is greater than or equal to 1
        if (doubleVector.size() == 1) { // If the size is exactly 1, then a solution has been reached. The printOutput function prints the solution to the terminal
            return doubleVector[0];
        }

        // nextStep returns start index
        // solveStep evaluates at index and updates keyVec and doubVec
        solveStep(doubleVector, keyVector, nextStep(keyVector, doubleVector, 0, keyVector.size()-1));
    }

    return 10;
}

void solveStep(vector<double>& numbers, vector<char>& key, int start_index) {
    /* Take in an expression vector, a key vector, and what the next step to perform is and update the vectors to simplify the expression. */
    //cout << start_index << " ";
    //print_expression(numbers, key);
    //print_vector(numbers);
    //print_vector(key);
    double answer;  // Variable that holds the answer to the step
    if (key[start_index] == '-') {
        numbers.erase(numbers.begin() + start_index);  // gets rid of the operator out of the vector
        key.erase(key.begin() + start_index);  // gets rid of the operator out of the key
        numbers[start_index] = numbers[start_index] * -1;  // inverts the number

    } else if (key[start_index] == '+') {
        numbers.erase(numbers.begin() + start_index);
        key.erase(key.begin() + start_index);
        numbers[start_index] = numbers[start_index];

    } else if (key[start_index] == '('){  // Checks if the step is a paranthesis reduction step i.e. "(10)" -> "10"
        numbers[start_index] = numbers[start_index+1];
        key[start_index] = '0';
        numbers.erase(numbers.begin() + start_index+2);  // Edits the expression vector
        numbers.erase(numbers.begin() + start_index+1);
        key.erase(key.begin() + start_index+2);  // Edits the key vector
        key.erase(key.begin() + start_index+1);
    } else {  // If it's a normal math expression... 
        if (key[start_index + 1] == '+') {  // Checks what operation
            answer = numbers[start_index] + numbers[start_index + 2];
        } else if (key[start_index + 1] == '-'){
            answer = numbers[start_index] - numbers[start_index + 2];
        } else if (key[start_index + 1] == '*'){
            answer = numbers[start_index] * numbers[start_index + 2];
        } else if (key[start_index + 1] == '/'){
            if (numbers[start_index + 2] == 0) {
                throw domain_error("Error - Division by zero");
            }
            else {
                answer = numbers[start_index] / numbers[start_index + 2];
            }
        } else if (key[start_index + 1] == '%'){
            answer = double((int)numbers[start_index] % (int)numbers[start_index + 2]);
        }  else if (key[start_index + 1] == '^'){
            if ((numbers[start_index + 2] < 1 && numbers[start_index + 2] > -1 && numbers[start_index + 2] != 0) && (numbers[start_index] < 0)) {
                throw invalid_argument("Error - Root of a negative number"); // Throws an error for taking the square root of a negative number
            }
            answer = pow(numbers[start_index], numbers[start_index + 2]);
        }
        
        numbers[start_index] = answer; // Sets the first number in expression to the answer (so that it works when there is only expression left)
        key[start_index] = '0';  // Updates key
        numbers.erase(numbers.begin() + start_index + 2);  // Gets rid of used elements
        numbers.erase(numbers.begin() + start_index + 1);
        key.erase(key.begin() + start_index + 2);
        key.erase(key.begin() + start_index + 1);
    }
    
}

// Input key vector and double vector
// Outputs the beginning index  of the part of the expression that needs to be simplified next
int nextStep(vector<char>& keyVec, vector<double>& doubVec, int startIndex, int endIndex) {
    /* Find the index of the beginning of the next step */

    // Case 1 --> (number)
    for (int i = startIndex; i <= endIndex-1; i++) {
        if (keyVec[i] == '(') {
            if (keyVec[i+2] == ')') {
                // pass keyVec[i] - the start parenthesis
                return i;
            }
        }
    }

    // Case 2 --> (expression)
    for (int i = startIndex; i <= endIndex-1; i++) {
        if (keyVec[i] == '(') {
            int lastParan;
            int open = 0;
            int closed = 0;
            for (int j = i; j <= endIndex; j++) {
                if (keyVec[j] == '(') {
                    open += 1;
                } else if (keyVec[j] == ')') {
                    closed += 1;
                    if (open == closed) {
                        lastParan = j;
                        break;
                    }
                }

            }
                //cout << "\n" << i+1 << " -> " << lastParan << "\n";
                return nextStep(keyVec, doubVec, i+1, lastParan-1);
            }
    }

    // Case 4 - PEMDAS
    for (int i = startIndex; i <= endIndex-1; i++) { // Iterates through the key vector
        if (keyVec[i] == '0' && keyVec[i+1] != '0' && keyVec[i+1] != '(' && keyVec[i+1] != ')' && keyVec[i+2] == '0') { // Checks for an instance of num-operator-num
            if (keyVec[i+1] == '^') { // If there is an instance of an exponential, it determines it to be the next step and returns the index of the first number in the grouping
                //cout << "EXPONENT!\n";
                return i;
            }
        }
    }

     // Case 3 --> -number or +number (unary operator)
    for (int i = startIndex; i <= endIndex-1; i++) {
        if (keyVec[i] == '-') { // Check that it is NOT subtraction
            if ((keyVec[i-1] == '0' || keyVec[i-1] == ')') && (keyVec[i+1] == '0' || keyVec[i+1] == '(')) {
                break;
            }
            else {
                if (keyVec[i+1] == '0') {
                    //cout << "UNARY-\n";
                    return i;
                }
            }
        }
        if (keyVec[i] == '+') { // Check that it is NOT addition
            if (keyVec[i-1] == '0' && keyVec[i+1] == '0') {
                break;
            }
            else {
                if (keyVec[i+1] == '0') {
                    //cout << "UNARY+\n";
                    return i;
                }
            }
        }
    }

    for (int i = startIndex; i <= endIndex-1; i++) {
        if (keyVec[i] == '0' && keyVec[i+1] != '0' && keyVec[i+1] != '(' && keyVec[i+1] != ')' && keyVec[i+2] == '0') { // Checks again for an instance of num-operator-num in the key vector
            if (keyVec[i+1] == '*' or keyVec[i+1] == '/' or keyVec[i+1] == '%') { // If there is an instance of multiplication, division, or modulo, returns the index of the first number in the grouping
                if (keyVec[i+1] == '*') {
                    //cout << "* TIME\n";
                    return i;
                }
                else if (keyVec[i+1] == '/') {
                    //cout << "/ TIME\n";
                    return i;
                }
                else if (keyVec[i+1] == '%') {
                    //cout << "% TIME\n";
                    return i;
                }
            }
        }
    }

    for (int i = startIndex; i <= endIndex-1; i++) {
        if (keyVec[i] == '0' && keyVec[i+1] != '0' && keyVec[i+1] != '(' && keyVec[i+1] != ')' && keyVec[i+2] == '0') { // Checks again for an instance of num-operator-num in the key vector
            if (keyVec[i+1] == '+' || keyVec[i+1] == ('-')) { // If there is an instance of addition or subtraction, returns the index of the first number in the grouping
                if (keyVec[i+1] == '+') {
                    //cout << "ADDITION\n";
                    return i;
                }
                else if (keyVec[i+1] == '-') {
                    //cout << "SUBTRACTION\n";
                    return i;
                }
            }
        }
    }
    return 0; // Should never be reached, but if none of the following situations are found, returns 0.
}
