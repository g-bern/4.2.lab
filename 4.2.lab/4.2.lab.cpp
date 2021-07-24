#include <iostream>
#include <fstream>
#include <string.h>
#include <Windows.h>

using namespace std;

const UINT maxx = 256;

//тип данных
class Type {
private:
   char* value;
   char* linkName;
   UINT size, n;
   Type* next;
public:
   void setAmount(int a)
   {
      n = a;
   }

   Type* components[maxx] = { NULL };
   UINT sizeOfArrays[maxx] = { 0 };

   void setNext(Type* newNext)
   {
      next = newNext;
   }

   Type* getNext()
   {
      return next;
   }

   char* getName()
   {
      return value;
   }

   UINT getSize()
   {
      return size;
   }

   UINT getAmountOfComponents()
   {
      return n;
   }

   Type(Type* source)
   {
      next = NULL;
      strcpy_s(value, maxx, source->getName());
      n = source->getSize();
      for (int i = 0; i < n; i++) {
         components[i] = source->components[i];
      }
      size = source->getSize();
   }

   Type(char* newValue)
   {
      next = NULL;
      value = new char[maxx];
      strcpy_s(value, maxx, newValue);
      size = 0;
      n = 0;
   }

   Type(char* newValue, UINT newSize)
   {
      next = NULL;
      value = new char[maxx];
      strcpy_s(value, maxx, newValue);
      size = newSize;
      n = 0;
   }

   void newComponent(Type* newValue)
   {
      size += newValue->getSize();
      components[n] = newValue;
      sizeOfArrays[n] = 0;
      n++;
   }

   void newComponent(Type* newValue, UINT newSize)
   {
      size += newValue->getSize() * newSize;
      components[n] = newValue;
      sizeOfArrays[n] = newSize;
      n++;
   }

   void newComponent(Type* newValue, char* newLink)
   {
      size += newValue->getSize();
      components[n] = newValue;
      components[n]->linkName = new char[maxx];
      strcpy_s(components[n]->linkName, maxx, newLink);
      sizeOfArrays[n] = 0;
      n++;
   }

   void newComponent(Type* newValue, char* newLink, UINT newSize)
   {
      size += newValue->getSize() * newSize;
      components[n] = newValue;
      components[n]->linkName = new char[maxx];
      strcpy_s(components[n]->linkName, maxx, newLink);
      components[n]->setAmount(newSize);
      sizeOfArrays[n] = newSize;
      n++;
   }
};

//список доступных типов данных
class Types {
private:
   Type* first;
   Type* last;
   Type* p = NULL; //Дополнительный указатель для некоторых операций
   UINT amount;
public:
   Types()
   {
      //int
      char* a = new char[maxx];
      strcpy_s(a, maxx, "int");
      first = new Type(a, 4);
      last = first;
      //char
      strcpy_s(a, maxx, "char");
      last->setNext(new Type(a, 1));
      last = last->getNext();
      //bool
      strcpy_s(a, maxx, "bool");
      last->setNext(new Type(a, 1));
      last = last->getNext();
      //float
      strcpy_s(a, maxx, "float");
      last->setNext(new Type(a, 4));
      last = last->getNext();
      //ссылочный тип
      strcpy_s(a, maxx, "link");
      last->setNext(new Type(a, 8));
      last = last->getNext();
      amount = 5;
      //double
      strcpy_s(a, maxx, "double");
      last->setNext(new Type(a, 8));
      last = last->getNext();
      amount = 6;
      //short
      strcpy_s(a, maxx, "short");
      last->setNext(new Type(a, 2));
      last = last->getNext();
      amount = 7;
   }

   Type* getFirst()
   {
      return first;
   }

   UINT getAmount()
   {
      return amount;
   }

   void newType(char* newValue)
   {
      last->setNext(new Type(newValue));
      last = last->getNext();
      amount++;
   }

   void newType(char* newValue, UINT newSize)
   {
      last->setNext(new Type(newValue, newSize));
      last = last->getNext();
      amount++;
   }

   void newComponentForLastType(char* newValue, UINT size)
   {
      if (strstr(newValue, "*") != NULL) {
         char* a = new char[maxx];
         strcpy_s(a, maxx, "link");
         p = findType(a);
         last->newComponent(p, newValue, size);
         return;
      } else {
         p = findType(newValue);
      }

      if (p == NULL) {
         cout << "\nОшибка добавления компонента\n";
         return;
      }
      last->newComponent(p, size);
   }

   void newComponentForLastType(char* newValue)
   {
      if (strstr(newValue, "*") != NULL) {
         char* a = new char[maxx];
         strcpy_s(a, maxx, "link");
         p = findType(a);
         last->newComponent(p, newValue);
         return;
      } else {
         p = findType(newValue);
      }
      if (p == NULL) {
         cout << "\nОшибка добавления компонента\n";
         return;
      }
      last->newComponent(p);
   }

   Type* findType(char* value)
   {
      p = first;
      while (p != NULL) {
         if (!strcmp(p->getName(), value)) {
            break;
         }
         p = p->getNext();
      }
      return p;
   }
};

//объект (элемент динамической таблицы)
class Object {
private:
   char* name;
   char* value;
   UINT size;
   UINT amountOfComponents;
   UINT amount;
public:
   bool arr;
   Object* next;
   Type* components[maxx] = { NULL };
   UINT sizeOfArrays[maxx] = { 0 };

   Object(Object* prev)
   {
      strcpy_s(name, maxx, prev->getName());
      strcpy_s(value, maxx, prev->getValue());
      size = prev->getSize();
      amountOfComponents = prev->getAmountOfComponents();
      amount = prev->getAmount();
      arr = prev->arr;
      next = NULL;
      for (int i = 0; i < maxx; i++) {
         components[i] = prev->components[i];
         sizeOfArrays[i] = prev->sizeOfArrays[i];
      }
   }

   void incAmount()
   {
      amount++;
      return;
   }

   //конструктор для создания объекта массива
   Object(char* newName, Type* newType, UINT amountt)
   {
      arr = true;
      name = new char[maxx];
      value = new char[maxx];
      strcpy_s(name, maxx, newName);
      strcpy_s(value, maxx, newType->getName());
      size = amountt * newType->getSize();
      amountOfComponents = amountt;
      next = NULL;
      amount = 0;
   }

   //конструктор для создания объекта переменной
   Object(char* newName, Type* newType)
   {
      arr = false;
      name = new char[maxx];
      value = new char[maxx];
      strcpy_s(name, maxx, newName);
      strcpy_s(value, maxx, newType->getName());
      size = newType->getSize();
      amountOfComponents = newType->getAmountOfComponents();
      for (int i = 0; i < amountOfComponents; i++) {
         components[i] = newType->components[i];
         sizeOfArrays[i] = newType->sizeOfArrays[i];
      }
      next = NULL;
      amount = 0;
   }

   //конструктор для создания объекта ссылки
   Object(char* newName, char* linkName, Type* newType)
   {
      arr = false;
      next = NULL;
      name = new char[maxx];
      value = new char[maxx];
      strcpy_s(name, maxx, newName);
      strcpy_s(value, maxx, linkName);
      size = newType->getSize();
      amountOfComponents = 0;
      amount = 0;
   }

   //конструктор для создания объекта массива ссылок
   Object(char* newName, char* linkName, Type* newType, UINT amountt)
   {
      arr = true;
      name = new char[maxx];
      value = new char[maxx];
      strcpy_s(name, maxx, newName);
      strcpy_s(value, maxx, linkName);
      size = amountt * newType->getSize();
      amountOfComponents = amountt;
      next = NULL;
      amount = 0;
   }

   char* getName()
   {
      return name;
   }

   char* getValue()
   {
      return value;
   }

   UINT getSize()
   {
      return size;
   }

   UINT getAmountOfComponents()
   {
      return amountOfComponents;
   }

   UINT getAmount()
   {
      return amount;
   }
};

//динамическая таблица
class Objects {
private:
   Types* types = NULL; //набор типов, доступных для объектов
   UINT amount; //количество объектов
   Object *first = NULL, *last = NULL; //ссылка на первый объект
   Object *p = NULL, *p1 = NULL; //вариативная ссылка

   void sort(Object** arr, UINT size, int* arr_int)
   {
      if (size <= 1) {
         return;
      }
      bool flag = false;
      UINT k;
      for (int i = 1; i < size; i++) {
         flag = false;
         for (int j = i - 1; j >= 0; j--) {
            if (!flag) {
               k = i;
            }
            if (arr[k]->getAmount() > arr[j]->getAmount()) {
               swapp(arr_int[j], arr_int[k]);
               Object* q = arr[k];
               arr[k] = arr[j];
               arr[j] = q;
               flag = true;
               k = j;
            } else {
               break;
            }
         }
      }
   }

   void swapp(int a_id, UINT b_id)
   {
      Object* a = getById(a_id);
      Object* b = getById(b_id);
      Object* a_prev, * b_prev, * alpha, * a_next, * b_next;
      a_prev = getById(a_id - 1);
      b_prev = getById(b_id - 1);
      alpha = a;
      a_next = a->next;
      b_next = b->next;
      bool flag = false;
      if (a_prev != NULL) {
         if (b->next == NULL) {
            flag = true;
         }
         if ((b_id - a_id) == 1) {
            alpha->next = b_next;
            a_prev->next = b;
            a = b;
            a->next = alpha;
         } else {
            a_prev->next = b;
            a = b;
            a->next = a_next;
            alpha->next = b_next;
            b_prev->next = alpha;
         }
      } else {
         if (b->next == NULL) {
            flag = true;
         }
         if ((b_id - a_id) == 1) {
            alpha->next = b_next;
            a = b;
            a->next = alpha;
         } else {
            a = b;
            first = a;
            a->next = a_next;
            alpha->next = b_next;
            b_prev->next = alpha;
         }
      }
      if (flag) {
         last = alpha;
      }
   }

public:
   Objects(Types* value)
   {
      types = value;
      amount = 0;
      first = NULL;
      last = NULL;
      p = first;
   }

   Object* getFirst()
   {
      return first;
   }

   UINT getAmount(){
      return amount;
   }

   Object* getById(int value)
   {
      if (value > amount || value < 0) {
         return NULL;
      }

      p = first;
      for (int i = 0; i < value; i++) {
         p = p->next;
      }
      return p;
   }

   void newObject(char* newName, char* newType, UINT size)
   {
      char* newType1 = new char[maxx];
      bool link = false;
      if (newType[strlen(newType) - 1] == '*') {
         strcpy_s(newType1, maxx, "link");
         link = true;
      } else {
         newType1 = newType;
      }

      Type* delta = types->findType(newType1);
      if (first == NULL) {
         if (link) {
            first = new Object(newName, newType, delta, size);
         } else {
            first = new Object(newName, delta, size);
         }
         last = first;
         p = first;
         amount++;
      } else {
         if (strcmp(first->getName(), newName) > 0) {
            if (link) {
               p = new Object(newName, newType, delta, size);
            } else {
               p = new Object(newName, delta, size);
            }
            p->next = first;
            first = p;
            amount++;
            return;
         }

         if (strcmp(last->getName(), newName) < 0) {
            if (link) {
               last->next = new Object(newName, newType, delta, size);
            } else {
               last->next = new Object(newName, delta, size);
            }
            last = last->next;
            amount++;
            return;
         }

         p = first;
         p1 = NULL;
         while (strcmp(p->getName(), newName) < 0) {
            p1 = p;
            p = p->next;
         }
         if (p1 == NULL) return;
         if (link) {
            p = new Object(newName, newType, delta, size);
         } else {
            p = new Object(newName, delta, size);
         }
         p->next = p1->next;
         p1->next = p;
         amount++;
      }
   }
   
   void newObject(char* newName, char* newType)
   {
      char* newType1 = new char[maxx];
      bool link = false;

      if (newType[strlen(newType) - 1] == '*') {
         strcpy_s(newType1, maxx, "link");
         link = true;
      } else {
         newType1 = newType;
      }

      Type* delta = types->findType(newType1);
      if (first == NULL) {
         if (link) {
            first = new Object(newName, newType, delta);
         } else {
            first = new Object(newName, delta);
         }
         last = first;
         p = first;
         amount++;
      } else {
         if (strcmp(first->getName(), newName) > 0) {
            if (link) {
               p = new Object(newName, newType, delta);
            } else {
               p = new Object(newName, delta);
            }
            p->next = first;
            first = p;
            amount++;
            return;
         }

         if (strcmp(last->getName(), newName) < 0) {
            if (link) {
               last->next = new Object(newName, newType, delta);
            } else {
               last->next = new Object(newName, delta);
            }
            last = last->next;
            amount++;
            return;
         }

         p = first;
         p1 = NULL;
         while (strcmp(p->getName(), newName) < 0) {
            p1 = p;
            p = p->next;
         }
         if (p1 == NULL) return;
         if (link) {
            p = new Object(newName, newType, delta);
         } else {
            p = new Object(newName, delta);
         }
         p->next = p1->next;
         p1->next = p;
         amount++;
      }
   }

   void shell_sort()
   {
      if (amount <= 1) {
         return;
      }

      UINT delta = amount / 2;
      UINT i, j, k;
      while (delta) {
         Object** q = new Object*[amount / delta];
         int* q_int = new int[amount / delta];
         for (i = 0; i < (amount - delta); i++) {
            k = 0;
            j = i + delta;
            q[k] = getById(i);
            q_int[k] = i;
            k++;
            while (j < amount) {
               q[k] = getById(j);
               q_int[k] = j;
               k++;
               j += delta;
            }
            sort(q, k, q_int);
         }
         delta--;
      }
   }

   void printAll()
   {
      p = first;
      for (int i = 0; i < amount; i++) {
         if (p == NULL) return;
         if (p->getAmountOfComponents() == 0) {
            cout << endl << "Name: " << p->getName() << " Type: " << p->getValue() << " Size: " << p->getSize();
            cout << " Freq: " << p->getAmount() << endl;
         } else {
            UINT sz = p->getSize();
            cout << endl << "Name: " << p->getName() << " Type: " << p->getValue() << " Size: " << sz;
            cout << " Freq: " << p->getAmount();
            UINT alpha = p->getAmountOfComponents();
            cout << endl << "Amount of components: " << alpha << endl;
            if (!(p->arr)) {
               for (int i = 0; i < alpha; i++) {
                  cout << p->components[i]->getName();
                  if (p->sizeOfArrays[i]) {
                     cout << "[" << p->sizeOfArrays[i] << "] ";
                  } else {
                     cout << " ";
                  }
               }
               cout << endl;
            }
         }
         p = p->next;
      }
      return;
   }
};

//элемент статической таблицы
struct PartOfTable {
   char* name;
   char* value;
   UINT size;
   UINT amount;
   UINT amountOfComponents;
   bool arr;
   Type* components[maxx];
   UINT sizeOfArrays[maxx];
};

//статическая таблица
class StaticTable {
private:
   UINT size;
   UINT sortSize = 0;
   PartOfTable* Table;

public:
   StaticTable(Objects* Value)
   {
      size = Value->getAmount();
      Table = new PartOfTable[size];
      Object* p = Value->getFirst();
      for (int i = 0; i < size; i++) {
         Table[i].amount = p->getAmount();
         Table[i].amountOfComponents = p->getAmountOfComponents();
         Table[i].arr = p->arr;
         Table[i].size = p->getSize();
         Table[i].name = p->getName();
         Table[i].value = p->getValue();
         for (int j = 0; j < maxx; j++) {
            Table[i].components[j] = p->components[j];
            Table[i].sizeOfArrays[j] = p->sizeOfArrays[j];
         }
         p = p->next;
      }
   }

   void shell_sort()
   {
      PartOfTable delta;
      UINT i, j, step;
      for (step = size / 2; step > 0; step = step / 2) {
         for (i = step; i < size; i++) {
            delta = this->Table[i];

            for (j = i; j >= step; j -= step) {
               if (delta.amount > this->Table[j - step].amount) {
                  this->Table[j] = this->Table[j - step];
               } else {
                  break;
               }
            }
            this->Table[j] = delta;
         }
      }
   }

   void heapify(int i)
   {
      UINT min = i;
      UINT l = 2 * i + 1;
      UINT r = 2 * i + 2;

      if (l < sortSize && Table[l].amount < Table[min].amount)
         min = l;

      if (r < sortSize && Table[r].amount < Table[min].amount)
         min = r;

      if (min != i) {
         swap(Table[i], Table[min]);
         heapify(min);
      }
   }

   void heapSort()
   {
      sortSize = size;
   
      for (int i = size / 2 - 1; i >= 0; i--)
         heapify(i);

      for (int i = size - 1; i >= 0; i--) {
         swap(Table[0], Table[i]);
         sortSize--;

         heapify(0);
      }
   }

   void printAll()
   {
      cout.width(10);
      cout.setf(ios::right);
      for (int i = 0; i < size; i++) {
         if (this->Table[i].amountOfComponents == 0) {
            cout << endl << " | Name: " << this->Table[i].name << " | Type: " << this->Table[i].value << " | Size: " << this->Table[i].size;
            cout << " | Freq: " << this->Table[i].amount << " |\n";
         } else {
            UINT sz = this->Table[i].size;
            cout << endl << " | Name: " << this->Table[i].name << " | Type: " << this->Table[i].value << " | Size: " << sz;
            cout << " | Freq: " << this->Table[i].amount << " |";
            UINT alpha = this->Table[i].amountOfComponents;
            cout << endl << " | Amount of components: " << alpha << " |\n";
            if (!(this->Table[i].arr)) {
               for (int j = 0; j < alpha; j++) {
                  cout << " | " << this->Table[i].components[j]->getName();
                  if (this->Table[i].sizeOfArrays[j]) {
                     cout << "[" << this->Table[i].sizeOfArrays[j] << "] ";
                  } else {
                     cout << " ";
                  }
               }
               cout << " | ";
               cout << endl;
            }
         }
      }
      return;
   }
};

int main()
{
   setlocale(LC_ALL, "Russian");
   ifstream in("input.txt");
   char* string, * string1, * string2;
   Types* A = new Types();
   Objects* B = new Objects(A);
   Type* p;
   string = new char[maxx];
   char del[] = ";";
   char del2[] = " !;%:&*()-+=[]{}./|<>,\t\0\n";
   char *part, *cont = NULL;

   while (in) {
      part = new char[maxx];
      part[0] = ' ';
      string = new char[maxx];
      in.getline(string, maxx);

      string1 = strstr(string, "struct");
      if (string1 != NULL) {
         UINT i = 0;
         while (string1[i] != ' ') {
            i++;
         }
         UINT j = 0;
         i++;
         string2 = new char[maxx];
         while (string1[i] != '{' && string1[i] != ' ') {
            string2[j] = string1[i];
            i++;
            j++;
         }
         string2[j] = '\0';
         A->newType(string2);
         while (strstr(part, "}") == NULL) {
            in.getline(part, maxx);
            if (strstr(part, "}") != NULL) break;
            string = strtok_s(part, del, &cont);
            while (string != NULL) {
               p = A->getFirst();
               while (p != NULL) {
                  string1 = strstr(string, p->getName());
                  if (string1 != NULL) {
                     break;
                  }
                  p = p->getNext();
               }
               if (p == NULL) {
                  string = strtok_s(NULL, del, &cont);
                  continue;
               }

               UINT i = 0;
               while (string1[i] != ' ') {
                  i++;
               }
               string1[i] = '\0';
               string2 = new char[maxx];
               i++;
               UINT j = 0;
               while (string1[i] != ' ' && string1[i] != '[' && string1[i] != ';' && string1[i] != '\0') {
                  string2[j] = string1[i];
                  i++;
                  j++;
               }
               string2[j] = '\0';
               if (string1[i] == ';' || string1[i] == ' ' || string1[i] == '\0') {
                  A->newComponentForLastType(string1);

               } else if (string1[i] == '[') {
                  char a = ' ';
                  UINT size = 0;
                  i++;
                  while (string1[i] != ']') {
                     a = string1[i];
                     size = size * 10 + (a - '0');
                     i++;
                     j++;
                  }
                  A->newComponentForLastType(string1, size);
               }
               string = strtok_s(NULL, del, &cont);
            }
         }
         continue;
      }


      string1 = strstr(string, "int main()");
      if (string1 != NULL) {
         char* part1 = new char[maxx];
         while (strstr(part, "}") == NULL) {
            in.getline(part, maxx);
            strcpy_s(part1, maxx, part);
            if (strstr(part, "}") != NULL) break;
            string = strtok_s(part, del, &cont);
            while (string != NULL) {
               p = A->getFirst();
               while (p != NULL) {
                  string1 = strstr(string, p->getName());
                  if (string1 != NULL) {
                     break;
                  }
                  p = p->getNext();
               }
               if (p == NULL) {
                  string = strtok_s(NULL, del, &cont);
                  continue;
               }

               UINT i = 0;
               while (string1[i] != ' ') {
                  i++;
               }
               string1[i] = '\0';
               string2 = new char[maxx];
               i++;
               UINT j = 0;
               while (string1[i] != ' ' && string1[i] != '[' && string1[i] != ',' && string1[i] != '\0') {
                  string2[j] = string1[i];
                  i++;
                  j++;
               }
               string2[j] = '\0';

               if (string1[i] == ';' || string1[i] == ' ' || string1[i] == '\0') {
                  B->newObject(string2, string1);

               } else if (string1[i] == '[') {
                  char a = ' ';
                  UINT size = 0;
                  i++;
                  while (string1[i] != ']') {
                     a = string1[i];
                     size = size * 10 + (a - '0');
                     i++;
                     j++;
                  }

                  B->newObject(string2, string1, size);
               }
               string = strtok_s(NULL, del, &cont);
            }
            char* part2 = new char[maxx];
            Object* q = B->getFirst();
            char* findElement;
            while(q != NULL) {
               strcpy_s(part2, maxx, part1);
               string = strtok_s(part2, del2, &cont);
               while (string != NULL) {
                  findElement = strstr(string, q->getName());

                  if ((findElement == NULL) || (strlen(string) != strlen(q->getName()))) {
                     string = strtok_s(NULL, del2, &cont);
                     continue;
                  }

                  q->incAmount();

                  string = strtok_s(NULL, del2, &cont);
               }
               q = q->next;
            }
         }
      }
   }

   StaticTable* table = new StaticTable(B);

   table->heapSort();
   table->printAll();

   return 0;
}