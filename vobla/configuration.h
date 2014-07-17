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

#ifndef VOBLA_CONFIGURATION_H_
#define VOBLA_CONFIGURATION_H_

#include <exception>
#include <string>

namespace vobla {

class Status;

class Configuration {
 public:
  typedef std::string Key;

  class BadValueException : public std::exception {
  };

  class KeyNotFoundException : public std::exception {
  };

  virtual ~Configuration();

  virtual Status Load(const std::string& path) = 0;

  virtual bool Has(const Key& key) = 0;

  /**
   * \brief Gets boolean value.
   * It accepts 'yes', 'true' or '1' as true.
   */
  virtual bool GetBool(const Key& key) const;

  virtual int64_t GetInt64(const Key& key) const;

  virtual int GetInt(const Key& key) const;

  virtual std::string Get(const Key& key) const = 0;
};

}  // namespace vobla

#endif  // VOBLA_CONFIGURATION_H_
