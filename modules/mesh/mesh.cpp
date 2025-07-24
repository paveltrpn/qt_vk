module;

#include <string>
#include <format>
#include <vector>

export module mesh : mesh;

namespace tire {

export template <typename FloatType = double>
struct Mesh final {
    std::vector<FloatType> vertecies_{};
    std::vector<unsigned int> indecies_{};
    std::vector<FloatType> texcrds_{};
    std::vector<FloatType> normals_{};

    using float_type = FloatType;
    using int_type = decltype( indecies_ )::value_type;
};

}  // namespace tire
