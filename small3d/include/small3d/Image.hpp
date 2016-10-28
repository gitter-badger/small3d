/*
 *  Image.hpp
 *
 *  Created on: 2014/10/18
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Logger.hpp"
#include <png.h>

namespace small3d {

  /**
   * @class	Image
   *
   * @brief	Image loading class. Only handles RGB encoded data in PNG files.
   *
   */

  class Image {
  private:

    int width, height;
    std::vector<float> imageData;
    unsigned int imageDataSize;

    void loadFromFile(const std::string &fileLocation);

  public:

    /**
     * @brief Default constructor
     * @param fileLocation Location of image file
     */
    Image(std::string fileLocation = "");

    /**
     * @brief Destructor
     */
    ~Image() = default;

    /**
     * @brief Get the image width
     * @return The image width
     */
    const int getWidth() const;

    /**
     * @brief Get the image height
     * @return The image height
     */
    const int getHeight() const;

    /**
     * @brief Get the size of the image, in bytes
     * @return Size of the image, in bytes
     */
    const unsigned int size() const;

    /**
     * @brief Get the image data
     * @return The image data
     */
    const float* getData() const;

  };

}
