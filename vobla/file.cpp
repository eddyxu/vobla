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

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include "vobla/file.h"
#include "vobla/status.h"

using std::string;

namespace vobla {

File::~File() {
  this->close();
}

Status File::open(const string& path, int oflag) {
  fd_ = ::open(path.c_str(), oflag);
  if (fd_ == -1) {
    return Status::system_error(errno);
  }
  return Status::OK;
}

Status File::close() {
  if (fd_ > 0) {
    ::close(fd_);
    fd_ = 0;
  }
  return Status::OK;
}

int File::fd() const {
  return fd_;
}

}  // namespace
