/**
 * Copyright 2014 (c) Lei Xu <eddyxu@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vobla/configuration.h"
#include <boost/lexical_cast.hpp>
#include <string>
#include <unordered_map>
#include "vobla/gutil/strings/case.h"

using std::string;
using std::unordered_map;

namespace vobla {

Configuration::~Configuration() {
}

bool Configuration::GetBool(const Key& key) const {
  string v = strings::ToLower(Get(key));
  if (v == "1" || v == "true" || v == "yes") {
    return true;
  } else if (v == "0" || v == "false" || v == "no") {
    return false;
  }
  throw BadValueException();
}

int Configuration::GetInt(const Key& key) const {
  try {
    string v = Get(key);
    return boost::lexical_cast<int64_t>(v);
  } catch (boost::bad_lexical_cast e) {
    throw BadValueException();
  }
}

int64_t Configuration::GetInt64(const Key& key) const {
  try {
    string v = Get(key);
    return boost::lexical_cast<int64_t>(v);
  } catch (boost::bad_lexical_cast e) {
    throw BadValueException();
  }
}

}  // namespace vobla
