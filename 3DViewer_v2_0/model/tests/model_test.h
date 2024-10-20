#ifndef _3DVIEWER_v2_0_MODEL_MODEL_TEST_H_
#define _3DVIEWER_v2_0_MODEL_MODEL_TEST_H_

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "../../controller/controller.h"

namespace s21 {
class ModelTest : public testing::Test {
 protected:
  ModelTest()
      : controller(),
        cube("obj/cube.obj"),
        point("obj/point.obj"),
        tree("obj/Low_tree.obj"),
        triangle("obj/3angle.obj"),
        kEps(1e-7){};
  ~ModelTest(){};
  void TearDown() override{};
  void SetUp() override{};

  Controller controller;
  std::string cube, point, tree, triangle;
  const double kEps;
};
}  // end namespace s21
#endif  // _3DVIEWER_v2_0_MODEL_MODEL_TEST_H_
