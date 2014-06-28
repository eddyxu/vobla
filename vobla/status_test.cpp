/*
 * Copyright 2011-2014 (c) Lei Xu <eddyxu@gmail.com>
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

/**
 * \file vobla/status_test.cpp
 * \brief Unit tests for status class.
 */

#include <gtest/gtest.h>
#include <string>
#include "vobla/status.h"

using std::string;

namespace vobla {

TEST(StatusTest, TestConstructors) {
  // Default constructor
  Status s;
  EXPECT_TRUE(s.Ok());
  EXPECT_EQ(0, s.Error());
  EXPECT_TRUE(s.Message().empty());

  Status s1(0, "SUCCESS");
  EXPECT_TRUE(s1.Ok());
  EXPECT_EQ(0, s1.Error());
  EXPECT_EQ("SUCCESS", s1.Message());

  EXPECT_NE(s, s1);

  Status s2(1, "test failure");
  Status s3(s2);
  Status s4;
  s4 = s2;
  EXPECT_EQ(s2, s3);
  EXPECT_EQ(s3, s4);

  Status s5;
  EXPECT_TRUE(s5.Ok());
  s5.Set(1, "test failure");
  EXPECT_FALSE(s5.Ok());
  EXPECT_EQ(s2, s5);
}

TEST(StatusTest, TestConstructFromSystemError) {
  Status s = Status::system_error(ENOENT);
  EXPECT_EQ(-ENOENT, s.Error());
  EXPECT_EQ("No such file or directory", s.Message());

  s = Status::system_error(EACCES);
  EXPECT_EQ(-EACCES, s.Error());
  EXPECT_EQ("Permission denied", s.Message());
}

TEST(StatusTest, TestMoveConstructors) {
  Status s(1, "This is a test.");
  Status s1 = std::move(s);
  EXPECT_EQ(1, s1.Error());
  EXPECT_EQ(0, s.Error());
  EXPECT_EQ(string("This is a test."), s1.Message());
  EXPECT_TRUE(s.Message().empty());

  Status s2;
  s2 = std::move(s1);
  EXPECT_EQ(1, s2.Error());
  EXPECT_EQ(0, s1.Error());
  EXPECT_EQ(string("This is a test."), s2.Message());
  EXPECT_TRUE(s.Message().empty());
}

TEST(StatusTest, TestSetterAndGetter) {
  Status s;
  s.SetError(10);
  s.SetMessage("10");
  EXPECT_EQ(Status(10, "10"), s);
}

}  // namespace vobla
