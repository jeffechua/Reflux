/*
 * FROM https://github.com/albin-johansson/abby/blob/main/include/abby.hpp
 * LICENSE OF SOURCE BELOW
 *
 *
 * MIT License
 *
 * Copyright (c) 2019-2020 Albin Johansson: adapted and improved source code
 * from the AABBCC library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This codebase was mainly based on the AABBCC library, written by Lester
 * Hedges, which uses the Zlib license: https://github.com/lohedges/aabbcc.
 * Furthermore, the AABB tree implementation in the Simple Voxel Engine project
 * also influenced this library, which uses the MIT license:
 * https://github.com/JamesRandall/SimpleVoxelEngine.
 */

#pragma once

#include <stdexcept>        // invalid_argument

namespace Reflux::Engine::Geometry {

	/**
	 * \struct vector2
	 *
	 * \brief A simple two-dimensional vector.
	 *
	 * \tparam T the representation type.
	 *
	 * \since 0.1.0
	 *
	 * \headerfile vector2.hpp
	 */
	template <typename T>
	struct vector2 final {
		T x{};  ///< The x-coordinate.
		T y{};  ///< The y-coordinate.

		auto operator[](std::size_t index) -> T& {
			if (index == 0) {
				return x;
			} else if (index == 1) {
				return y;
			} else {
				throw std::invalid_argument{ "vector2: bad subscript index!" };
			}
		}

		auto operator[](std::size_t index) const -> const T& {
			if (index == 0) {
				return x;
			} else if (index == 1) {
				return y;
			} else {
				throw std::invalid_argument{ "vector2: bad subscript index!" };
			}
		}

		// CUSTOM ADDITIONs
		template<typename TOut>
		[[nodiscard]] constexpr operator vector2<TOut>() const {
			return { static_cast<TOut>(x), static_cast<TOut>(y) };
		}

		template <typename T>
		vector2<T>& operator+=(const vector2<T>& other) noexcept {
			x += other.x;
			y += other.y;
			return *this;
		}

		template <typename T>
		vector2<T>& operator-=(const vector2<T>& other) noexcept {
			x -= other.x;
			y -= other.y;
			return *this;
		}

		template <typename T>
		vector2<T>& operator*= (const T other) noexcept {
			x *= other;
			y *= other;
			return *this;
		}

		template <typename T>
		vector2<T>& operator/= (const T other) noexcept {
			x /= other;
			y /= other;
			return *this;
		}

	};

	// clang-format off
	template <typename T> vector2(T, T) -> vector2<T>;
	// clang-format on

	/**
	 * \brief Adds two vectors and returns the result.
	 *
	 * \tparam T the representation type used by the vectors.
	 *
	 * \param lhs the left-hand side vector.
	 * \param rhs the right-hand side vector.
	 *
	 * \return a vector that is the result of adding the components of the two
	 * vectors.
	 *
	 * \since 0.1.0
	 */
	template <typename T>
	[[nodiscard]] constexpr auto operator+(const vector2<T>& lhs,
		const vector2<T>& rhs) noexcept
		-> vector2<T> {
		return { lhs.x + rhs.x, lhs.y + rhs.y };
	}

	/**
	 * \brief Subtracts two vectors and returns the result.
	 *
	 * \tparam T the representation type used by the vectors.
	 *
	 * \param lhs the left-hand side vector.
	 * \param rhs the right-hand side vector.
	 *
	 * \return a vector that is the result of subtracting the components of the two
	 * vectors.
	 *
	 * \since 0.1.0
	 */
	template <typename T>
	[[nodiscard]] constexpr auto operator-(const vector2<T>& lhs,
		const vector2<T>& rhs) noexcept
		-> vector2<T> {
		return { lhs.x - rhs.x, lhs.y - rhs.y };
	}

	/**
	 * \brief Indicates whether or not two vectors are equal.
	 *
	 * \tparam T the representation type used by the vectors.
	 *
	 * \param lhs the left-hand side vector.
	 * \param rhs the right-hand side vector.
	 *
	 * \return `true` if the two vectors are equal; `false` otherwise.
	 *
	 * \since 0.1.0
	 */
	template <typename T>
	[[nodiscard]] constexpr auto operator==(const vector2<T>& lhs,
		const vector2<T>& rhs) noexcept -> bool {
		return (lhs.x == rhs.x) && (lhs.y == rhs.y);
	}

	/**
	 * \brief Indicates whether or not two vectors aren't equal.
	 *
	 * \tparam T the representation type used by the vectors.
	 *
	 * \param lhs the left-hand side vector.
	 * \param rhs the right-hand side vector.
	 *
	 * \return `true` if the two vectors aren't equal; `false` otherwise.
	 *
	 * \since 0.1.0
	 */
	template <typename T>
	[[nodiscard]] constexpr auto operator!=(const vector2<T>& lhs,
		const vector2<T>& rhs) noexcept -> bool {
		return !(lhs == rhs);
	}

	// CUSTOM ADDITIONS

	template <typename T>
	[[nodiscard]] constexpr vector2<T> operator*(const T& factor,
		const vector2<T> vector) noexcept {
		return { vector.x * factor, vector.y * factor };
	}

	template <typename T>
	[[nodiscard]] constexpr vector2<T> operator*(const vector2<T> vector, const T& factor) noexcept {
		return factor * vector;
	}

	template <typename T>
	[[nodiscard]] constexpr vector2<T> operator/(const vector2<T> vector, const T& factor) noexcept {
		return { vector.x / factor, vector.y / factor };
	}

}