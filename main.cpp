#include <iostream>
#include <span>
#include <array>
#include <thread>
#define DISPLAY std::array<std::array<std::pair<char, float>, DISPLAY_SIZE>, DISPLAY_SIZE>

constexpr int DISPLAY_SIZE = 50;
constexpr float INF = 1e6f;
constexpr float PI = 3.14159265358979323846f;

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
                ans[i][j] = get_row(i).dot(other.get_col(j));

        return { ans };
    }

    vec3 operator *(const vec3& other) const {
        vec3 ans {};
        ans.x = get_row(0).dot(other);
        ans.y = get_row(1).dot(other);
        ans.z = get_row(2).dot(other);

        return ans;
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

void clear() {
    std::cout << "\033[2J\033[1;1H";
}

DISPLAY init_display() {
    DISPLAY display {};

    for (int i = 0; i < DISPLAY_SIZE; i++) {
        for (int j = 0; j < DISPLAY_SIZE; j++) {
            display[i][j] = { ' ', -INF };
        }
    }

    return display;
}

int mapToInt(float x) {
    int ix = (int)std::round( (x + 1.0f) * 0.5f * (DISPLAY_SIZE - 1));
    return std::clamp(ix, 0, DISPLAY_SIZE - 1);
}

auto line_equation(int x1, int y1, int x2, int y2) {
    return [=](int x, int y) {
        const int del_x = x2 - x1;
        const int del_y = y2 - y1;

        return del_y * x - del_x * y - (del_y * x1 - del_x * y1);
    };
}

void paint_face(const std::array<vec3, 4>& face, char c, DISPLAY& display) {

    const auto equations = std::array {
        line_equation(mapToInt(face[0].x), mapToInt(face[0].y), mapToInt(face[1].x), mapToInt(face[1].y)),
        line_equation(mapToInt(face[1].x), mapToInt(face[1].y), mapToInt(face[2].x), mapToInt(face[2].y)),
        line_equation(mapToInt(face[2].x), mapToInt(face[2].y), mapToInt(face[3].x), mapToInt(face[3].y)),
        line_equation(mapToInt(face[3].x), mapToInt(face[3].y), mapToInt(face[0].x), mapToInt(face[0].y))
    };

    float height = 0;
    for (const auto& point: face)
        height += point.z;

    height /= 4;

    for (int i = 0; i < DISPLAY_SIZE; i++) {
        for (int j = 0; j < DISPLAY_SIZE; j++) {
            if (display[i][j].second > height)
                continue;

            bool all_pos = true;
            bool all_neg = true;

            for (const auto& eq: equations) {
                if (eq(i, j) <= 0) all_pos = false;
                if (eq(i, j) >= 0) all_neg = false;
            }

            if (all_pos || all_neg)
                display[i][j] = { c, height };
        }
    }
}

void show_display(const DISPLAY& display) {
    std::string s;
    s.reserve(DISPLAY_SIZE * DISPLAY_SIZE + DISPLAY_SIZE);

    for (int i = 0; i < DISPLAY_SIZE; i++) {
        for (int j = 0; j < DISPLAY_SIZE; j++) {
            s.push_back(display[i][j].first);
            s.push_back(display[i][j].first);
        }

        s.push_back('\n');
    }

    std::cout << s;
}

int main() {
    mat3 M = xRotation(PI / 1000.0f) * yRotation(-PI / 600.0f) * zRotation(PI / 1500.0f) * yRotation(PI / 300.0f);


    vec3 A = vec3 { 1, -1, 1 } * 0.5f;
    vec3 B = vec3 { 1, 1, 1 } * 0.5f;
    vec3 C = vec3 { -1, 1, 1 } * 0.5f;
    vec3 D = vec3 { -1, -1, 1 } * 0.5f;

    std::array<vec3, 4> face1 { A, B, C, D };
    std::array<vec3, 4> face2 {
        xRotation(PI / 2) * A,
        xRotation(PI / 2) * B,
        xRotation(PI / 2) * C,
        xRotation(PI / 2) * D
    };
    std::array<vec3, 4> face3 {
        yRotation(PI / 2) * A,
        yRotation(PI / 2) * B,
        yRotation(PI / 2) * C,
        yRotation(PI / 2) * D
    };
    std::array<vec3, 4> face4 {
        xRotation(-PI / 2) * A,
        xRotation(-PI / 2) * B,
        xRotation(-PI / 2) * C,
        xRotation(-PI / 2) * D
    };
    std::array<vec3, 4> face5 {
        yRotation(-PI / 2) * A,
        yRotation(-PI / 2) * B,
        yRotation(-PI / 2) * C,
        yRotation(-PI / 2) * D
    };
    std::array<vec3, 4> face6 {
        yRotation(PI) * A,
        yRotation(PI) * B,
        yRotation(PI) * C,
        yRotation(PI) * D
    };

    std::array faces = { face1, face2, face3, face4, face5, face6 };

    std::array characters { '.', ',', '\"', '`', '\'', '-'};

    for (int i = 0; i < 6; ++i) {
        std::cout << "\n\nPlease enter a character for face " + std::to_string(i) + " (or Q to use default): ";
        std::string c;
        std::cin >> c;

        if (c == "Q" || c == "q")
            break;

        characters[i] = c[0];
    }


    for (;;) {
        DISPLAY display = init_display();

        for (int i = 0; i < 6; i++) {
            faces[i] = { M * faces[i][0], M * faces[i][1], M * faces[i][2], M * faces[i][3] };
            paint_face(faces[i], characters[i], display);
        }

        show_display(display);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        clear();
    }
}