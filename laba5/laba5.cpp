#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#define MAXSIZE 27

typedef struct Help {
  Help *mas[MAXSIZE];
  unsigned list;
  unsigned length;
  unsigned first;
  Help *suff;
} Help;

class Suffix {
public:
  Suffix(std::istream &check);
  std::string &GetStr();
  void Find(std::istream &check);
  ~Suffix();

private:
  Help body;
  Help head;
  std::string str;
  std::vector<Help *> massiv;
};

Suffix::Suffix(std::istream &check) {
  char f;
  check >> f;
  f = tolower(f);
  while (f >= 'a' && f <= 'z') {
    str.push_back(f);
    check.get(f);
    f = tolower(f);
  }
  str.push_back('z' + 1);
  massiv.reserve(str.size());
  Help *suf = &head;
  Help *last = &body;
  Help *help = nullptr;
  unsigned suff_len = 0;
  unsigned last_list = 0;
  unsigned i = 0;
  short suff_i = -1;
  for (short j = 0; j < MAXSIZE; ++j) {
    body.mas[j] = &head;
    head.mas[j] = nullptr;
  }
  body.list = head.list = head.first = 0;
  head.length = 1;
  head.suff = &body;

  for (unsigned j = 0; j < str.size(); ++j) {
    last = &body;
    if (suff_len && str[j] == str[(suf->mas[suff_i]->first) + suff_len]) {
      ++suff_len;
    } else {
      while (suff_len) {
        help = (Help *)malloc(sizeof(Help));
        massiv.push_back(help);
        memset(help->mas, 0, MAXSIZE * sizeof(Help *));
        help->mas[str[suf->mas[suff_i]->first + suff_len] - 'a'] =
            suf->mas[suff_i];
        suf->mas[suff_i]->length -= suff_len;
        help->first = suf->mas[suff_i]->first;
        suf->mas[suff_i]->first += suff_len;
        help->length = suff_len;
        suf->mas[suff_i] = help;
        help->list = 0;
        last->suff = help;
        last = help;
        help = (Help *)malloc(sizeof(Help));
        massiv.push_back(help);
        help->first = j;
        help->length = str.size() - j;
        help->list = ++last_list;
        last->mas[str[j] - 'a'] = help;

        help = suf;
        suf = suf->suff;
        i = 0;

        while (suf->mas[str[help->mas[suff_i]->first + i] - 'a']->length <
               suff_len) {
          suf = suf->mas[str[help->mas[suff_i]->first + i] - 'a'];
          suff_len -= suf->length;
          i += suf->length;
        }
        if (suf->mas[str[help->mas[suff_i]->first + i] - 'a']->length ==
            suff_len) {
          suf = suf->mas[str[help->mas[suff_i]->first + i] - 'a'];
          suff_len = 0;
          last->suff = suf;
          break;
        } else {
          suff_i = str[help->mas[suff_i]->first + i] - 'a';
        }
      }
      if (!suff_len) {
        while (1) {
          if (!suf->mas[str[j] - 'a']) {
            help = (Help *)malloc(sizeof(Help));
            massiv.push_back(help);
            help->list = ++last_list;
            help->length = str.size() - j;
            help->first = j;
            suf->mas[str[j] - 'a'] = help;
            suf = suf->suff;
          } else {
            suff_i = str[j] - 'a';
            ++suff_len;
            break;
          }
        }
      }
    }
    if (suff_len && suff_len == suf->mas[suff_i]->length) {
      suf = suf->mas[suff_i];
      suff_len = 0;
    }
  }
}

std::string &Suffix::GetStr() { return str; }

void Suffix::Find(std::istream &check) {
  char f;
  Help *suf = &head;
  Help *help = nullptr;
  unsigned speed = 1;
  unsigned iter = 1;
  unsigned suff_len = 0;
  unsigned help_len = 0;
  short suff_i;
  check >> f;
  while (f >= 'a' && f <= 'z') {
    if (suff_len) {
      while (f != str[suf->mas[suff_i]->first + suff_len]) {
        if (speed == str.size()) {
          std::cout << iter << std::endl;
        }
        --speed;
        ++iter;
        help = suf;
        help_len = 0;
        suf = suf->suff;
        while (
            suff_len >
            suf->mas[str[help->mas[suff_i]->first + help_len] - 'a']->length) {
          suf = suf->mas[str[help->mas[suff_i]->first + help_len] - 'a'];
          help_len += suf->length;
          suff_len -= suf->length;
        }
        if (suff_len ==
            suf->mas[str[help->mas[suff_i]->first + help_len] - 'a']->length) {
          suf = suf->mas[str[help->mas[suff_i]->first + help_len] - 'a'];
          suff_len = 0;
          suff_i = f - 'a';
          break;
        } else {
          suff_i = str[help->mas[suff_i]->first + help_len] - 'a';
        }
      }
    }
    if (!suff_len) {
      suff_i = f - 'a';
      while (!suf->mas[suff_i]) {
        if (speed == str.size()) {
          std::cout << iter << std::endl;
        }
        suf = suf->suff;
        ++iter;
        --speed;
      }
    }
    ++speed;
    ++suff_len;
    if (suf->mas[suff_i]->length == suff_len) {
      suff_len = 0;
      suf = suf->mas[suff_i];
    }
    if (!check.get(f)) {
      break;
    }
    f = tolower(f);
  }
  if (speed == str.size()) {
    std::cout << iter << std::endl;
  }
}

Suffix::~Suffix() {
  for (unsigned j = 0; j < massiv.size(); ++j) {
    free(massiv[j]);
  }
}

int main() {
  while (std::cin.peek() != EOF) {
    Suffix tree(std::cin);
    tree.Find(std::cin);
  }
  return 0;
}