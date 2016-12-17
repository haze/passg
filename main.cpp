#include <stdio.h>
#include <map>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <functional>
#include <vector>


std::map<std::string, std::function<std::string()>> modes;

void lower(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }

void upper(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }


std::string ambigious() { return "!@#$%^&*()_+-=,./\\;:[]{}|\'\"`~i"; }
std::string alphabet() { return "abcdefghijklmnopqrstuvwxyz"; }
std::string alphabet_caps() {
  std::string up_alpha = alphabet();
  upper(up_alpha);
  return alphabet() + up_alpha;
}

std::string char_to_string(char ch) {
  std::stringstream ss;
  ss << ch;
  return ss.str();
}

std::string generate_password(std::string mode, int length) {
  std::stringstream password;
  for(int i = 0; i < length; i++) {
    auto func = modes.find(mode)->second;
    password << func();
  }
  return password.str();
}

std::string ambigious_char() {
  std::string ambig = ambigious();
  return char_to_string(ambig.at(std::rand() % ambig.length()));
}

std::string random_num() {
  return std::to_string(std::rand() % 9);
}

std::string random_char() {
  std::string afull = alphabet_caps();
  return char_to_string(afull.at(std::rand() % afull.length()));
}

// why didnt i just make these two functions above return !!? >.<

// God i love programming
template< class T, class V > // todo: type constraints for map
std::vector<T> keys_from(std::map<T, V> map) {
  std::vector<T> keys;
  for(auto it = map.begin(); it != map.end(); ++it)
    keys.push_back(it->first);
  return keys;
}


std::string generate_mode_str() {
  std::stringstream ss;
  auto keys = keys_from(modes);
  ss << "[";
  for(auto it = keys.begin(); it != keys.end(); it++)
    if(std::distance(keys.begin(), it) == keys.size() - 1)
      ss << *it;
    else
      ss << *it << "|";
  ss << "]";
  return ss.str();
}

int main(int argc, char **argv) {
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("al"), [](){ return random_char(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("di"), [](){ return random_num(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("am"), [](){ return ambigious_char(); }));
    modes.insert(std::pair<std::string, std::function<std::string()>>(std::string("all"), [](){
            switch (std::rand() % 3) {
              case 0:
                return random_num();
              case 1:
                return random_char();
              case 2:
                return ambigious_char();
            }
          }));
  if (argc != 3) {
    std::cout << "please use this format: ./pwg " << generate_mode_str() << " [0..infinity]" << std::endl;
  } else {
    std::srand( std::time(NULL) );
    int length = std::atoi(argv[2]);
    std::string pw_type(argv[1]);
    lower(pw_type);

    if(modes.find(pw_type) == modes.end()) {
      std::cout << "invalid option: \"" << pw_type << "\"" << std::endl;
      return 1;
    } // avoid else !

    std::cout << generate_password(pw_type, length) << std::endl;

  }
}
