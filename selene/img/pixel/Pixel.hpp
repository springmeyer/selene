// This file is part of the `Selene` library.
// Copyright 2017-2018 Michael Hofmann (https://github.com/kmhofmann).
// Distributed under MIT license. See accompanying LICENSE file in the top-level directory.

#ifndef SELENE_IMG_PIXEL_PIXEL_HPP
#define SELENE_IMG_PIXEL_PIXEL_HPP

/// @file

#include <selene/base/Assert.hpp>
#include <selene/base/Promote.hpp>
#include <selene/base/Types.hpp>

#include <selene/img/common/PixelFormat.hpp>

#include <array>
#include <cstdint>
#include <type_traits>

namespace sln {

/** \brief Represents a statically typed pixel with arbitrary number of channels.
 *
 * A Pixel<T, n> represents an image element with static type T and n number of samples (channels) of this type per
 * element.
 * A template instance of Pixel<T, n> is guaranteed to be a tightly packed POD type.
 *
 * \tparam T The type for each channel element.
 * \tparam nr_channels_ The number of channels.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_ = PixelFormat::Unknown>
class Pixel
{
public:
  using value_type = T;  ///< The type of each pixel sample (channel).
  static constexpr auto nr_channels = nr_channels_;  ///< The number of channels per pixel.
  static constexpr auto pixel_format = pixel_format_;  ///< The pixel format.

  constexpr Pixel() noexcept = default;  ///< Default constructor. Pixel values are uninitialized.

  template <typename... Args, typename = std::enable_if_t<sizeof...(Args) == nr_channels_>>
  constexpr Pixel(Args... args) noexcept;

  constexpr explicit Pixel(const std::array<T, nr_channels>& arr) noexcept;

  ~Pixel() = default;

  constexpr Pixel(const Pixel<T, nr_channels_, pixel_format_>&) = default;  ///< Copy constructor.
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator=(const Pixel<T, nr_channels_, pixel_format_>&) =
      default;  ///< Copy assignment operator.

  constexpr Pixel(Pixel<T, nr_channels_, pixel_format_>&&) noexcept = default;  ///< Move constructor.
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator=(Pixel<T, nr_channels_, pixel_format_>&&) noexcept =
      default;  ///< Move assignment operator.

  template <typename U>
  constexpr Pixel(const Pixel<U, nr_channels_, pixel_format_>& other);

  template <typename U>
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator=(const Pixel<U, nr_channels_, pixel_format_>& other);

  constexpr T* data() noexcept;
  constexpr const T* data() const noexcept;

  constexpr T& operator[](std::size_t n) noexcept;
  constexpr const T& operator[](std::size_t n) const noexcept;

  // Allow implicit conversion to T, iff nr_channels_ ==1:
  constexpr operator std::conditional_t<nr_channels_ == 1, T, void>() const noexcept;

  constexpr Pixel<T, nr_channels_, pixel_format_>&
    operator+=( const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>&
    operator-=( const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>&
    operator*=( const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>&
    operator/=( const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept;

  constexpr Pixel<T, nr_channels_, pixel_format_>& operator+=(T rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator-=(T rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator*=(T rhs) noexcept;
  constexpr Pixel<T, nr_channels_, pixel_format_>& operator/=(T rhs) noexcept;

  constexpr Pixel<T, nr_channels_, pixel_format_> operator-() noexcept;

private:
  static_assert(std::is_arithmetic<T>::value, "Pixel element type needs to be an arithmetic type");
  static_assert(get_nr_channels(pixel_format_) == nr_channels_
                || pixel_format_ == PixelFormat::Unknown, "Pixel format mismatch");
  std::array<T, nr_channels> data_;
};

template <typename PixelType>
struct ConstifyPixel
{
  using type = const PixelType;
};

template <typename PixelType> using ConstifyPixel_t = typename ConstifyPixel<PixelType>::type;


template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr bool operator==(const Pixel<T, nr_channels_, pixel_format_0>& px0,
                          const Pixel<T, nr_channels_, pixel_format_1>& px1) noexcept;

template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr bool operator!=(const Pixel<T, nr_channels_, pixel_format_0>& px0,
                          const Pixel<T, nr_channels_, pixel_format_1>& px1) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto operator+(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto operator-(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto operator*(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto operator/(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator+(Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator+(T lhs, Pixel<U, nr_channels_, pixel_format_> rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator-(Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator*(Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator*(T lhs, Pixel<U, nr_channels_, pixel_format_> rhs) noexcept;

template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_>
operator/(Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept;

// -----

template <typename PixelType, typename = void>
struct IsPixelType : std::false_type
{ };

template <typename PixelType>
struct IsPixelType<PixelType, std::void_t<typename PixelType::value_type,
                                          decltype(std::declval<PixelType>().nr_channels),
                                          decltype(std::declval<PixelType>().pixel_format)>> : std::true_type
{ };

template <typename PixelType>
inline constexpr bool is_pixel_type_v = IsPixelType<PixelType>::value;

// -----

template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
struct promote<Pixel<T, nr_channels_, pixel_format_>>
{
  using type = Pixel<promote_t<T>, nr_channels_, pixel_format_>;  ///< The promoted type.
};

// ----------
// Implementation:

/** \brief Constructor initializing all pixel channels.
 *
 * This constructor takes exactly Pixel::nr_channels channel values of Pixel::value_type as separate argument inputs.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param args The channel values as separate arguments.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
template <typename... Args, typename>
constexpr Pixel<T, nr_channels_, pixel_format_>::Pixel(Args... args) noexcept
    : data_{{static_cast<T>(args)...}}
{
  static_assert(std::is_trivial<Pixel<T, nr_channels_, pixel_format_>>::value, "Pixel type is not trivial");
  static_assert(std::is_standard_layout<Pixel<T, nr_channels_, pixel_format_>>::value,
                "Pixel type is not standard layout");
  static_assert(sizeof(Pixel<T, nr_channels_, pixel_format_>) == nr_channels_ * sizeof(T),
                "Pixel class is not tightly packed");
}

/** \brief Constructor initializing all pixel channels.
 *
 * This constructor takes a std::array<> with exactly Pixel::nr_channels values of type Pixel::value_type as input.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param arr The channel values as std::array<>.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>::Pixel(const std::array<T, nr_channels>& arr) noexcept
    : data_(arr)
{
  static_assert(std::is_trivial<Pixel<T, nr_channels_, pixel_format_>>::value, "Pixel type is not trivial");
  static_assert(std::is_standard_layout<Pixel<T, nr_channels_, pixel_format_>>::value,
                "Pixel type is not standard layout");
  static_assert(sizeof(Pixel<T, nr_channels_, pixel_format_>) == nr_channels_ * sizeof(T),
                "Pixel class is not tightly packed");
}

/** \brief Copy constructor taking a `Pixel<>` with a different element type.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @tparam U The pixel element type of the argument pixel.
 * @tparam PF The pixel format of the argument pixel.
 * @param other The pixel to copy from.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
template <typename U>
constexpr Pixel<T, nr_channels_, pixel_format_>::Pixel(const Pixel<U, nr_channels_, pixel_format_>& other)
{
  static_assert(std::is_trivial<Pixel<T, nr_channels_>>::value, "Pixel type is not trivial");
  static_assert(std::is_standard_layout<Pixel<T, nr_channels_>>::value, "Pixel type is not standard layout");
  static_assert(sizeof(Pixel<T, nr_channels_>) == nr_channels_ * sizeof(T), "Pixel class is not tightly packed");

  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    this->operator[](i) = static_cast<T>(other[i]);
  }
}

/** \brief Copy assignment operator taking a `Pixel<>` with a different element type.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @tparam U The pixel element type of the argument pixel.
 * @tparam PF The pixel format of the argument pixel.
 * @param other The pixel to copy from.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
template <typename U>
constexpr
Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator=(const Pixel<U, nr_channels_, pixel_format_>& other)
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    this->operator[](i) = static_cast<T>(other[i]);
  }

  return *this;
}

/** \brief Returns a pointer to the first element of the pixel.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \return Pointer to the first pixel element.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr T* Pixel<T, nr_channels_, pixel_format_>::data() noexcept
{
  return data_.data();
}

/** \brief Returns a constant pointer to the first element of the pixel.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \return Constant pointer to the first pixel element.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr const T* Pixel<T, nr_channels_, pixel_format_>::data() const noexcept
{
  return data_.data();
}

/** \brief Provides read-write access to the n-th channel element of the pixel.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param n Index of the channel to access.
 * \return Reference to the n-th channel element.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr T& Pixel<T, nr_channels_, pixel_format_>::operator[](std::size_t n) noexcept
{
  return data_[n];
}

/** \brief Provides read access to the n-th channel element of the pixel.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param n Index of the channel to access.
 * \return Const reference to the n-th channel element.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr const T& Pixel<T, nr_channels_, pixel_format_>::operator[](std::size_t n) const noexcept
{
  return data_[n];
}

/** \brief Allows implicit conversion to Pixel::value_type, iff Pixel::nr_channels == 1.
 *
 * This is useful for usage with any single-channel pixel instance, because the 0-th element will not have to be
 * accessed by operator[] anymore, but the instance decays to type Pixel::value_type.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>::operator std::conditional_t<nr_channels_ == 1, T, void>() const
    noexcept
{
  return data_[0];
}

/** \brief Addition operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The pixel value to add.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator+=(
    const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] += rhs.data_[i];
  }

  return *this;
}

/** \brief Subtraction operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The pixel value to subtract.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator-=(
    const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] -= rhs.data_[i];
  }

  return *this;
}

/** \brief Multiplication operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The pixel value to multiply with.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator*=(
    const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] *= rhs.data_[i];
  }

  return *this;
}

/** \brief Division operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The pixel value to divide by.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator/=(
    const Pixel<T, nr_channels_, pixel_format_>& rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] /= rhs.data_[i];
  }

  return *this;
}

/** \brief Addition operation between the current pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The scalar to add to each pixel value element.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator+=(T rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] += rhs;
  }

  return *this;
}

/** \brief Subtraction operation between the current pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The scalar to subtract from each pixel value element.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator-=(T rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] -= rhs;
  }

  return *this;
}

/** \brief Multiplication operation between the current pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The scalar to multiply each pixel value element with.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator*=(T rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] *= rhs;
  }

  return *this;
}

/** \brief Division operation between the current pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param rhs The scalar to divide each pixel value element by.
 * @return A reference to *this.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_>& Pixel<T, nr_channels_, pixel_format_>::operator/=(T rhs) noexcept
{
  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    data_[i] /= rhs;
  }

  return *this;
}

/** \brief Unary negation operation on the current pixel value. Each element is negated.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @return A pixel value with all elements negated.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<T, nr_channels_, pixel_format_> Pixel<T, nr_channels_, pixel_format_>::operator-() noexcept
{
  Pixel<T, nr_channels_> result{};

  for (std::size_t i = 0; i < nr_channels; ++i)
  {
    result[i] = -data_[i];
  }

  return result;
}

// ----------

/** \brief Equality comparison for two pixels.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param px0 The left-hand side pixel to compare.
 * \param px1 The right-hand side pixel to compare.
 * \return True, if the two pixels have equal values for all channels; false otherwise.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr bool operator==(const Pixel<T, nr_channels_, pixel_format_0>& px0,
                          const Pixel<T, nr_channels_, pixel_format_1>& px1) noexcept
{
  static_assert(pixel_format_0 == pixel_format_1
                || pixel_format_0 == PixelFormat::Unknown
                || pixel_format_1 == PixelFormat::Unknown, "Illegal pixel equality comparison");

  bool equal = true;

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    equal &= (px0[i] == px1[i]);
  }

  return equal;
}

/** \brief Inequality comparison for two pixels.
 *
 * @tparam T The pixel element type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * \param px0 The left-hand side pixel to compare.
 * \param px1 The right-hand side pixel to compare.
 * \return True, if the two pixels are not equal; false, if they are equal.
 */
template <typename T, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr bool operator!=(const Pixel<T, nr_channels_, pixel_format_0>& px0,
                          const Pixel<T, nr_channels_, pixel_format_1>& px1) noexcept
{
  return !(px0 == px1);
}

/** \brief Addition operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side pixel value.
 * @return The operation result. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto
operator+(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept
{
  static_assert(pixel_format_0 == pixel_format_1
                || pixel_format_0 == PixelFormat::Unknown
                || pixel_format_1 == PixelFormat::Unknown, "Pixel addition not allowed: different pixel formats");

  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_1 == PixelFormat::Unknown ? pixel_format_0 : pixel_format_1> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] + rhs[i];
  }

  return result;
}

/** \brief Subtraction operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side pixel value.
 * @return The operation result. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto
operator-(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept
{
  static_assert(pixel_format_0 == pixel_format_1
                || pixel_format_0 == PixelFormat::Unknown
                || pixel_format_1 == PixelFormat::Unknown, "Pixel subtraction not allowed: different pixel formats");

  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_1 == PixelFormat::Unknown ? pixel_format_0 : pixel_format_1> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] - rhs[i];
  }

  return result;
}

/** \brief Multiplication operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side pixel value.
 * @return The operation result. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto
operator*(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept
{
  static_assert(pixel_format_0 == pixel_format_1
                || pixel_format_0 == PixelFormat::Unknown
                || pixel_format_1 == PixelFormat::Unknown, "Pixel multiplication not allowed: different pixel formats");

  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_1 == PixelFormat::Unknown ? pixel_format_0 : pixel_format_1> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] * rhs[i];
  }

  return result;
}

/** \brief Division operation between pixel values, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side pixel value.
 * @return The operation result. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_0, PixelFormat pixel_format_1>
constexpr auto
operator/(Pixel<T, nr_channels_, pixel_format_0> lhs, const Pixel<U, nr_channels_, pixel_format_1>& rhs) noexcept
{
  static_assert(pixel_format_0 == pixel_format_1
                || pixel_format_0 == PixelFormat::Unknown
                || pixel_format_1 == PixelFormat::Unknown, "Pixel division not allowed: different pixel formats");

  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_1 == PixelFormat::Unknown ? pixel_format_0 : pixel_format_1> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] / rhs[i];
  }

  return result;
}

/** \brief Addition operation between a pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The scalar type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side scalar.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator+(
    Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] + rhs;
  }

  return result;
}

/** \brief Addition operation between a scalar and a pixel value, performed element-wise.
 *
 * @tparam T The scalar type.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side scalar.
 * @param rhs The right-hand side pixel value.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator+(
    T lhs, Pixel<U, nr_channels_, pixel_format_> rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs + rhs[i];
  }

  return result;
}

/** \brief Subtraction operation between a pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The scalar type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side scalar.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator-(
    Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] - rhs;
  }

  return result;
}

/** \brief Multiplication operation between a pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The scalar type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side scalar.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator*(
    Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] * rhs;
  }

  return result;
}

/** \brief Multiplication operation between a scalar and a pixel value, performed element-wise.
 *
 * @tparam T The scalar type.
 * @tparam U The pixel element type of the right-hand side pixel value.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side scalar.
 * @param rhs The right-hand side pixel value.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator*(
    T lhs, Pixel<U, nr_channels_, pixel_format_> rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs * rhs[i];
  }

  return result;
}

/** \brief Division operation between a pixel value and a scalar, performed element-wise.
 *
 * @tparam T The pixel element type of the left-hand side pixel value.
 * @tparam U The scalar type.
 * @tparam nr_channels_ The number of channels.
 * @tparam pixel_format_ The pixel format.
 * @param lhs The left-hand side pixel value.
 * @param rhs The right-hand side scalar.
 * @return The resulting pixel value. Note that the element type will be `std::common_type_t<T, U>`.
 */
template <typename T, typename U, std::size_t nr_channels_, PixelFormat pixel_format_>
constexpr Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> operator/(
    Pixel<T, nr_channels_, pixel_format_> lhs, U rhs) noexcept
{
  Pixel<std::common_type_t<T, U>, nr_channels_, pixel_format_> result{};

  for (std::size_t i = 0; i < nr_channels_; ++i)
  {
    result[i] = lhs[i] / rhs;
  }

  return result;
}

}  // namespace sln

#endif  // SELENE_IMG_PIXEL_PIXEL_HPP