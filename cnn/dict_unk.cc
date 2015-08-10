#include "dict_unk.h"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

namespace cnn {

std::vector<int> ReadSentence(const std::string& line, Dict_U* sd) {
  std::istringstream in(line);
  std::string word;
  std::string sep = "|||";
  std::vector<int> res;
  while(in) {
    in >> word;
    if (word.empty()) break;
    res.push_back(sd->Convert(word));
  }
  return res;
}

void ReadSentencePair(const std::string& line, std::vector<int>* s, Dict* sd, std::vector<int>* t, Dict_U* td) {
  std::istringstream in(line);
  std::string word;
  std::string sep = "|||";
  Dict* d = sd;
  std::vector<int>* v = s;
  while(in) {
    in >> word;
    if (!in) break;
    if (word == sep) { d = td; v = t; continue; }
    v->push_back(d->Convert(word));
  }
}

void CountWords(const std::string& line, Dict_U* sd){
  std::istringstream in(line);
  std::string word;
  std::string sep = "|||";
  while(in) {
    in >> word;
    if (word.empty()) break;
    sd->Count(word);
  }
  return;
}

} // namespace cnn
