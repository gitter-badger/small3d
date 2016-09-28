/*
 *  BoundingBoxSet.cpp
 *
 *  Created on: 2014/10/19
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include "BoundingBoxSet.hpp"
#include <fstream>
#include "Exception.hpp"
#include "GetTokens.hpp"
#include "MathFunctions.hpp"
#include "SDL.h"


namespace small3d {

  /**
   * Constructor
   */
  BoundingBoxSet::BoundingBoxSet() {
    initLogger();
    vertices.clear();
    facesVertexIndexes.clear();
    numBoxes = 0;
  }

  void BoundingBoxSet::loadFromFile(string fileLocation) {
    if (vertices.size() != 0) {
      throw Exception(
          "Illegal attempt to reload bounding boxes. Please use another object.");
    }
    ifstream file((SDL_GetBasePath() + fileLocation).c_str());
    string line;
    if (file.is_open()) {
      while (getline(file, line)) {
        if (line[0] == 'v' || line[0] == 'f') {
          string *tokens = new string[5]; // Max 5 such tokens in the specific kind of
          // Wavefront file

          int numTokens = getTokens(line, ' ', tokens);

          int idx = 0;

          if (line[0] == 'v') {
            // get vertex
            vector<float> v;

            for (int tokenIdx = 0; tokenIdx < numTokens; ++tokenIdx) {
              string t = tokens[tokenIdx];
              if (idx > 0)   // The first token is the vertex indicator
              {
                v.push_back(static_cast<float>(atof(t.c_str())));
              }
              ++idx;
            }
            vertices.push_back(v);
          }
          else {
            // get vertex index
            vector<int> v;

            for (int tokenIdx = 0; tokenIdx < numTokens; ++tokenIdx) {
              string t = tokens[tokenIdx];
              if (idx > 0)   // The first token is face indicator
              {
                v.push_back(atoi(t.c_str()));
              }
              ++idx;
            }
            facesVertexIndexes.push_back(v);

          }

          if (tokens != NULL) {
            delete[] tokens;
          }
        }
      }
      file.close();
      numBoxes = (int) (facesVertexIndexes.size() / 6);
    }
    else
      throw Exception(
          "Could not open file " + string(SDL_GetBasePath())
          + fileLocation);

  }

  bool BoundingBoxSet::collidesWith(glm::vec3 point) const {
    bool collides = false;
    glm::mat4 rotationMatrix = rotateZ(rotation.z) * rotateX(rotation.x) * rotateY(rotation.y);

    for (int idx = 0; idx < numBoxes; ++idx) {
      float minZ, maxZ, minX, maxX, minY, maxY;

      glm::vec4 coords(vertices[static_cast<unsigned int>(idx * 8)][0], vertices[static_cast<unsigned int>(idx * 8)][1],
        vertices[static_cast<unsigned int>(idx * 8)][2], 1);
      glm::vec4 rotatedCoords(0.0f, 0.0f, 0.0f, 1.0f);

      rotatedCoords = rotationMatrix * coords;

      rotatedCoords.x += offset.x;
      rotatedCoords.y += offset.y;
      rotatedCoords.z += offset.z;

      minX = rotatedCoords.x;
      maxX = rotatedCoords.x;
      minY = rotatedCoords.y;
      maxY = rotatedCoords.y;
      minZ = rotatedCoords.z;
      maxZ = rotatedCoords.z;

      for (int checkidx = idx * 8; checkidx < (idx + 1) * 8; ++checkidx) {
        coords = glm::vec4(vertices[static_cast<unsigned int>(checkidx)][0], vertices[static_cast<unsigned int>(checkidx)][1],
          vertices[static_cast<unsigned int>(checkidx)][2], 1);
        rotatedCoords = rotationMatrix * coords;

        rotatedCoords.x += offset.x;
        rotatedCoords.y += offset.y;
        rotatedCoords.z += offset.z;

        if (rotatedCoords.x < minX)
          minX = rotatedCoords.x;
        if (rotatedCoords.x > maxX)
          maxX = rotatedCoords.x;
        if (rotatedCoords.y < minY)
          minY = rotatedCoords.y;
        if (rotatedCoords.y > maxY)
          maxY = rotatedCoords.y;
        if (rotatedCoords.z < minZ)
          minZ = rotatedCoords.z;
        if (rotatedCoords.z > maxZ)
          maxZ = rotatedCoords.z;
      }

      if (point.x > minX && point.x < maxX &&
          point.y > minY && point.y < maxY &&
          point.z > minZ && point.z < maxZ) {

        collides = true;
        break;
      }
    }

    return collides;
  }

  bool BoundingBoxSet::collidesWith(BoundingBoxSet &otherBoxSet) const {
    bool collides = false;

    glm::mat4 rotationMatrix =
        rotateZ(otherBoxSet.rotation.z) * rotateX(otherBoxSet.rotation.x) * rotateY(otherBoxSet.rotation.y);

    for (vector<vector<float> >::iterator vertex = otherBoxSet.vertices.begin();
         vertex != otherBoxSet.vertices.end(); ++vertex) {

      glm::vec4 otherCoords(vertex->at(0), vertex->at(1), vertex->at(2), 1.0f);
      glm::vec4 rotatedOtherCoords(0.0f, 0.0f, 0.0f, 1.0f);

      rotatedOtherCoords = rotationMatrix * otherCoords;

      rotatedOtherCoords.x += otherBoxSet.offset.x;
      rotatedOtherCoords.y += otherBoxSet.offset.y;
      rotatedOtherCoords.z += otherBoxSet.offset.z;

      /*cout << "Checking " << rotatedOtherCoords.x << ", " << rotatedOtherCoords.y << ", " << rotatedOtherCoords.z <<
    " with " << boxesX << ", " << boxesY << ", " << boxesZ << " rotation " << boxesRotation << endl;*/
      if (collidesWith(glm::vec3(rotatedOtherCoords.x, rotatedOtherCoords.y, rotatedOtherCoords.z))) {

        collides = true;
        break;
      }
    }

    return collides;
  }

}
