#include <iostream>

struct vec3 {
    float x, y, z;

    vec3 operator*(float scalar) const {
        return vec3 {x * scalar, y * scalar, z * scalar };
    }

    float dot(const vec3& b) const {
        return x * b.x + y * b.y + z * b.z;
    }

    vec3 operator+(const vec3& vector) const {
        return { x + vector.x, y + vector.y, z + vector.z };
    }

    vec3 operator-(const vec3& vector) const {
        return { x - vector.x, y - vector.y, z - vector.z };
    }
};

vec3 operator*(float scalar, const vec3& v) {
    return v * scalar;
}

struct mat3 {
    std::array<std::array<float, 3>, 3> mat;

    vec3 get_row(int i) const {
        return vec3 {mat[i][0], mat[i][1], mat[i][2]};
    }

    vec3 get_col(int i) const {
        return vec3 {mat[0][i], mat[1][i], mat[2][i]};
    }

    mat3 operator *(const mat3& other) const {
        std::array<std::array<float, 3>, 3> ans {};

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ans[i][j] = get_row(i).dot(get_col(j));

        return { ans };
    }
};

mat3 xRotation(float angle) {
    const std::array mat {
        std::array{ 1.0f,            0.0f,             0.0f },
        std::array{ 0.0f, std::cos(angle), -std::sin(angle) },
        std::array{ 0.0f, std::sin(angle),  std::cos(angle) }
    };

    return mat3 { mat };
}

mat3 yRotation(float angle) {
    const std::array mat {
        std::array{  std::cos(angle), 0.0f, std::sin(angle) },
        std::array{  0.0f,            1.0f,           0.0f  },
        std::array{ -std::sin(angle), 0.0f, std::cos(angle) }
    };

    return mat3 { mat };
}

mat3 zRotation(float angle) {
    const std::array mat {
        std::array{ std::cos(angle), -std::sin(angle), 0.0f },
        std::array{ std::sin(angle),  std::cos(angle), 0.0f },
        std::array{ 0.0f,             0.0f,            1.0f }
    };

    return mat3 { mat };
}

int main() {
    return 0;
}