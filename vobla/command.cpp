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

#include <getopt.h>
#include <glog/logging.h>
#include <cstdio>
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

HelpCommand::HelpCommand(CommandFactory* fact) : factory_(fact) {
  CHECK_NOTNULL(factory_);
}

HelpCommand::~HelpCommand() {
}

Status HelpCommand::ParseArgs(int argc, char* argv[]) {
  // TODO(eddyxu): Provide python-like ArgParse later?
  int ch;
  static struct option longopts[] = {
    {"full", no_argument, 0, 0},
    {"short", no_argument, 0, 1},
    {0, 0, 0, 0},
  };
  while ((ch = getopt_long(argc, argv, "fs", longopts, NULL)) != -1) {
    switch (ch) {
      case 'f':
        break;
      default:
        usage();
    }
  }
  argc -= optind;
  argv += optind;
  if (argc > 0) {
    sub_command_ = argv[0];
  }
  return Status::OK;
}

Status HelpCommand::Run() {
  if (sub_command_.empty()) {
    fprintf(stderr, "Usage: %s help [command|topics]", program_.c_str());
  } else {
    Command* command = factory_->Get(sub_command_);
    if (command) {
      command->PrintHelp();
    } else {
      fprintf(stderr, "Unknown command: %s\n", sub_command_.c_str());
      return Status(-1, "Unknown command");
    }
  }
  return Status::OK;
}

void CommandFactory::Add(const string& name, Command* command) {
  CHECK(!ContainsKey(commands_, name));
  commands_[name].reset(command);
}

Command* CommandFactory::Get(const std::string& name) const {
  auto iter = commands_.find(name);
  if (iter == commands_.end()) {
    return nullptr;
  }
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
