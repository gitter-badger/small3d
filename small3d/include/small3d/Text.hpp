/*
 *  Text.hpp
 *
 *  Created on: 2014/11/23
 *      Author: Dimitri Kourkoulis
 *     License: BSD 3-Clause License (see LICENSE file)
 */

#include <string>
#include "Renderer.hpp"
#include <memory>
#include <SDL_ttf.h>

using namespace std;

#pragma once

extern bool ttfInitialised;

namespace small3d {

  string intToStr(int number);

  /**
   * @struct Text
   * @brief Structure for rendering text
   */

  struct Text {
  private:
    shared_ptr<Renderer> renderer;

    TTF_Font *font;
    int size;

  public:

    /**
     * @brief Constructor
     *
     * @param	renderer   	The renderer to be used.
     * @param	ttfFontPath	(optional) The path to the TrueType font to be used by the Renderer,
     * 						including its filename. It defaults to the font provided
     * 						by the engine but, even if the same one is used for an
     * 						game, most probably the path to where it is placed
     * 						will need to be specified here. Only one font can be used
     * 						by the engine at present but that will change in the near
     * 						future.
     * @param	size	   	(optional) the size of the font in the created images. It defaults
     * 						to 48.
     */

    Text(shared_ptr<Renderer> renderer, string ttfFontPath="resources/fonts/CrusoeText/CrusoeText-Regular.ttf",
         int size = 48);

    /**
     * Destructor
     *
     */

    ~Text();

    /**
     * @brief Render some text on the screen. A texture will be generated, containing the given
     * text and it will be rendered at a depth z of 0.5 in an orthographic coordinate space.
     * @param text The text to be rendered
     * @param colour The colour in which the text will be rendered
     * @param topX The top x coordinate of the text rectangle
     * @param topY The top y coordinate of the text rectangle
     * @param bottomX The bottom x coordinate of the text rectangle
     * @param bottomY The bottom y coordinate of the text rectangle
     */
    void renderText(string text, const SDL_Color &colour,
		    float topX, float topY, float bottomX, float bottomY);

    /**
     * @brief	Deletes a texture, generated for text rendering, from the GPU and
     * 			from the renderer's map of textures
     *
     * @param	text	The text for which the texture has been generated.
     */

    void deleteTextTexture(string text);

  };
}
