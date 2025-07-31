module;

#include <string>
#include <format>
#include <vector>

export module mesh : mesh;

namespace tire {

export template <typename FloatType = double, typename IntType = unsigned int>
struct Mesh final {
    using float_type = FloatType;
    using int_type = IntType;

    std::vector<FloatType> vertecies_{};
    std::vector<IntType> indecies_{};
    std::vector<FloatType> texcrds_{};
    std::vector<FloatType> normals_{};
    std::vector<FloatType> colors_{};
};

}  // namespace tire
