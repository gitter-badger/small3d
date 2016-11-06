/*
 *  main.cpp
 *
 *  Created on: 2014/10/18
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

// Without undefining strict ANSI, compilation in MinGW fails when C++11 is enabled
#ifdef __MINGW32__
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#endif
#endif

#include <gtest/gtest.h>
#include "Logger.hpp"
#include "Image.hpp"
#include "Model.hpp"
#include "BoundingBoxSet.hpp"
#include "WavefrontLoader.hpp"
#include "SceneObject.hpp"
#include "Renderer.hpp"



/* MinGW produces the following linking error, if the unit tests
 * are linked to the renderer:
 *    undefined reference to `SDL_SetMainReady'
 * This started occurring when GLEW was removed from the small3d block
 * and placed in an independent block. It probably has something to do
 * with the order in which the SDL libraries are linked (see
 * http://www.cplusplus.com/forum/beginner/110753/). It does not occur
 * in the sample game, only in these unit tests, when they are built
 * under MinGW.
 */
#ifndef __MINGW32__

#endif

using namespace small3d;
using namespace std;

TEST(LoggerTest, LogSomething) {
  deleteLogger();
  ostringstream oss;
  initLogger(oss);

  LOGINFO("It works");
  EXPECT_TRUE(oss.str().find("It works") != (string::npos));

  LOGERROR("Error test");
  EXPECT_TRUE(oss.str().find("Error test") != (string::npos));
  deleteLogger();

}

TEST(ImageTest, LoadImage) {

  Image image("resources/images/testImage.png");

  cout << "Image width " << image.getWidth() << ", height " << image.getHeight() << endl;

  const float *imageData = image.getData();

  unsigned long x = 0, y = 0;

  while (y < image.getHeight()) {
    x = 0;
    while (x < image.getWidth()) {

      const float *colour = &imageData[4 * y * image.getWidth() + 4 * x];

      EXPECT_GE(colour[0], 0.0f);
      EXPECT_LE(colour[0], 1.0f);
      EXPECT_GE(colour[1], 0.0f);
      EXPECT_LE(colour[1], 1.0f);
      EXPECT_GE(colour[2], 0.0f);
      EXPECT_LE(colour[2], 1.0f);
      EXPECT_EQ(1.0f, colour[3]);

      // Uncomment the following to actually see RGB values for each test image pixel
      // 			cout << "At (" << x << ", " << y << ") R: " << setprecision( 2 ) << colour[0] << endl;
      // 			cout << "At (" << x << ", " << y << ") G: " << setprecision( 2 ) << colour[1] << endl;
      // 			cout << "At (" << x << ", " << y << ") B: " << setprecision( 2 ) << colour[2] << endl;

      ++x;
    }
    ++y;
  }
}

TEST(ModelTest, LoadModel) {

  Model model;
  WavefrontLoader loader;

  loader.load("resources/models/Cube/Cube.obj", model);

  EXPECT_NE(0, model.vertexData.size());
  EXPECT_NE(0, model.indexData.size());
  EXPECT_NE(0, model.normalsData.size());
  EXPECT_NE(0, model.textureCoordsData.size());

  cout << "Vertex data component count: "
  << model.vertexData.size() << endl << "Index count: "
  << model.indexData.size() << endl
  << "Normals data component count: "
  << model.normalsData.size() << endl
  << "Texture coordinates count: "
  << model.textureCoordsData.size() << endl;

  Model modelWithNoTexture;

  loader.load("resources/models/Cube/CubeNoTexture.obj", modelWithNoTexture);

  EXPECT_NE(0, modelWithNoTexture.vertexData.size());
  EXPECT_NE(0, modelWithNoTexture.indexData.size());
  EXPECT_NE(0, modelWithNoTexture.normalsData.size());
  EXPECT_EQ(0, modelWithNoTexture.textureCoordsData.size());

  cout << "Vertex data component count: "
  << modelWithNoTexture.vertexData.size() << endl << "Index count: "
  << modelWithNoTexture.indexData.size() << endl
  << "Normals data component count: "
  << modelWithNoTexture.normalsData.size() << endl
  << "Texture coordinates count: "
  << modelWithNoTexture.textureCoordsData.size() << endl;

}

TEST(BoundingBoxesTest, LoadBoundingBoxes) {

  unique_ptr<BoundingBoxSet> bboxes(new BoundingBoxSet());

  bboxes->loadFromFile("resources/models/GoatBB/GoatBB.obj");

  EXPECT_EQ(16, bboxes->vertices.size());
  EXPECT_EQ(12, bboxes->facesVertexIndexes.size());

  cout << "Bounding boxes vertices: " << endl;
  for (unsigned long idx = 0; idx < 16; idx++) {
    cout << bboxes->vertices[idx][0] << ", " <<
    bboxes->vertices[idx][1] << ", " <<
    bboxes->vertices[idx][2] << ", " << endl;

  }

  cout << "Bounding boxes faces vertex indexes: " << endl;
  for (unsigned long idx = 0; idx < 12; idx++) {
    cout << bboxes->facesVertexIndexes[idx][0] << ", " <<
    bboxes->facesVertexIndexes[idx][1] << ", " <<
    bboxes->facesVertexIndexes[idx][2] << ", " <<
    bboxes->facesVertexIndexes[idx][3] << ", " << endl;

  }

  bboxes->offset.x = 0.0f;
  bboxes->offset.y = 0.1f;
  bboxes->offset.z = 0.1f;
  bboxes->rotation = glm::vec3(0.0f, 0.0f, 0.0f);

  EXPECT_FALSE(bboxes->collidesWith(glm::vec3(0.1f, 0.1f, 0.1f)));

}


//This cannot run on the CI environment because there is no video device available there.

// Cannot run this with MinGW (see comment above Renderer.h include directive)
#ifndef __MINGW32__
TEST(RendererTest, StartAndUse) {

shared_ptr<SceneObject> object(
new SceneObject("animal",
"resources/models/UnspecifiedAnimal/UnspecifiedAnimalWithTexture.obj", 1,
                "resources/models/UnspecifiedAnimal/UnspecifiedAnimalWithTextureRedBlackNumbers.png"));
shared_ptr<vector<shared_ptr<SceneObject> > > scene(
new vector<shared_ptr<SceneObject> >());
scene->push_back(object);

unique_ptr<Renderer> renderer(new Renderer("test", 640, 480));

}
#endif



int main(int argc, char **argv) {
  // Set up a console, if using MinGW
  // This is because the mwindows linker flag,
  // used by blocks referenced by small3d,
  // eliminates the default one.
#ifdef __MINGW32__
  AllocConsole();
  freopen("CONOUT$", "w", stdout);
#endif
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
