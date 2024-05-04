#include <iostream>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

map<char, int> weight { 
    {'+', 2 },
    {'-', 2 },
    {'*', 3 },
    {'/', 3 },
    {'(', 0 },
    {')', 1 }
};

map<char, int> weightReversed { 
    {'+', 4 },
    {'-', 4 },
    {'*', 6 },
    {'/', 7 },
    {'(', 1 },
    {')', 0 }
};

struct Stack {
  union {
    int digit;
    char letter;
  } data;
  Stack* tail;
};

unsigned length(Stack* beg) {
  unsigned length = 0;
  while (beg) {
    ++length;
    beg = beg->tail;
  }
  return length;
}

bool isEmpty(Stack* beg) {
  if (beg ==  NULL || length(beg) == 0) return true;
  return false;
}

Stack* findItemByIndex(Stack* beg, unsigned idx) {
  int counter = 0;
      
  while (beg) {
    if (counter == idx) return beg;
    beg = beg->tail;
    counter++;
  }
  return NULL;
}

Stack* peek(Stack* beg) { 
  if (beg == NULL) return NULL;
  while (beg->tail) beg = beg->tail;
  return beg;
}

void pop(Stack* &beg) {
  Stack* item = NULL;
  int index = length(beg) - 1;

  if (index < 0) return;
  if ( !index ) {
    item = beg -> tail;
    delete beg;
    beg = item;
    return;
  }

  item = findItemByIndex(beg, index - 1);
  Stack* dItem = item->tail;
  item->tail = dItem->tail;
  delete dItem;
}

Stack* addNode(Stack* &beg) {
  Stack* item = new Stack;
  item->tail = NULL;

  if (beg == NULL) {
    beg = item;
    return item;
  }

  int index = length(beg);
  Stack* prevItem = beg;
  --index;

  while (prevItem->tail && (index--)) prevItem = prevItem->tail; 

  item->tail = prevItem->tail;
  prevItem->tail = item;

  return item;
}

Stack* addDigit(Stack* &beg, int data) {
  Stack* item = addNode(beg);
  if (item) item->data.digit = data;
  return item;
}

Stack* addLetter(Stack* &beg, int data) {
  Stack* item = addNode(beg);
  if (item) item->data.letter = data;
  return item;
}

void printNode(Stack* curr, bool isDigit) {
  cout << "\nItem\n"
  << " Data: " << (isDigit ? curr->data.digit : curr->data.letter) << "\n"
  << " Tail: " << curr->tail << "\n"
  << " Addr: " << curr << "\n";
}

void printStack(Stack* beg, bool isDigit) {
  if (!beg) {
    cout << "\nThe stack is empty(((\n";
    return;
  }
  while (beg) {
    printNode(beg, isDigit);
    beg = beg->tail;
  }
}


int calculateInfixOperation(char op, int first, int second) {
  if (!second) return first;
  if (!first) return second;
  switch (op) {
    case '+':
      return first + second;
    case '-':
      return first - second;
    case '*':
      return first * second;
    case '/':
      return first / second;
    default:
      return 0;
  };
}

string getNumberFromString(string& expr, int& pos) {
  string output = "";
    
  for (pos; pos < expr.length(); pos++) {
    char c = expr[pos];
    
    if (isdigit(c)) output += c;
    else {
      pos--;
      break;
    }
  }
  return output;
}

void reverseString(string& inp) {
  string output = "";
  for (int i = inp.length() - 1; i >= 0; i--) output += inp[i];
  inp = output;
}

string getNumberFromReverseString(string& expr, int& pos) {
  string output = "";
    
  for (pos; pos >= 0; pos--) {
    char c = expr[pos];
    
    if (isdigit(c)) output += c;
    else {
      pos++;
      break;
    }
  }
  reverseString(output);
  return output;
}

void reverseNumbersInString(string& inp) {
  string output = "";
  string number = "";
  for (int i = 0; i < inp.length(); i++) {
    if (isdigit(inp[i])) {
      number = getNumberFromString(inp, i);
      reverseString(number);
      output += number;
    } else {
      output += inp[i];
    }
  }
  inp = output;
}

string fromInfixToPostfix(string& infixExpr) {
  if (infixExpr.length() == 0) return "";
  string output;
  Stack* opStack = NULL; // стек операций

  for (int i = 0; i < infixExpr.length(); i++) {
    char c = infixExpr[i];

    if (isdigit(c)) {
      string number = getNumberFromString(infixExpr, i);
      output += number;
      output += ' ';
      cout << "\nFound number " << number << ", add it to the output line\n";
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      cout << "\nFound an operation sign " << c << ", checking priority\n";
      if ( length(opStack) == 0 || (weight[peek(opStack)->data.letter] < weight[c]) ) {
        addLetter(opStack, c);
        cout << "\nCool cla$$, operation priority " << c << " > priority of top of the stack (or empty), add it to the operations stack\n";
      }
      else {
        cout << "\nThat's a do$ada, the priority of the operation " << c << " <= priority of top of the stack, pull out all operations to the lowest priority\n";
        while ( (peek(opStack) != NULL ? weight[peek(opStack)->data.letter] : -1) >= weight[c] ) {
          output += peek(opStack)->data.letter;
          output += ' ';
          cout << "\nAdded an operation sign from the stack " << peek(opStack)->data.letter << " to the output line\n";
          pop(opStack);
        }
        cout << "\nAnd consolidate success: priority " << c << " > priority of top of the stack (or empty), add it to the operations stack\n";
        if ( length(opStack) == 0 || (weight[peek(opStack)->data.letter] < weight[c]) ) {
          addLetter(opStack, c);
          cout << "\nAdded an operation sign " << c << " to the operations stack\n";
        }
      }
    } else if (c == '(') {
      addLetter(opStack, c);
      cout << "\nAdd open bracket to the operation stack\n";
    } else if (c == ')') {
      cout << "\nFound closed bracket, pull out all the operation signs from stack, until open bracket:\n";
      while ( peek(opStack) != NULL && peek(opStack)->data.letter != '(' ) {
        output += peek(opStack)->data.letter;
        output += ' ';
        cout << "\nAdded an operation sign from the stack " << peek(opStack)->data.letter << " to the output line\n";
        pop(opStack);
      }  
      cout << "\nRemove open bracket from the operation stack\n";
      pop(opStack);
    }
  }
  cout << "\nThe input line has ended, we throw all the remaining operation signs from the stack into the output line\n";
  while (length(opStack) > 0) {
    output += peek(opStack)->data.letter;
    output += ' ';
    cout << "\nAdded an operation sign from the stack " << peek(opStack)->data.letter << " to the output line\n";
    pop(opStack);
  }
  return output;
}

string fromPrefixToPostfix(string& prefixExpr) {
  Stack* stack = NULL;
  string postfix = "";
  string temp;
  string number;
  
  for (int i = prefixExpr.length() - 1; i >= 0; i--) {
    char c = prefixExpr[i];
    if (isdigit(c)) {
      number = getNumberFromReverseString(prefixExpr, i);
      addDigit(stack, stoi(number));
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      string op1 = peek(stack) ? to_string(peek(stack)->data.digit) : "";
      if (op1 == "0") op1 = temp;
      pop(stack);
      string op2 = peek(stack) ? to_string(peek(stack)->data.digit) : "";
      if (op2 == "0") op2 = temp;
      pop(stack);
      temp = (op1 + " " + op2 + " " + c);

      addDigit(stack, 0);
    }
  }
  return temp;
}

string fromInfixToPrefix(string& infixExpr) {
  if (infixExpr.length() == 0) return "";
  string output;
  Stack* opStack = NULL; // стек операций

  for (int i = infixExpr.length(); i >= 0; i--) {
    char c = infixExpr[i];

    if (isdigit(c)) {
      string number = getNumberFromReverseString(infixExpr, i);
      output += number;
      output += ' ';
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      if ( length(opStack) == 0 || (weightReversed[peek(opStack)->data.letter] < weightReversed[c]) ) {
        addLetter(opStack, c);
      }
      else {
        while ( (peek(opStack) != NULL ? weightReversed[peek(opStack)->data.letter] : -1) >= weightReversed[c] ) {
          output += peek(opStack)->data.letter;
          output += ' ';
          pop(opStack);
        }
        if ( length(opStack) == 0 || (weightReversed[peek(opStack)->data.letter] < weightReversed[c]) ) {
          addLetter(opStack, c);
        }
      }
    } else if (c == ')') {
      addLetter(opStack, c);
    } else if (c == '(') {
      while ( peek(opStack) != NULL && peek(opStack)->data.letter != ')' ) {
        output += peek(opStack)->data.letter;
        output += ' ';
        pop(opStack);
      }  
      pop(opStack);
    }
  }
  while (length(opStack) > 0) {
    output += peek(opStack)->data.letter;
    output += ' ';
    pop(opStack);
  }
  reverseString(output);
  reverseNumbersInString(output);
  return output;
}

int calculatePostfix(string& postfixExpr) {
  if (postfixExpr.length() == 0) return 0;
  Stack* stack = NULL; // стек операндов
  string number;

  for (int i = 0; i < postfixExpr.length(); i++) {
    char c = postfixExpr[i];
    if (isdigit(c)) {
      number = getNumberFromString(postfixExpr, i);
      addDigit(stack, stoi(number));
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      int num2 = peek(stack) ? peek(stack)->data.digit : 0;
      pop(stack);
      int num1 = peek(stack) ? peek(stack)->data.digit : 0;
      pop(stack);

      addDigit(stack, calculateInfixOperation(c, num1, num2));
    }
  }
  return peek(stack)->data.digit;
}

int calculatePrefix(string& prefixExpr) {
  if (prefixExpr.length() == 0) return 0;
  string postfixExpr = fromPrefixToPostfix(prefixExpr);
  return calculatePostfix(postfixExpr);
}

int countOperators(const string& expr) {
  int counter = 0;
  counter += count(expr.begin(), expr.end(), '+');
  counter += count(expr.begin(), expr.end(), '-');
  counter += count(expr.begin(), expr.end(), '*');
  counter += count(expr.begin(), expr.end(), '/');
  return counter;
}

int countNumbers(string& expr) {
  int counter = 0;
  for (int i = 0; i < expr.length(); i++) {
    char c = expr[i];
    if (isdigit(c)) {
      string number = getNumberFromString(expr, i);
      counter++;
    }
  }
  return counter;
}

bool checkExpr(string& expr) {
  if (expr.empty()) return false;
  if (count(expr.begin(), expr.end(), '(') != count(expr.begin(), expr.end(), ')')) {
    return false;
  }
  if (countNumbers(expr) - countOperators(expr) != 1) {
    return false;
  }

  return true;
}

bool checkDictIncludes(char letter, map<char, int>& dict) {
  return bool(dict[letter]);
}

string cutString(string& inp, unsigned startIdx, unsigned endIdx) {
  string out;
  for (int i = 0; i < inp.length(); i++) if (i >= startIdx && i < endIdx) out += inp[i];
  return out;
}

string replaceVariablesInString(string& inp, map<char, int>& dict) {
  string out = inp;
  string inpBefore;
  string inpAfter;
  for (int i = 0; i < inp.length(); i++) {
    if (checkDictIncludes(inp[i], dict)) {
      inpBefore = cutString(out, 0, i);
      inpAfter = cutString(out, i + 1, inp.length());
      out = inpBefore + to_string(dict[inp[i]]) + inpAfter;
    }
  }
  return out;
}

void printMap(map<char, int> dict) {
  for (const auto& [key, value] : dict)
    cout << key << " " << value << "\n";
}

int main() {
  setlocale(LC_ALL, "Russian");

  string infixExpr;
  string postfixExpr;
  string prefixExpr;

  char actionType;
  short unsigned choiseType;
  short int workPoint;

  map<char, int> variables {};

  while(true) {
    cout << "\nNavigation\n"
         << "1) Convert from infix to postfix and prefix:\n"
         << "2) Check expression\n"
         << "3) Calculate expression\n";

    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    cout << "Select point of work (number 1 to 3): ";
    cin >> workPoint;

    switch (workPoint) {   
      case 1: {
        cout << "Write an infix expression:\n";
        cin.clear();
        cin.sync();
        getline(cin, infixExpr);
        cout << "Are you used variables in expression? (Y/N): \n";
        cin >> actionType;
        switch (actionType) {
          case 'Y': {
            string varLine;
            cout << "Write all variables (new var. = new line, enter <S> to stop it) (example: a=24):\n";
            cin.clear();
            cin.sync();
            while (true) {
              getline(cin, varLine);
              if (varLine.length() < 3) break;
              int num = stoi(cutString(varLine, 2, varLine.length()));
              variables[varLine[0]] = num;
            }
            infixExpr = replaceVariablesInString(infixExpr, variables);
            break;
          }
        }
        if (!checkExpr(infixExpr)) {
          cout << "\nInvalid Input";
          break;
        }
        postfixExpr = fromInfixToPostfix(infixExpr);
        cout << "\nPostfix: " << postfixExpr << "\n";
        
        prefixExpr = fromInfixToPrefix(infixExpr);
        cout << "\nPrefix: " << prefixExpr << "\n";

        break;
      }
      case 2: {
        cout << "\nChoose the expression type:"
             << "\n 1 - Infix"
             << "\n 2 - Prefix"
             << "\n 3 - Postfix\n";
        cin >> choiseType;

        cout << "Write an expression:\n";
        cin.clear();
        cin.sync();

        switch (choiseType) {
          case 1: {
            getline(cin, infixExpr);
            if (!checkExpr(infixExpr)) {
              cout << "\nInvalid Input";
              infixExpr = "";
              break;
            }
            cout << "Cool";
            break;
          }
          case 2: {
            getline(cin, prefixExpr);
            if (!checkExpr(prefixExpr)) {
              cout << "\nInvalid Input";
              prefixExpr = "";
              break;
            }
            cout << "Cool";
            break;
          }
          case 3: {
            getline(cin, postfixExpr);
            if (!checkExpr(postfixExpr)) {
              cout << "\nInvalid Input";
              postfixExpr = "";
              break;
            }
            cout << "Cool";
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }

        break;
      }
      case 3: {
        cout << "\nChoose the expression type:"
             << "\n 1 - Infix"
             << "\n 2 - Prefix"
             << "\n 3 - Postfix\n";
        cin >> choiseType;

        cout << "Do you want to use old expression from memory< or write new? (O/N)\n";
        cin >> actionType;
        if (actionType == 'O') {
          switch (choiseType) {
            case 1: {
              postfixExpr = fromInfixToPostfix(infixExpr);
              cout << "\nResult: " << calculatePostfix(postfixExpr);
              break;
            }
            case 2: {
              cout << "\nResult: " << calculatePrefix(prefixExpr);
              break;
            }
            case 3: {
              cout << "\nResult: " << calculatePostfix(postfixExpr);
              break;
            }
            default: {
              cout << "\nYou entered an incorrect value\n";
              break;
            }
          }
          break;
        }
        cout << "Write an expression:\n";
        cin.clear();
        cin.sync();

        switch (choiseType) {
          case 1: {
            getline(cin, infixExpr);
            if (!checkExpr(infixExpr)) {
              cout << "\nInvalid Input";
              break;
            }
            postfixExpr = fromInfixToPostfix(infixExpr);
            cout << "\nResult: " << calculatePostfix(postfixExpr);
            break;
          }
          case 2: {
            getline(cin, prefixExpr);
            if (!checkExpr(prefixExpr)) {
              cout << "\nInvalid Input";
              break;
            }
            cout << "\nResult: " << calculatePrefix(prefixExpr);
            break;
          }
          case 3: {
            getline(cin, postfixExpr);
            if (!checkExpr(postfixExpr)) {
              cout << "\nInvalid Input";
              break;
            }
            cout << "\nResult: " << calculatePostfix(postfixExpr);
            break;
          }
          default: {
            cout << "\nYou entered an incorrect value\n";
            break;
          }
        }

        break;
      }
      default: {
        cout << "\n" << "You did not enter a number in the range from 1 to 3";
        break;
      }
    }
    cin.clear(); // Clearing the input stream from possible errors
    cin.sync();

    char stopFlag;
    cout << "\n" << "Continue the program? (Y/N) ";
    cin >> stopFlag;
        
    if (stopFlag != 'Y' && stopFlag != 'y') break;
  }

  return 0;
}