#pragma once

#include "vector2.h"

namespace Reflux::Engine::Geometry {

	template<typename TSrc, typename TDest>
	struct transform final {
		vector2<float> offset;
		vector2<float> scale;

		transform(vector2<float> offset, vector2<float> scale): offset(offset), scale(scale) {}

		[[nodiscard]] constexpr TDest transform_position(TSrc position) const {
			return {
				(position.x + offset.x) * scale.x,
				(position.y + offset.y) * scale.y,
			};
		}

		[[nodiscard]] constexpr TDest transform_vector(TSrc vector) const {
			return {
				vector.x * scale.x,
				vector.y * scale.y,
			};
		}

		[[nodiscard]] constexpr TSrc inv_transform_position(TDest position) const {
			return {
				static_cast<float>(position.x) / scale.x - offset.x,
				static_cast<float>(position.y) / scale.y - offset.y,
			};
		}

		[[nodiscard]] constexpr TSrc inv_transform_vector(TDest vector) const {
			return {
				static_cast<float>(vector.x) / scale.x,
				static_cast<float>(vector.y) / scale.y,
			};
		}

	};

}