#include <list>
#include <iostream>
#include <vector>

std::list<int> intersection(const std::list<int>& l1, const std::list<int>& l2) {
    std::list<int> result;
    std::list<int>::const_iterator it1 = l1.begin();
    std::list<int>::const_iterator it2 = l2.begin();

    while(it1 != l1.end() && it2 != l2.end()){
        if(*it1 < *it2) {
            ++it1;
        } else if(*it2 < *it1) {
            ++it2;
        } else {
            result.push_back(*it1);
            ++it1;
            ++it2;
        }
    }
    return result;
}

void print(const std::list<int>& l) {
  std::cout << "{";
  for (std::list<int>::const_iterator it = l.begin(); it != l.end(); ++it) {
    if (it != l.begin()) std::cout << ",";
    std::cout << *it;
  }
  std::cout << "}\n";
}

template<typename T>
struct Node {
  Node* next;
  T obj;
  Node(T o, Node* n=nullptr)
    : obj(o), next(n) { }
};

template<typename T>
int count_nodes(const Node<T>* head) {
  if(head == nullptr) return 0;
  return 1 + count_nodes(head->next);
}

int find_max_value(const std::vector<int>& v, int n) {
    if(n == 1) return v[0];
    int rest = find_max_value(v, n - 1);
    return (v[n - 1]  >  rest) ? v[n - 1] : rest;
}

int main() {
  // Test 1: max in the middle
  std::vector<int> a = {3, 9, 2, 7, 5};
  std::cout << "Test 1: max = " << find_max_value(a, a.size()) << "\n";

  // Test 2: single element, and negative values
  std::vector<int> b = {-8};
  std::cout << "Test 2: max = " << find_max_value(b, b.size()) << "\n";

  return 0;
}