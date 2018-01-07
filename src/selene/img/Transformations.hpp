// This file is part of the `Selene` library.
// Copyright 2017 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_TRANSFORMATIONS_HPP
#define SELENE_IMG_TRANSFORMATIONS_HPP

/// @file

#include <selene/base/Assert.hpp>

#include <selene/img/Image.hpp>
#include <selene/img/PixelTraits.hpp>

#include <algorithm>
#include <utility>

namespace sln {

/** \brief Describes the flip direction. */
enum class FlipDirection
{
  Horizontal,  ///< Horizontal flip.
  Vertical,  ///< Vertical flip.
  Both,  ///< Combined horizontal and vertical flip.
};

/** \brief Describes the rotation direction. */
enum class RotationDirection
{
  Clockwise0,  ///< Rotation by 0 degrees clockwise.
  Clockwise90,  ///< Rotation by 90 degrees clockwise.
  Clockwise180,  ///< Rotation by 180 degrees clockwise.
  Clockwise270,  ///< Rotation by 270 degrees clockwise.
  Counterclockwise0,  ///< Rotation by 0 degrees counterclockwise.
  Counterclockwise90,  ///< Rotation by 90 degrees counterclockwise.
  Counterclockwise180,  ///< Rotation by 180 degrees counterclockwise.
  Counterclockwise270,  ///< Rotation by 270 degrees counterclockwise.
};

template <FlipDirection flip_dir, typename PixelType>
void flip(const Image<PixelType>& img_src, Image<PixelType>& img_dst);

template <FlipDirection flip_dir, typename PixelType>
Image<PixelType> flip(const Image<PixelType>& img);

template <typename PixelType>
void flip_horizontally_in_place(Image<PixelType>& img);

template <typename PixelType>
void flip_vertically_in_place(Image<PixelType>& img);

template <bool flip_h = false, bool flip_v = false, typename PixelType>
void transpose(const Image<PixelType>& img_src, Image<PixelType>& img_dst);

template <bool flip_h = false, bool flip_v = false, typename PixelType>
Image<PixelType> transpose(const Image<PixelType>& img);

template <RotationDirection rot_dir, typename PixelType>
void rotate(const Image<PixelType>& img_src, Image<PixelType>& img_dst);

template <RotationDirection rot_dir, typename PixelType>
Image<PixelType> rotate(const Image<PixelType>& img);

// ----------
// Implementation:

/** \brief Flips the image contents according to the specified flip direction.
 *
 * @tparam flip_dir The flip direction. Must be provided
 * @tparam PixelType The pixel type.
 * @param img_src The source image.
 * @param[out] img_dst The flipped output image.
 */
template <FlipDirection flip_dir, typename PixelType>
void flip(const Image<PixelType>& img_src, Image<PixelType>& img_dst)
{
  SELENE_ASSERT(&img_src != &img_dst);
  img_dst.maybe_allocate(img_src.width(), img_src.height());

  switch (flip_dir)
  {
    case FlipDirection::Horizontal:
    {
      for (auto y_src = 0_px; y_src < img_src.height(); ++y_src)
      {
        std::reverse_copy(img_src.data(y_src), img_src.data_row_end(y_src), img_dst.data(y_src));
      }
      break;
    }
    case FlipDirection::Vertical:
    {
      for (auto y_src = 0_px; y_src < img_src.height(); ++y_src)
      {
        const auto y_dst = PixelIndex{img_src.height() - y_src - 1};
        std::copy(img_src.data(y_src), img_src.data_row_end(y_src), img_dst.data(y_dst));
      }
      break;
    }
    case FlipDirection::Both:
    {
      for (auto y_src = 0_px; y_src < img_src.height(); ++y_src)
      {
        const auto y_dst = PixelIndex{img_src.height() - y_src - 1};
        std::reverse_copy(img_src.data(y_src), img_src.data_row_end(y_src), img_dst.data(y_dst));
      }
      break;
    }
  }
}

/** \brief Flips the image contents according to the specified flip direction.
 *
 * @tparam flip_dir The flip direction. Must be provided
 * @tparam PixelType The pixel type.
 * @param img The source image.
 * @return The flipped output image.
 */
template <FlipDirection flip_dir, typename PixelType>
Image<PixelType> flip(const Image<PixelType>& img)
{
  Image<PixelType> img_flip;
  flip<flip_dir>(img, img_flip);
  return img_flip;
}

/** \brief Flips the image horizontally, in-place.
 *
 * @tparam PixelType The pixel type.
 * @param img The image to be flipped horizontally.
 */
template <typename PixelType>
void flip_horizontally_in_place(Image<PixelType>& img)
{
  const auto half_width = img.width() / 2;

  for (auto y = 0_px; y < img.height(); ++y)
  {
    auto row_ptr = img.data(y);

    for (auto x_left = 0_px; x_left < half_width; ++x_left)
    {
      const auto x_right = img.width() - x_left - 1;
      std::swap(row_ptr[x_left], row_ptr[x_right]);
    }
  }
}

/** \brief Flips the image vertically, in-place.
 *
 * @tparam PixelType The pixel type.
 * @param img The image to be flipped vertically.
 */
template <typename PixelType>
void flip_vertically_in_place(Image<PixelType>& img)
{
  const auto half_height = img.height() / 2;

  for (auto y_top = 0_px; y_top < half_height; ++y_top)
  {
    const auto y_bottom = PixelIndex{img.height() - y_top - 1};

    const auto x_top_end = img.data_row_end(y_top);
    auto x_top = img.data(y_top);
    auto x_bottom = img.data(y_bottom);

    for (; x_top != x_top_end; ++x_top, ++x_bottom)
    {
      std::swap(*x_top, *x_bottom);
    }
  }

  // TODO: measure
#if 0
  std::vector<PixelType> buffer(img.width());  // TODO: could be stack-based in some cases?

  for (auto y_top = 0_px; y_top < half_height; ++y_top)
  {
    const auto y_bottom = PixelIndex{img.height() - 1 - y_top};
    std::copy(img.data(y_top), img.data_row_end(y_top), buffer.begin());
    std::copy(img.data(y_bottom), img.data_row_end(y_bottom), img.data(y_top));
    std::copy(std::cbegin(buffer), std::cend(buffer), img.data(y_bottom));
  }
#endif
}

/** \brief Transposes the image.
 *
 * The output image will have transposed extents, i.e. output width will be input height, and output height will be
 * input width.
 *
 * @tparam flip_h If true, the output will additionally be horizontally flipped.
 * @tparam flip_v If true, the output will additionally be vertically flipped.
 * @tparam PixelType The pixel type.
 * @param img_src The source image.
 * @param[out] img_dst The transposed output image.
 */
template <bool flip_h, bool flip_v, typename PixelType>
void transpose(const Image<PixelType>& img_src, Image<PixelType>& img_dst)
{
  SELENE_ASSERT(&img_src != &img_dst);
  img_dst.maybe_allocate(img_src.height(), img_src.width());

  for (auto dst_y = 0_px; dst_y < img_dst.height(); ++dst_y)
  {
    for (auto dst_x = 0_px; dst_x < img_dst.width(); ++dst_x)
    {
      const auto src_x = flip_v ? PixelIndex{img_src.width() - 1 - dst_y} : dst_y;  // branch determined at
      const auto src_y = flip_h ? PixelIndex{img_src.height() - 1 - dst_x} : dst_x;  // compile time
      img_dst(dst_x, dst_y) = img_src(src_x, src_y);
    }
  }
}

/** \brief Transposes the image.
 *
 * The output image will have transposed extents, i.e. output width will be input height, and output height will be
 * input width.
 *
 * @tparam flip_h If true, the output will additionally be horizontally flipped.
 * @tparam flip_v If true, the output will additionally be vertically flipped.
 * @tparam PixelType The pixel type.
 * @param img The source image.
 * @return The transposed output image.
 */
template <bool flip_h, bool flip_v, typename PixelType>
Image<PixelType> transpose(const Image<PixelType>& img)
{
  Image<PixelType> img_t;
  transpose(img, img_t);
  return img_t;
}

/** \brief Rotates the image (in 90 degree increments) by the specified amount and direction.
 *
 * @tparam rot_dir The rotation amount and direction. Must be provided.
 * @tparam PixelType The pixel type.
 * @param img_src The source image.
 * @param[out] img_dst The rotated output image.
 */
template <RotationDirection rot_dir, typename PixelType>
void rotate(const Image<PixelType>& img_src, Image<PixelType>& img_dst)
{
  SELENE_ASSERT(&img_src != &img_dst);

  switch (rot_dir)  // known at compile time
  {
    case RotationDirection::Clockwise0:
    case RotationDirection::Counterclockwise0:
    {
      clone(img_src, img_dst);
      break;
    }
    case RotationDirection::Clockwise90:
    case RotationDirection::Counterclockwise270:
    {
      transpose<true, false>(img_src, img_dst);
      break;
    }
    case RotationDirection::Clockwise180:
    case RotationDirection::Counterclockwise180:
    {
      flip<FlipDirection::Both>(img_src, img_dst);
      break;
    }
    case RotationDirection::Clockwise270:
    case RotationDirection::Counterclockwise90:
    {
      transpose<false, true>(img_src, img_dst);
      break;
    }
  }
}

/** \brief Rotates the image (in 90 degree increments) by the specified amount and direction.
 *
 * @tparam rot_dir The rotation amount and direction. Must be provided.
 * @tparam PixelType The pixel type.
 * @param img The source image.
 * @return The rotated output image.
 */
template <RotationDirection rot_dir, typename PixelType>
Image<PixelType> rotate(const Image<PixelType>& img)
{
  Image<PixelType> img_r;
  rotate<rot_dir>(img, img_r);
  return img_r;
}

}  // namespace sln

#endif  // SELENE_IMG_TRANSFORMATIONS_HPP
