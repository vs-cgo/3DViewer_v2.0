#include "model_test.h"
namespace s21 {
TEST_F(ModelTest, OpenFile) {
  std::string str = "obj/cube.obj";
  controller.Parsing(str);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  s21::Data &d = controller.GetData();
  for (size_t i = 0; i < d.vertex.size(); ++i) {
    EXPECT_NEAR(d.vertex[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, OpenFile2) { EXPECT_NO_THROW(controller.Parsing(tree)); }

TEST_F(ModelTest, OpenFile3) { EXPECT_NO_THROW(controller.Parsing(triangle)); }

TEST_F(ModelTest, NoFile) {
  std::string str = "obj/NoFile.obj";
  EXPECT_THROW(controller.Parsing(str), s21::not_open);
}

TEST_F(ModelTest, Empty) {
  std::string str = "obj/empty.obj";
  EXPECT_THROW(controller.Parsing(str), s21::bad_file);
}

TEST_F(ModelTest, BadFile) {
  std::string str = "obj/bad_cube.obj";
  EXPECT_THROW(controller.Parsing(str), s21::bad_file);
}

TEST_F(ModelTest, BadFile2) {
  std::string str = "obj/bad_cube2.obj";
  EXPECT_THROW(controller.Parsing(str), s21::bad_file);
}

TEST_F(ModelTest, Zoom) {
  controller.Parsing(cube);
  Command *c = new ZoomCommand(2);
  controller.Run(c);
  std::vector<double> expected = {2, 2,  -2, -2, 2,  -2, -2, 2,  2,  2, 2,  2,
                                  2, -2, 2,  -2, -2, 2,  -2, -2, -2, 2, -2, -2};
  EXPECT_TRUE(controller.GetData().vertex == expected);
}

TEST_F(ModelTest, Zoom2) {
  controller.Parsing(cube);
  Command *c = new ZoomCommand(3);
  controller.Run(c);
  std::vector<double> expected = {3, 3,  -3, -3, 3,  -3, -3, 3,  3,  3, 3,  3,
                                  3, -3, 3,  -3, -3, 3,  -3, -3, -3, 3, -3, -3};
  EXPECT_TRUE(controller.GetData().vertex == expected);
}

TEST_F(ModelTest, Zoom3) {
  controller.Parsing(cube);
  Command *c = new ZoomCommand(0.5);
  controller.Run(c);
  std::vector<double> expected = {
      0.5, 0.5,  -0.5, -0.5, 0.5,  -0.5, -0.5, 0.5,  0.5,  0.5, 0.5,  0.5,
      0.5, -0.5, 0.5,  -0.5, -0.5, 0.5,  -0.5, -0.5, -0.5, 0.5, -0.5, -0.5};
  EXPECT_TRUE(controller.GetData().vertex == expected);
}

TEST_F(ModelTest, AroundX) {
  controller.Parsing(cube);
  Command *c = new AroundXCommand(90 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {
      1.0, 1.0,  1.0,  -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,  1.0, -1.0, 1.0,
      1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,  -1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}
TEST_F(ModelTest, AroundX2) {
  controller.Parsing(cube);
  Command *c = new AroundXCommand(125 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {
      1.0000000,  0.2455756,  1.3927285,  -1.0000000, 0.2455756,  1.3927285,
      -1.0000000, -1.3927285, 0.2455756,  1.0000000,  -1.3927285, 0.2455756,
      1.0000000,  -0.2455756, -1.3927285, -1.0000000, -0.2455756, -1.3927285,
      -1.0000000, 1.3927285,  -0.2455756, 1.0000000,  1.3927285,  -0.2455756};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}
TEST_F(ModelTest, AroundY) {
  controller.Parsing(cube);
  Command *c = new AroundYCommand(90 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {-1.0, 1.0, -1.0, -1.0, 1.0, 1.0,  1.0,  1.0,
                                  1.0,  1.0, 1.0,  -1.0, 1.0, -1.0, -1.0, 1.0,
                                  -1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}
TEST_F(ModelTest, AroundY2) {
  controller.Parsing(cube);
  Command *c = new AroundYCommand(-90 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {1.0,  1.0,  1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,
                                  -1.0, -1.0, 1.0, 1.0,  -1.0, -1.0, 1.0,  -1.0,
                                  -1.0, -1.0, 1.0, -1.0, -1.0, 1.0,  -1.0, 1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, AroundY3) {
  controller.Parsing(cube);
  Command *c = new AroundYCommand(125 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {
      -1.3927285, 1.0000000,  -0.2455756, -0.2455756, 1.0000000,  1.3927285,
      1.3927285,  1.0000000,  0.2455756,  0.2455756,  1.0000000,  -1.3927285,
      0.2455756,  -1.0000000, -1.3927285, 1.3927285,  -1.0000000, 0.2455756,
      -0.2455756, -1.0000000, 1.3927285,  -1.3927285, -1.0000000, -0.2455756};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, AroundZ) {
  controller.Parsing(cube);
  Command *c = new AroundZCommand(90 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {
      -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0,  -1.0, 1.0, 1.0,
      1.0,  1.0, 1.0,  1.0,  -1.0, 1.0,  1.0,  -1.0, -1.0, 1.0,  1.0, -1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, AroundZ2) {
  controller.Parsing(cube);
  Command *c = new AroundZCommand(-90 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {1.0, -1.0, -1.0, 1.0, 1.0,  -1.0, 1.0,  1.0,
                                  1.0, 1.0,  -1.0, 1.0, -1.0, -1.0, 1.0,  -1.0,
                                  1.0, 1.0,  -1.0, 1.0, -1.0, -1.0, -1.0, -1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, AroundZ3) {
  controller.Parsing(cube);
  Command *c = new AroundZCommand(125 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {
      -1.3927285, 0.2455756,  -1.0000000, -0.2455756, -1.3927285, -1.0000000,
      -0.2455756, -1.3927285, 1.0000000,  -1.3927285, 0.2455756,  1.0000000,
      0.2455756,  1.3927285,  1.0000000,  1.3927285,  -0.2455756, 1.0000000,
      1.3927285,  -0.2455756, -1.0000000, 0.2455756,  1.3927285,  -1.0000000};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}
TEST_F(ModelTest, AroundZ4) {
  controller.Parsing(point);
  Command *c = new AroundZCommand(57 * 0.017453292519943295474);
  controller.Run(c);
  std::vector<double> expected = {-0.2940315, 1.3833096, -1.0};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
}

TEST_F(ModelTest, TranslationX) {
  controller.Parsing(cube);
  Command *c = new TranslateXCommand(5.5);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + 5.5, kEps);
  }
}

TEST_F(ModelTest, TranslationX2) {
  controller.Parsing(cube);
  Command *c = new TranslateXCommand(-2.3);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + (-2.3), kEps);
  }
}

TEST_F(ModelTest, TranslationY) {
  controller.Parsing(cube);
  Command *c = new TranslateYCommand(1.7);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 1; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + 1.7, kEps);
  }
}

TEST_F(ModelTest, TranslationY2) {
  controller.Parsing(cube);
  Command *c = new TranslateYCommand(-2.3);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 1; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + (-2.3), kEps);
  }
}
TEST_F(ModelTest, TranslationZ) {
  controller.Parsing(cube);
  Command *c = new TranslateZCommand(1.7);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 2; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + 1.7, kEps);
  }
}

TEST_F(ModelTest, TranslationZ2) {
  controller.Parsing(cube);
  Command *c = new TranslateZCommand(-2.3);
  controller.Run(c);
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};
  std::vector<double> res = controller.GetData().vertex;
  for (size_t i = 2; i < res.size(); i += 3) {
    EXPECT_NEAR(res[i], expected[i] + (-2.3), kEps);
  }
}

TEST_F(ModelTest, Memento) {
  controller.Parsing(cube);

  std::vector<double> res = controller.GetData().vertex;
  std::vector<double> expected = {1, 1,  -1, -1, 1,  -1, -1, 1,  1,  1, 1,  1,
                                  1, -1, 1,  -1, -1, 1,  -1, -1, -1, 1, -1, -1};

  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
  // SaveCommand
  Memento *m = nullptr;
  Command *sl = new SaveCommand(&m);
  controller.Run(sl);

  // Change vertex
  Command *c = new ZoomCommand(2);
  controller.Run(c);

  // Check
  res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i] / 2, expected[i], kEps);
  }
  // LoadComand
  sl = new LoadCommand(&m);
  controller.Run(sl);
  res = controller.GetData().vertex;
  for (size_t i = 0; i < res.size(); ++i) {
    EXPECT_NEAR(res[i], expected[i], kEps);
  }
  delete m;
}
}  // end namespace s21
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
