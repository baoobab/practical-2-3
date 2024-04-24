#include <iostream>
using namespace std;

struct Stack
{
    int data;
    Stack* tail;


  unsigned getStackLength() {
    Stack* curr = this;
    unsigned length = 0;
    while (curr) {
      ++length;
      curr = curr->tail;
    }
    return length;
  }

  bool isEmpty() {
    if (this ==  NULL || getStackLength() == 0) return true;
    return false;
  }

  Stack* findItemByIndex(unsigned idx) {
    Stack* curr = this;
    int counter = 0;
    
    while (curr) {
      if (counter == idx) return curr;
      curr = curr->tail;
      counter++;
    }
    return NULL;
  }

  Stack* top() { 
    Stack* curr = this;
    while (curr->tail) curr = curr->tail;
    return curr;
  }
};


void del(Stack* &beg) {
  Stack* item = NULL;
  int index = beg->getStackLength() - 1;

  if (index < 0) return;
  if ( !index ) {
    item = beg -> tail;
    delete beg;
    beg = item;
    return;
  }

  item = beg->findItemByIndex(index - 1);
  Stack* dItem = item->tail;
  item->tail = dItem->tail;
  delete dItem;
}

Stack* add(Stack* &beg, int data) {
  Stack* item = new Stack;

  item->data = data;
  item->tail = NULL;

  if (beg == NULL) {
    beg = item;
    return item;
  }

  int index = beg->getStackLength();
  Stack* prevItem = beg;
  --index;

  while (prevItem->tail && (index--)) prevItem = prevItem->tail; 

  item->tail = prevItem->tail;
  prevItem->tail = item;

  return item;
}

void printNode(Stack* curr) {
  cout << "\nItem\n"
  << " Data: " << curr->data << "\n"
  << " Tail: " << curr->tail << "\n"
  << " Addr: " << curr << "\n";
}

void printStack(Stack* beg) {
  if (!beg) {
    cout << "\nThe stack is empty(((\n";
    return;
  }
  while (beg) {
    printNode(beg);
    beg = beg->tail;
  }
}



int main() {
  Stack* stack = NULL;
  cin.clear(); // Clearing the input stream from possible errors
  cin.sync();

  int value;
  while (cin >> value) add(stack, value);


  while (!stack->isEmpty()) {
    printStack(stack);
    del(stack);
  }

  return 0;
}