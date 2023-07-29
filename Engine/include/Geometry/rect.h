#pragma once

#include "vector2.h"

namespace Reflux::Engine::Geometry {

	template <typename T>
	class rect final {
	public:

		/**
		 * \brief Creates an rect.
		 *
		 * \param min_ the lower bounds of the rect.
		 * \param max_ the upper bounds of the rect.
		 *
		 * \throws invalid_argument if `min_` is greater than `max_`.
		 *
		 * \since 0.1.0
		 */
		constexpr rect(const vector2<T>& min, const vector2<T>& max) : min_{ min }, max_{ max } {
			if ((min_.x > max_.x) || (min_.y > max_.y)) {
				throw std::invalid_argument("rect: min_ > max_");
			}
		}

		[[nodiscard]] constexpr auto contains(const rect& other, bool countTouch = true) const noexcept
			-> bool {
			if (countTouch) {
				other.max.x <= max_.x && other.max.y <= max_.y && other.min.x >= min_.x && other.min.y >= min_.y;
			} else {
				other.max.x < max_.x&& other.max.y < max_.y&& other.min.x > min_.x&& other.min.y > min_.y;
			}
		}

		[[nodiscard]] constexpr auto contains(const vector2<T>& point, bool countTouch = true) const noexcept
			-> bool {
			if (countTouch) {
				return point.x <= max_.x && point.y <= max_.y && point.x >= min_.x && point.y >= min_.y;
			} else {
				point.x < max_.x&& point.y < max_.y&& point.x > min_.x&& point.y > min_.y;
			}
		}

		[[nodiscard]] constexpr auto overlaps(const rect& other,
			bool countTouch = true) const noexcept
			-> bool {
			if (countTouch) {
				return !(other.max_.x < min_.x || other.max_.y < min_.y || other.min_.x > max_.x || other.min_.y > max_.y);
			} else {
				return !(other.max_.x <= min_.x || other.max_.y <= min_.y || other.min_.x >= max_.x || other.min_.y >= max_.y);
			}

			return true;
		}

		[[nodiscard]] constexpr auto size() const noexcept -> vector2<T> {
			return max_ - min_;
		}

		[[nodiscard]] constexpr auto min() const noexcept -> const vector2<T>& {
			return min_;
		}

		[[nodiscard]] constexpr auto max() const noexcept -> const vector2<T>& {
			return max_;
		}

	private:
		vector2<T> min_;
		vector2<T> max_;
	};

	template <typename T>
	[[nodiscard]] constexpr auto operator+(const rect<T>& lhs,
		const vector2<T>& rhs) noexcept
		-> rect<T> {
		return { lhs.min() + rhs, lhs.max() + rhs };
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator-(const rect<T>& lhs,
		const vector2<T>& rhs) noexcept
		-> rect<T> {
		return { lhs.min() - rhs, lhs.max() - rhs };
	}

	template <typename T>
	[[nodiscard]] constexpr auto operator+(const rect<T>& lhs,
		const rect<T>& rhs) noexcept
		-> rect<T> {

		return rect<T>{ { std::min(lhs.min().x, rhs.min().x), std::min(lhs.min().y, rhs.min().y) }, { std::max(lhs.max().x, rhs.max().x), std::max(lhs.max().y, rhs.max().y) } };
	}

}
