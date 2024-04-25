#include <iostream>
using namespace std;


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


int main() {
  Stack* stack = NULL;
  cin.clear(); // Clearing the input stream from possible errors
  cin.sync();

  int value;
  while (cin >> value) addDigit(stack, value);


  while (!isEmpty(stack)) {
    printStack(stack, true);
    pop(stack);
  }

  return 0;
}