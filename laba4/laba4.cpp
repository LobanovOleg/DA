#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using ui = unsigned int;

struct NumVal {
  int word;
  int string;
  ui value;
};

std::vector<int> ZFunction(const std::vector<NumVal> &text) {
  ui textSize = text.size();
  std::vector<int> result(textSize);
  ui left = 0;
  ui right = 0;
  for (ui i = 1; i < textSize; ++i) {
    result[i] =
        std::max(0, std::min(static_cast<int>(right - i), result[i - left]));
    while (i + result[i] < textSize &&
           text[result[i]].value == text[i + result[i]].value) {
      result[i]++;
    }
    if (i + result[i] > right) {
      left = i;
      right = i + result[i];
    }
  }
  return result;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  const int sep = 2456;
  ui input;
  int patternSize;

  std::string pattern;
  getline(std::cin, pattern);

  std::vector<NumVal> text;
  std::stringstream patternStream(pattern);

  while (patternStream >> input) {
    text.push_back({0, 0, input});
  }
  patternSize = text.size();
  text.push_back({0, 0, sep});

  std::string textLine;
  int number = 1;

  while (!std::cin.eof()) {
    getline(std::cin, textLine);
    std::stringstream textLineStream(textLine);
    int word = 1;
    while (textLineStream >> input) {
      text.push_back({word, number, input});
      word++;
    }
    number++;
  }

  auto zVector = ZFunction(text);

  for (ui i = patternSize + 1; i < zVector.size(); ++i) {
    if (zVector[i] == patternSize) {
      std::cout << text[i].string << ", " << text[i].word << std::endl;
    }
  }

  return 0;
}