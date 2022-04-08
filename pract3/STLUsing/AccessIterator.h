#pragma once
#include <iterator>

#include "Node.h"

using std::iterator;

class AccessIterator : public std::iterator<std::forward_iterator_tag, int> {

};