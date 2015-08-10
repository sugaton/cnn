#ifndef CNN_DICT_U_H_
#define CNN_DICT_U_H_

#include <cassert>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include "dict.h"

#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp>
#endif

namespace cnn {

// this dict regard rare-words as a Unknown word (UNK).
// when we call Freeze, rare-words are changed to <UNK>
class Dict_U : public Dict {
 typedef std::unordered_map<std::string, int> Map;
 public:
  void Freeze(const int N=5) {
      Convert(unk);
      Erase_rare(N);
      frozen = true;
  }

  inline int Convert(const std::string& word) {
    auto i = d_.find(word);
    if (i == d_.end()) {
      if (frozen)
        return d_[unk];
      words_.push_back(word);
      return d_[word] = words_.size() - 1;
    } else {
      return i->second;
    }
  }
  inline const std::string& Convert(const int& id) const {
    assert(id < (int)words_.size());
    return words_[id];
  }
  inline void Count(const std::string& word){
    auto i = d_.find(word);
    if (i == d_.end()) {
      words_.push_back(word);
      d_[word] = words_.size() - 1;
      counter_[word] = 1;
    } else {
      ++(counter_[word]);
    }
  }
 private:
  const std::string unk = "<UNK>";
  Map counter_;
  // O(words_.size())
  void Erase_rare(const int N){
    for(auto it = counter_.begin(); it != counter_.end(); ++it)
      if(it->second < N)
        d_[it->first] = d_[unk];
  }
};
std::vector<int> ReadSentence(const std::string& line, Dict_U* sd);
void ReadSentencePair(const std::string& line, std::vector<int>* s, Dict* sd, std::vector<int>* t, Dict_U* td);
void CountWords(const std::string& line, Dict_U* sd);
} // namespace cnn

#endif
