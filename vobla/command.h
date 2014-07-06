/*
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
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace vobla {

class CommandFactory;
class Status;

/**
 * \brief Command interface.
 *
 * Provides a convinient way for command line oriented program to use
 * sub-commands.
 *
 * E.g., /path/to/program foo --test arg1
 *
 * \code{.cpp}
 * class ListCommand : public vobla::Command {
 *    // ...
 * };
 *
 * CommandFactory factory;
 * REGISTER_COMMAND(factory, "list", ListCommand);
 * REGISTER_COMMAND(factory, "list", GetCommand);
 * \end{.cpp}
 */
class Command : boost::noncopyable {
 public:
  Command();

  virtual ~Command();

  virtual Status ParseArgs(int argc, char* argv[]) = 0;

  virtual Status Run() = 0;

  virtual void PrintHelp();

  virtual std::string usage() const { return usage_; }

  virtual std::string description() const { return description_; }

  std::string program() { return program_; }

  void set_program(const std::string& prog) { program_ = prog; }

 protected:
  std::string program_;
  std::string usage_;
  std::string description_;
};

class HelpCommand : public Command {
 public:
  HelpCommand(CommandFactory* factory);

  virtual ~HelpCommand();

  virtual Status ParseArgs(int argc, char* argv[]);

  virtual Status Run();

 private:
  std::string sub_command_;
  CommandFactory* factory_;
};

class CommandFactory {
 public:
  void Add(const std::string& name, Command* command);

  Command* Get(const std::string& name) const;

  /**
   * \brief Return all registered command names.
   */
  std::vector<std::string> GetNames() const;

 private:
  std::map<std::string, std::unique_ptr<Command>> commands_;
};

#define REGISTER_COMMAND(factory, name, cls) \
    factory.Add(name, new cls());

}  // namespace vobla

#endif  // VOBLA_COMMAND_H_
