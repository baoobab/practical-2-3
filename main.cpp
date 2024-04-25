#include <iostream>
#include <string>
#include <map>
using namespace std;

map<char, int> weight { 
    {'+', 2 },
    {'-', 2 },
    {'*', 3 },
    {'/', 3 },
    {'(', 0 },
    {')', 1 }
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

string getNumberFromString(string expr, int& pos) {
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

string fromInfixToPostfix(string& infixExpr) {
  string output;
  Stack* opStack = NULL; // стек операций

  for (int i = 0; i < infixExpr.length(); i++) {
    char c = infixExpr[i];

    if (isdigit(c)) {
      string number = getNumberFromString(infixExpr, i);
      output += number;
      output += ' ';
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      if ( length(opStack) == 0 || (weight[peek(opStack)->data.letter] < weight[c]) ) addLetter(opStack, c);
      else {
        while ( (peek(opStack) != NULL ? weight[peek(opStack)->data.letter] : -1) >= weight[c] ) {
          output += peek(opStack)->data.letter;
          output += ' ';
          pop(opStack);
        }
        if ( length(opStack) == 0 || (weight[peek(opStack)->data.letter] < weight[c]) ) addLetter(opStack, c);
      }
    } else if (c == '(') {
      addLetter(opStack, c);
    } else if (c == ')') {
      while ( peek(opStack) != NULL && peek(opStack)->data.letter != '(' ) {
        output += peek(opStack)->data.letter;
        output += ' ';
        pop(opStack);
      }  
      pop(opStack); // убираем саму скобку (
    }
  }
  while (length(opStack) > 0) { // закидываем оставшиеся символы из стека, после парсинга входной строки
    output += peek(opStack)->data.letter;
    output += ' ';
    pop(opStack);
  }
  return output;
}

int calculatePostfix(string postfixExpr) {
  Stack* stack = NULL;
  int number = 0;
  bool flag = true;

  for (int i = 0; i < postfixExpr.length(); i++) {
    char c = postfixExpr[i];
    if (isdigit(c)) {
      number *= 10;
      number += (c - '0');
      flag = true;
    } else {
      if (c != ' ') {
        int num2 = peek(stack)->data.digit;
        pop(stack);
        int num1 = peek(stack)->data.digit;
        pop(stack);

        addDigit(stack, calculateInfixOperation(c, num1, num2));
        flag = false;
      } else if (c == ' ' && flag) {
        addDigit(stack, number);
        number = 0;
      }
    }
  }
  return peek(stack)->data.digit;
}


int main() {
  string postfixExpr;
  string infixExpr;

  cout << "Write an infix expression: " << "\n";
  getline(cin, infixExpr);

  postfixExpr = fromInfixToPostfix(infixExpr);
  cout << "Postfix: " << postfixExpr << "\n";

  // getline(cin, postfixExpr);
  cout << "Result: " << calculatePostfix(postfixExpr);
  return 0;
}