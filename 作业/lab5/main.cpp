#pragma warning(disable: 4786)
#include <iostream>
#include <string>
using namespace std;

template <class T>
struct ListNode {
      ListNode *prev;
      ListNode *next;
      T data;
    ListNode() : prev(NULL), next(NULL), data() {}
    ListNode(const T& val) : prev(NULL), next(NULL), data(val) {}
  };

// 简化迭代器
template <typename T>
class Iterator {
private:
    ListNode<T>* node;
public:
    Iterator(ListNode<T>* n = NULL) : node(n) {}

    T& operator*() { return node->data; }

    Iterator& operator++() {
        node = node->next;
        return *this;
    }
    Iterator& operator--() {
        node = node->prev;
        return *this;
    }

    bool operator!=(const Iterator& other) { return node != other.node; }
};

template <class T>
class myList{
private:
    //链表结
    ListNode<T> senti;
    size_t length;

public:
    myList(): length(0) {
        senti.prev = &senti;
        senti.next = &senti;
    }

    Iterator<T> begin() {return Iterator<T>(senti.next);}//外部迭代器
    Iterator<T> end() {return Iterator<T>(&senti);}


    void push_back(const T & val) {
        ListNode<T>* N = new ListNode<T>;
        N->data = val;
        senti.prev->next = N;
        N->prev = senti.prev;
        N->next = &senti;
        senti.prev = N;
        length++;
    }

    void print() {
        for(Iterator<T> i = this->begin(); i !=  this->end(); ++i) {
            cout << " " << *i;
        }
    }
    
    void sort() {
        for(Iterator<T> i = this->begin(); i !=  this->end(); ++i) {
            for(Iterator<T> j = i; j != this->end(); ++j) {
                if (*i > *j) {
                    T temp = *i;
                    *i = *j;
                    *j = temp;
                }
            }
        }
    }
    
    ~myList() {
        ListNode<T> * del = senti.next;
        ListNode<T> * temp = del;
        for(int i = 0; i < length; ++i) {
            del = del->next;
            delete temp;
            temp = del;
        }
    }
};

int main( )
{
   myList <int> t1;
   myList <string> t2;
   t1.push_back( 20 );
   t1.push_back( 10 );
   t1.push_back( 30 );

   cout << "Before sorting: c1 =";
   t1.print();
   cout << endl;

   t1.sort( );
   cout << "After sorting c1 =";
   t1.print();
   cout << endl;

    t2.push_back( string("Tom") );
    t2.push_back( string("Jame") );
    t2.push_back( string("Alice") );

    cout << "Before sorting: c1 =";
    t2.print();
    cout << endl;

    t2.sort( );
    cout << "After sorting c1 =";
    t2.print();
    cout << endl;

    return 0;
}
