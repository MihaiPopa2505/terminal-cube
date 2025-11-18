#include <iostream>

struct vec3 {
    float x, y, z;

    vec3 operator*(const float& scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator+(const vec3& vector) const {
        return vec3(x + vector.x, y + vector.y, z + vector.z);
    }

    vec3 operator-(const vec3& vector) const {
        return vec3(x - vector.x, y - vector.y, z - vector.z);
    }

    float operator*(const vec3& vector) const {
        return x * vector.x + y * vector.y + z * vector.z;
    }
};

struct mat3 {
    std::array<std::array<float, 3>, 3> mat;

    vec3 get_row(int i) const {
        return vec3{mat[i][0], mat[i][1], mat[i][2]};
    }

    vec3 get_col(int i) const {
        return vec3{mat[0][i], mat[1][i], mat[2][i]};
    }

    mat3 operator *(const mat3& other) const {
        std::array<std::array<float, 3>, 3> ans {};

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ans[i][j] = get_row(i) * other.get_col(j);

        return mat3{ ans };
    }

int main() {

}