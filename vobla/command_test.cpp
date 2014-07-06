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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "vobla/command.h"
#include "vobla/status.h"

using ::testing::ElementsAre;

namespace vobla {

class TestCommand : public Command {
 public:
  Status ParseArgs(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    return Status::OK;
  }

  Status Run() {
    return Status::OK;
  }
};

TEST(CommandFactoryTest, TestAddClass) {
  CommandFactory factory;
  REGISTER_COMMAND(factory, "test", TestCommand);
  auto names = factory.GetNames();
  EXPECT_THAT(names, ElementsAre("test"));
}

}  // namespace vobla
