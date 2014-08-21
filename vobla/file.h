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

#ifndef VOBLA_FILE_H_
#define VOBLA_FILE_H_

#include <string>

namespace vobla {

class Status;

/**
 * Encapsure File related operations.
 */
class File {
 public:
  // Factory methods.
  static File* Open(const std::string& uri, int oflags);

  static File* Create(const std::string& uri);

  File() = default;

  virtual ~File();

  /** Opens a URI and returns the operation status */
  virtual Status open(const std::string& uri, int oflag);

  /** Close an opened file */
  virtual Status close();

  int fd() const;

 private:
  int fd_ = 0;
};

}  // namespace vobla

#endif  // VOBLA_FILE_H_
