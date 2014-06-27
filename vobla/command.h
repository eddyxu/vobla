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

#ifndef VOBLA_COMMAND_H_
#define VOBLA_COMMAND_H_

#include <boost/utility.hpp>
#include <string>
#include <vector>

namespace vobla {

class Status;

/**
 * \brief Command interface.
 */
class Command : boost::noncopyable {
 public:
  Command();

  virtual ~Command();

  virtual Status parseArgs(int argc, char* argv[]) = 0;

  virtual Status run() = 0;

  virtual void printHelp() = 0;
};

}  // namespace vobla

#endif  // VOBLA_COMMAND_H_