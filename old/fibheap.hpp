#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>
#include <unistd.h>
using std::list;

template <class T> class FibNode {
  public:
  bool marked{false};
  FibNode *parent{nullptr};
  list<FibNode *> children;
  T value;
  explicit FibNode(T value, FibNode *parent)
      : marked(false), parent(parent), value(value) {
    this->value = value;
  }
  size_t rank() const{
    return children.size();
  }
};

template <class T> class FibHeap {
  typedef FibNode<T> Node;
  list<Node *> roots;
  Node *minRoot;
  //typename list<Node *>::iterator minRootIter;
  Node *link_tree(Node *firstRoot, Node *secondRoot) {
    if (firstRoot->value < secondRoot->value) {
      secondRoot->parent = firstRoot;
      firstRoot->children.push_back(secondRoot);
      return firstRoot;
    } else {
      firstRoot->parent = secondRoot;
      secondRoot->children.push_back(firstRoot);
      return secondRoot;
    }
  }
  void cut_melt_up(Node *now) {
    while (now->parent != nullptr) {
      Node *p = now->parent;
      now->parent = nullptr;
      if (p->marked == false) {
        p->marked = true;
        break;
      } else {
        now=p;
      }
    }
  }

public:
  FibHeap() : minRoot(nullptr) { roots.clear(); }
  typedef const Node *iterator;

  iterator push(T value) {
    Node* newRoot = new Node(value, nullptr);
    roots.insert(roots.end(), newRoot);
    if (minRoot==nullptr || newRoot->value < minRoot->value)
      minRoot = newRoot;
    return newRoot;
  }
  void pop() {
    if (roots.empty()) {
      throw std::runtime_error("pop from an empty heap");
      return;
    }
    //roots.erase(minRootIter);
    for (Node *ch : minRoot->children) {
      if(ch->parent==minRoot){
        ch->parent=nullptr;
        roots.push_back(ch);
      }
    }
    delete minRoot;
    std::list<Node*> rank2NewRoot;
    for (Node* oldRoot:roots) {
      
    }
    roots=std::move(rank2NewRoot);
    minRoot = nullptr;
    for (Node *newRoot : roots) {
      if (minRoot == nullptr || newRoot->value < minRoot->value) {
        minRoot=newRoot;
      }
    }
  }
  T top() { return minRoot->value; }
  void decrease(iterator iter, T newValue) {
    Node* it=iter;
    if (newValue > it->value) {
      throw std::range_error("new value is greater than old value");
      return;
    }
    if (newValue == it->value)
      return;
    it->value = newValue;
    if (it->parent == nullptr) {
      if (it->value < minRoot->value) {
        minRoot=it;
      }
    } else if (it->value > it->parent->value) {
      cut_melt_up(it);
    }
  }

};
#endif