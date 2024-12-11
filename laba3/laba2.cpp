#include <algorithm>
#include <fstream>
#include <iostream>

using llu = unsigned long long;

class TRBTree;

class TNode {
public:
  std::string Key;
  llu Value;
  bool Red;
  TNode *Left;
  TNode *Right;
  TNode *Parent;

  TNode(std::string key, llu value);

  void Rotate_L(TRBTree *t);
  void Rotate_R(TRBTree *t);
  void Insert(TRBTree *t);
  void Delete(TRBTree *t);

  static bool IsRed(const TNode *node);
};

class TRBTree {
public:
  TNode *Root;

  void Insert(const std::string &key, llu value, bool fromFile) {
    if (Root != nullptr) {
      if (Root->Key == key) {
        std::cout << "Exist" << std::endl;
        return;
      }
      auto newNode = new TNode(key, value);
      Insert(newNode, this->Root, fromFile);
    } else {
      auto newNode = new TNode(key, value);
      Root = newNode;
      Root->Red = false;
      if (!fromFile) {
        std::cout << "OK" << std::endl;
      }
    }
  }

  TNode *Find(const std::string &key) {
    auto currentNode = this->Root;
    while (currentNode != nullptr) {
      int compRes = key.compare(currentNode->Key);
      if (compRes < 0) {
        currentNode = currentNode->Left;
      } else if (compRes > 0) {
        currentNode = currentNode->Right;
      } else {
        return currentNode;
      }
    }
    return nullptr;
  }

  void FindNode(const std::string &key) {
    TNode *node = Find(key);
    if (node == nullptr) {
      std::cout << "NoSuchWord" << std::endl;
      return;
    }
    std::cout << "OK: " << node->Value << std::endl;
  }

  void DeleteNode(const std::string &key) {
    TNode *z = Find(key);
    if (z == nullptr) {
      std::cout << "NoSuchWord" << std::endl;
      return;
    }
    DeleteNode(z);
  }

  void WriteTreeAlter(std::string &path) {
    std::ofstream file(path, std::ios::trunc | std::ios::binary);
    WriteBinary(this->Root, file);
    file.close();
  }

  void WriteBinary(TNode *node, std::ofstream &file) {
    if (node == nullptr) {
      return;
    }
    size_t size = node->Key.length();
    file.write(reinterpret_cast<char *>(&size), sizeof(size));
    file.write(node->Key.c_str(), size);
    file.write(reinterpret_cast<char *>(&node->Value), sizeof(node->Value));
    file.flush();
    WriteBinary(node->Left, file);
    WriteBinary(node->Right, file);
  }

  static TRBTree *ReadTreeAlter(std::string &source) {
    std::ifstream file(source, std::ios::binary);
    size_t size;
    auto tree = new TRBTree();
    while (file.read(reinterpret_cast<char *>(&size), sizeof(size))) {
      std::string key;
      llu value;
      key.resize(size);
      file.read(key.data(), size);
      file.read(reinterpret_cast<char *>(&value), sizeof(value));
      tree->Insert(key, value, true);
    }
    file.close();
    return tree;
  }

  void Clean(TNode *t) {
    if (t == nullptr) {
      return;
    }
    Clean(t->Left);
    Clean(t->Right);
    delete t;
  }

  ~TRBTree() { Clean(Root); }

private:
  void Insert(TNode *newNode, TNode *currentNode, bool fromFile) {
    int compRes = newNode->Key.compare(currentNode->Key);
    if (compRes > 0) {
      if (currentNode->Right != nullptr) {
        Insert(newNode, currentNode->Right, fromFile);
      } else {
        newNode->Parent = currentNode;
        currentNode->Right = newNode;
        newNode->Insert(this);
        if (!fromFile) {
          std::cout << "OK" << std::endl;
        }
      }
    } else if (compRes < 0) {
      if (currentNode->Left != nullptr) {
        Insert(newNode, currentNode->Left, fromFile);
      } else {
        newNode->Parent = currentNode;
        currentNode->Left = newNode;
        newNode->Insert(this);
        if (!fromFile) {
          std::cout << "OK" << std::endl;
        }
      }
    } else {
      std::cout << "Exist" << std::endl;
    }
  }

  void DeleteNode(TNode *node) {
    if (node->Left != nullptr && node->Right != nullptr) {
      TNode *s = FindNext(node);
      node->Key = s->Key;
      node->Value = s->Value;
      node = s;
    }
    TNode *replace;
    if (node->Left == nullptr) {
      replace = node->Right;
    } else {
      replace = node->Left;
    }

    if (replace != nullptr) {
      replace->Parent = node->Parent;
      if (node->Parent == nullptr) {
        this->Root = replace;
      } else if (node == node->Parent->Left) {
        node->Parent->Left = replace;
      } else {
        node->Parent->Right = replace;
      }

      node->Parent = nullptr;
      node->Left = nullptr;
      node->Right = nullptr;

      if (!TNode::IsRed(node)) {
        replace->Delete(this);
      }
    } else if (node->Parent == nullptr) {
      this->Root = nullptr;
    } else {
      if (!TNode::IsRed(node)) {
        node->Delete(this);
      }

      if (node->Parent != nullptr) {
        if (node == node->Parent->Left) {
          node->Parent->Left = nullptr;
        } else if (node == node->Parent->Right) {
          node->Parent->Right = nullptr;
        }
        node->Parent = nullptr;
      }
    }
    delete node;
    std::cout << "OK" << std::endl;
  }

  TNode *FindNext(TNode *node) {
    if (node == nullptr) {
      return node;
    } else if (node->Right != nullptr) {
      auto child = node->Right;
      while (child->Left != nullptr) {
        child = child->Left;
      }
      return child;
    } else {
      auto parent = node->Parent;
      auto currentChild = node;
      while (parent != nullptr && currentChild == parent->Right) {
        currentChild = parent;
        parent = parent->Parent;
      }
      return parent;
    }
  }
};

TNode::TNode(std::string key, llu value) {
  this->Key = std::move(key);
  this->Value = value;
  this->Red = true;

  Left = nullptr;
  Right = nullptr;
  Parent = nullptr;
}

void TNode::Rotate_L(TRBTree *t) {
  TNode *node = this->Right;
  this->Right = node->Left;

  if (node->Left != nullptr) {
    node->Left->Parent = this;
  }
  node->Parent = this->Parent;
  if (this->Parent == nullptr) {
    t->Root = node;
  } else if (this == this->Parent->Left) {
    this->Parent->Left = node;
  } else {
    this->Parent->Right = node;
  }
  node->Left = this;
  this->Parent = node;
}

void TNode::Rotate_R(TRBTree *t) {
  TNode *node = this->Left;
  this->Left = node->Right;

  if (node->Right != nullptr) {
    node->Right->Parent = this;
  }
  node->Parent = this->Parent;
  if (this->Parent == nullptr) {
    t->Root = node;
  } else if (this == this->Parent->Left) {
    this->Parent->Left = node;
  } else {
    this->Parent->Right = node;
  }
  node->Right = this;
  this->Parent = node;
}

void TNode::Insert(TRBTree *t) {
  auto current = this;
  auto parentNode = current->Parent;

  while (IsRed(parentNode)) {
    auto dad = parentNode->Parent;
    if (dad->Left == parentNode) {
      auto uncle = dad->Right;
      if (IsRed(uncle)) {
        parentNode->Red = false;
        uncle->Red = false;
        dad->Red = true;
        current = dad;
        parentNode = current->Parent;
      } else {
        if (parentNode->Right == current) {
          parentNode->Rotate_L(t);
          std::swap(current, parentNode);
        }
        dad->Rotate_R(t);
        dad->Red = true;
        parentNode->Red = false;
        break;
      }
    } else {
      auto uncle = dad->Left;
      if (IsRed(uncle)) {
        dad->Red = true;
        parentNode->Red = false;
        uncle->Red = false;
        current = dad;
        parentNode = current->Parent;
      } else {
        if (parentNode->Left == current) {
          parentNode->Rotate_R(t);
          std::swap(parentNode, current);
        }
        dad->Rotate_L(t);
        parentNode->Red = false;
        dad->Red = true;
        break;
      }
    }
  }
  t->Root->Red = false;
}

void TNode::Delete(TRBTree *t) {
  auto current = this;
  while (current != t->Root && !IsRed(current)) {
    if (current == current->Parent->Left) {
      auto bro = current->Parent->Right;

      if (IsRed(bro)) {
        bro->Red = false;
        current->Parent->Red = true;
        current->Parent->Rotate_L(t);
        bro = current->Parent->Right;
      }

      if (!IsRed(bro->Left) && !IsRed(bro->Right)) {
        bro->Red = true;
        current = current->Parent;
      } else {
        if (!IsRed(bro->Right)) {
          bro->Left->Red = false;
          bro->Red = true;
          bro->Rotate_R(t);
          bro = current->Parent->Right;
        }
        bro->Red = current->Parent->Red;
        current->Parent->Red = false;
        bro->Right->Red = false;
        current->Parent->Rotate_L(t);
        current = t->Root;
      }
    } else {
      auto bro = current->Parent->Left;

      if (IsRed(bro)) {
        bro->Red = false;
        current->Parent->Red = true;
        current->Parent->Rotate_R(t);
        bro = current->Parent->Left;
      }

      if (!IsRed(bro->Right) && !IsRed(bro->Left)) {
        bro->Red = true;
        current = current->Parent;
      } else {
        if (!IsRed(bro->Left)) {
          bro->Right->Red = false;
          bro->Red = true;
          bro->Rotate_L(t);
          bro = current->Parent->Left;
        }
        bro->Red = current->Parent->Red;
        current->Parent->Red = false;
        bro->Left->Red = false;
        current->Parent->Rotate_R(t);
        current = t->Root;
      }
    }
  }
  current->Red = false;
}

bool TNode::IsRed(const TNode *node) {
  if (node == nullptr) {
    return false;
  }
  return node->Red;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  auto tree = new TRBTree();
  std::string key;
  llu value;

  std::string command;

  while (std::cin >> command) {
    switch (command[0]) {
    case '+':
      std::cin >> key;
      transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return std::tolower(c); });
      std::cin >> value;
      tree->Insert(key, value, false);
      break;
    case '-':
      std::cin >> key;
      transform(key.begin(), key.end(), key.begin(),
                [](unsigned char c) { return std::tolower(c); });
      tree->DeleteNode(key);
      break;
    case '!':
      std::cin >> command;
      if (command == "Save") {
        std::string path;
        std::cin >> path;
        tree->WriteTreeAlter(path);
        std::cout << "OK" << std::endl;
      }
      if (command == "Load") {
        std::string path;
        std::cin >> path;
        tree->Clean(tree->Root);
        delete tree;
        tree = TRBTree::ReadTreeAlter(path);
        std::cout << "OK" << std::endl;
      }
      break;
    default:
      transform(command.begin(), command.end(), command.begin(),
                [](unsigned char c) { return std::tolower(c); });
      tree->FindNode(command);
      break;
    }
  }
  tree->Clean(tree->Root);
  delete tree;
}
