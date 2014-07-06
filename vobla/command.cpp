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

#include <glog/logging.h>
#include <map>
#include <string>
#include <vector>

#include "vobla/command.h"
#include "vobla/gutil/map_util.h"
#include "vobla/status.h"

using std::function;
using std::map;
using std::string;
using std::vector;

namespace vobla {

Command::Command() {
}

Command::~Command() {
}

void Command::PrintHelp() {
  if (!usage_.empty()) {
    fprintf(stdout, "Usage: %s", usage_.c_str());
  }
  if (!description_.empty()) {
    fprintf(stdout, "\n%s", description_.c_str());
  }
}

Status HelpCommand::Run() {
}

void CommandFactory::Add(const string& name, Command* command) {
  CHECK(!ContainsKey(commands_, name));
  commands_[name].reset(command);
}

Command* CommandFactory::Get(const std::string& name) const {
  auto iter = commands_.find(name);
  CHECK(iter != commands_.end());
  return iter->second.get();
}

vector<string> CommandFactory::GetNames() const {
  vector<string> tmp;
  tmp.reserve(commands_.size());
  for (const auto& name_and_cmd : commands_) {
    tmp.push_back(name_and_cmd.first);
  }
  return tmp;
}

}  // namespace vobla
