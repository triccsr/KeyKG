#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <unistd.h>

#include <cassert>
#include <cstddef>
#include <queue>
#include <stdexcept>
#include <vector>

using std::max;
using std::vector;

template <class T>
class FibHeap;

template <class T>
class FibNode {
  friend class FibHeap<T>;

 public:
  FibNode *parent{nullptr};
  FibNode *firstChild{nullptr};
  FibNode *leftSibling{nullptr}, *rightSibling{nullptr};
  size_t rank{0};
  bool marked{false};
  T value;
  FibNode() = default;
  explicit FibNode(T v) : value(std::move(v)) {}
};

template <class T>
class FibHeap {
  typedef FibNode<T> Node;

  bool maxHeap;
  Node *minRoot;
  Node *firstRoot;
  size_t rootNum;

  bool less_than(const T &a, const T &b) {
    if (maxHeap) {
      return b < a;
    }
    return a < b;
  }

  void cut_subtree(Node *subRoot) {  // cut the subtree rooted subRoot from its parent and siblings
    if (subRoot->parent != nullptr) {
      subRoot->parent->rank -= 1;
      if (subRoot == subRoot->parent->firstChild) {
        subRoot->parent->firstChild = subRoot->rightSibling;
      }
    } else {
      rootNum -= 1;
      if (firstRoot == subRoot) {
        firstRoot = subRoot->rightSibling;
      }
    }
    if (subRoot->leftSibling != nullptr) {
      subRoot->leftSibling->rightSibling = subRoot->rightSibling;
    }
    if (subRoot->rightSibling != nullptr) {
      subRoot->rightSibling->leftSibling = subRoot->leftSibling;
    }
    subRoot->leftSibling = subRoot->rightSibling = subRoot->parent = nullptr;
  }

  void insert_child(Node *child, Node *parent) {
    child->parent = parent;
    child->rightSibling = parent->firstChild;
    if (parent->firstChild != nullptr) parent->firstChild->leftSibling = child;
    parent->firstChild = child;
    parent->rank += 1;
  }

  void delete_node(Node *now, bool _free) {
    cut_subtree(now);
    for (Node *ch = now->firstChild; ch != nullptr; ch = ch->rightSibling) {
      ch->parent = nullptr;
    }
    if (_free) {
      delete now;
    }
  }

  void delete_tree(Node *treeRoot) {
    for (Node *ch = treeRoot->firstChild, *oldRightSibling; ch != nullptr; ch = oldRightSibling) {
      oldRightSibling = ch->rightSibling;
      delete_tree(ch);
    }
    delete_node(treeRoot, true);
  }
  Node *link_2_subtrees(Node *root1, Node *root2) {
    if (root2 == nullptr) return root1;
    if (root1 == nullptr) return root2;
    if (less_than(root1->value, root2->value)) {
      insert_child(root2, root1);
      return root1;
    } else {
      insert_child(root1, root2);
      return root2;
    }
  }
  void insert_root(Node *newRoot) {
    newRoot->rightSibling = firstRoot;
    if (firstRoot != nullptr) firstRoot->leftSibling = newRoot;
    newRoot->parent = nullptr;
    firstRoot = newRoot;
    if (minRoot == nullptr || less_than(newRoot->value, minRoot->value)) {
      minRoot = newRoot;
    }
    rootNum += 1;
  }
  void decrease(Node *node, T newValue, bool isNegInf) {
    assert(less_than(newValue, node->value) || isNegInf);
    node->value = newValue;
    if (node->parent != nullptr && (less_than(node->value, node->parent->value) || isNegInf)) {
      Node *nowCut = node;
      while (nowCut->parent != nullptr) {
        nowCut->marked = false;
        Node *pa = nowCut->parent;
        cut_subtree(nowCut);
        insert_root(nowCut);
        if (pa->parent == nullptr) {  // parent is a root
          break;
        }
        if (pa->marked) {
          nowCut = pa;
        } else {
          pa->marked = true;
          break;
        }
      }
    }
    if (node->parent == nullptr && (less_than(node->value, minRoot->value) || isNegInf)) {
      minRoot = node;
    }
  }
  FibNode<T>* push(T value, Node *newNode) {
    *newNode = Node(value);
    insert_root(newNode);
    return newNode;
  }
  void pop(bool deleteMinRoot) {
    if (minRoot == nullptr) {
      throw std::runtime_error("pop from an empty heap");
    }
    Node *popRoot = minRoot;
    for (Node *ch = popRoot->firstChild, *oldRightSibling; ch != nullptr; ch = oldRightSibling) {
      oldRightSibling = ch->rightSibling;
      // roots.push_back(ch);
      cut_subtree(ch);
      insert_root(ch);
    }
    delete_node(popRoot, deleteMinRoot);

    size_t maxRootRank = 0;
    for (Node *root = firstRoot; root != nullptr; root = root->rightSibling) {
      maxRootRank = max(maxRootRank, root->rank);
    }
    vector<Node *> rank2Root(maxRootRank + rootNum, nullptr);

    for (Node *root = firstRoot, *oldRightSibling; root != nullptr; root = oldRightSibling) {
      oldRightSibling = root->rightSibling;
      size_t newRank = root->rank;
      Node *newRoot = root;
      cut_subtree(root);
      while (rank2Root[newRank] != nullptr) {
        newRoot = link_2_subtrees(newRoot, rank2Root[newRank]);
        rank2Root[newRank] = nullptr;
        newRank += 1;
      }
      rank2Root[newRank] = newRoot;
    }
    assert(firstRoot == nullptr);
    assert(rootNum == 0);
    minRoot = nullptr;
    for (size_t rk = 0; rk < rank2Root.size(); ++rk) {
      if (rank2Root[rk] != nullptr) {
        insert_root(rank2Root[rk]);
      }
    }
  }

 public:
  typedef Node *iterator;
  void clear() {
    for (Node *root = firstRoot, *oldRightSibling; root != nullptr; root = oldRightSibling) {
      oldRightSibling = root->rightSibling;
      delete_tree(root);
    }
    minRoot = firstRoot = nullptr;
    rootNum = 0;
  }
  FibHeap() : maxHeap(false), minRoot(nullptr), firstRoot(nullptr), rootNum(0) {}
  explicit FibHeap(bool isMaxHeap)
      : maxHeap(isMaxHeap), minRoot(nullptr), firstRoot(nullptr), rootNum(0) {}
  ~FibHeap() { clear(); }
  [[nodiscard]] bool empty() const { return firstRoot == nullptr; }

  iterator push(T value) {
    Node *newNode = new Node(value);
    insert_root(newNode);
    return newNode;
  }
  [[nodiscard]] T top() const {
    if (minRoot == nullptr) {
      throw std::runtime_error("get top from an empty heap");
    }
    return minRoot->value;
  }
  void pop() {
    pop(true);
  }
  T poll() {
    T ret = top();
    pop(true);
    return ret;
  }
  void modify(iterator heapNodeIter, T newValue) {
    if (less_than(newValue, heapNodeIter->value)) {
      decrease(heapNodeIter, newValue, false);
    } else if (less_than(heapNodeIter->value, newValue)) {
      decrease(heapNodeIter, newValue, true);
      pop(false);
      push(newValue, heapNodeIter);
    }
  }
  void print() {
    printf("heap status:\n");
    std::queue<Node *> q;
    for (Node *root = firstRoot; root != nullptr; root = root->rightSibling) {
      q.push(root);
    }
    while (!q.empty()) {
      Node *now = q.front();
      q.pop();
      printf("%p's parent is %p, childrens: ", now, now->parent);
      for (Node *ch = now->firstChild; ch != nullptr; ch = ch->rightSibling) {
        printf("%p, ", ch);
        q.push(ch);
      }
      printf("\n");
    }
  }
};
#endif