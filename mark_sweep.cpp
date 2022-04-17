#include<bits/stdc++.h>
using namespace std;

template <typename... T>
void print(const T &... t) {
  (void)std::initializer_list<int>{(std::cout << t << "", 0)...};
  std::cout << "\n";
}

struct Node;
struct TraceableObject;
struct Header;


/**
 * Objects tracing information: allocation pointer to header.
*/
static std::map<TraceableObject *, Header *> traceInfo;


/**
 * Object header.
*/
struct Header {
  bool marked;
  size_t size;
};


/**
 * The `Traceable` struct is used as a base class
 * for any object which should be managed by GC.
 */
struct TraceableObject {
  Header *getHeader() { return traceInfo.at(this); }

  static void *operator new(size_t size) {
    // Allocate a block:
    void *object = ::operator new(size);

    // Create an object header for it:
    auto header = new Header{false,size};
    traceInfo.insert(make_pair((TraceableObject *)object, header));

    // Result is the actual block:
    return object;
  }
};

/**
 * Traceable Node structure.
 * Contains the name of the node
 * pointers to left and right sub-nodes,
 * forming a tree.
 */
struct Node : public TraceableObject {
  char name;

  Node *left;
  Node *right;
};

void dump(const char *label) {
  printf("\n------------------------------------------------\n");
  printf(label);

  printf("\n{\n");

  for (const auto &it : traceInfo) {
    auto node = reinterpret_cast<Node *>(it.first);

    print("  [", node->name, "] ", it.first, ": {.marked = ", it.second->marked,
          ", .size = ", it.second->size, "}, ");
  }

  printf("}\n");
}

/**
 * Go through object fields, and see if we have any
 * which are recorded in the traceInfo.
 */
std::vector<TraceableObject *> getPointers(TraceableObject *object) {
  auto p = (uint8_t *)object;
  auto end = (p + object->getHeader()->size);
  std::vector<TraceableObject *> result;
  while (p < end) {
    auto address = (TraceableObject *)*(uintptr_t *)p;
    if (traceInfo.count(address) != 0) {
      result.emplace_back(address);
    }
    p++;
  }
  return result;
}

// mark phase
void mark(std::vector<TraceableObject *> worklist) {
  
  while (!worklist.empty()) {
    auto o = worklist.back();
    worklist.pop_back();
    auto header = o->getHeader();

    if (!header->marked) {
      header->marked = true;
      for (const auto &p : getPointers(o)) {
        worklist.push_back(p);
      }
    }
  }
}

// sweep phase
void sweep() {
  auto it = traceInfo.cbegin();
  while (it != traceInfo.cend()) {
    if (it->second->marked) {
      ++it;
    } else {
      it = traceInfo.erase(it);
      delete it->first;
    }
  }
}

// mark sweep 
void gc(std::vector<TraceableObject*> worklist) {
  mark(worklist);
  dump("After mark:");

  sweep();
  dump("After sweep:");
}


Node *createGraph() {
  Node* H = new Node();
  H->name = '8';
  Node* G = new Node();
  G->name = '7';
  G->right = H;
  Node* F = new Node();
  F->name = '6';
  
  Node* E = new Node();
  E->name = '5';
  E->right = G;
  E->left = F;

  Node* D = new Node();
  D->name = '4';
  Node* C = new Node();
  C->name = '3';
  C->left = D;
  C->right = E;
  Node* B = new Node();
  B->name = '2';
  
  Node* A = new Node();
  A->name = '1';
  A->left = B;
  A->right = C;
  return A;  // Root
}

int main(int argc, char const *argv[]) {

  Node* A = createGraph();
  std::vector<TraceableObject*> worklist;
  worklist.push_back(A);
  // Full alive graph:
  dump("Allocated graph:");

  // Detach the whole right sub-tree:
  A->right = nullptr;

  // Run GC:
  gc(worklist);

  return 0;
}

