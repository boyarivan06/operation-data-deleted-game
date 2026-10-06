#include <raylib.h>
#include <vector>
#include <memory>
#include "view.h"
#include "game_object.h"
#include "colors.h"
#include "creature.h"
#include "../service/config.h"
#include "../ext/vector2_operations.h"

View::View() = default;

View::~View() {
    for (const auto& texture : textures) {
        if (texture.id != 0) {
            UnloadTexture(texture);
        }
    }
    UnloadFont(font);
    if (IsWindowReady()) {
        CloseWindow();
    }
}

void View::draw_view(const std::vector<std::shared_ptr<GameObject>>& objects) const {
    BeginDrawing();
    ClearBackground(CUSTOM_BLACK);

    // Отображаем все объекты
    for (const auto& obj : objects) {
        if (obj) {
            draw_object(*obj);
        }
    }

    EndDrawing();
}

void View::init_window(const std::string& title) {
    // Поменяйте местами для правильной ориентации
    InitWindow(screen_width, screen_height, title.c_str());
    SetTargetFPS(60);

    // Загружаем текстуры
    for (const auto& res : resources) {
        Texture2D texture = LoadTexture(res.c_str());
        if (texture.id != 0) {
            textures.push_back(texture);
        }
    }

    // Инициализация шрифта
    init_font();
}

void View::init_font() {
    std::vector<int> codepoints;

    // ASCII символы
    for (int i = 32; i <= 126; i++) {
        codepoints.push_back(i);
    }

    // Русские буквы
    for (int i = 0x0410; i <= 0x042F; i++) {
        codepoints.push_back(i);
    }
    for (int i = 0x0430; i <= 0x044F; i++) {
        codepoints.push_back(i);
    }

    // Ё и ё
    codepoints.push_back(0x0401);
    codepoints.push_back(0x0451);

    // Уберите жестко закодированный путь или сделайте его конфигурируемым
    // Вместо этого используйте относительный путь:
    const char* font_path = "/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/fonts/OperationNapalm-Regular.otf"; // или из конфига

    font = LoadFontEx(font_path, 32, codepoints.data(), codepoints.size());
    if (!IsFontValid(font)) {
        font = GetFontDefault();
    }
}

void View::draw_object(GameObject& game_object) const {
    Vector2 screen_position = CELL_SIZE * (game_object.get_position()+game_object.get_displacement());
    int resource_id = game_object.get_resource();

    if (resource_id == -1) {
        Vector2 size = game_object.get_size() * CELL_SIZE;
        DrawRectangleV(screen_position, size, game_object.get_color());

        if (!game_object.get_name().empty()) {
            DrawTextEx(font,
                      game_object.get_name().c_str(),
                      {screen_position.x + 5, screen_position.y + 5},
                      CELL_SIZE / 2.0f,
                      1,
                      WHITE);
        }
    } else if (resource_id >= 0 && resource_id < static_cast<int>(textures.size())) {
        Texture2D texture = textures[resource_id];
        if (texture.id != 0) {
            Rectangle source = {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
            Rectangle dest = {screen_position.x, screen_position.y, CELL_SIZE, CELL_SIZE};
            DrawTexturePro(texture, source, dest, {0, 0}, 0.0f, WHITE);
        }
    }
    //DrawText(std::format("{},{}", game_object.get_position().x, game_object.get_position().y).c_str(), (game_object.get_position().x+game_object.get_displacement().x)*CELL_SIZE, (game_object.get_position().y+game_object.get_displacement().y+0.2)*CELL_SIZE, 10, RED);
    if (const auto* creature = dynamic_cast<const Creature*>(&game_object)) {
        if (creature->get_max_health() > 0) {
            DrawRectangle((game_object.get_position().x+game_object.get_displacement().x)*CELL_SIZE, (game_object.get_position().y+game_object.get_displacement().y)*CELL_SIZE, CELL_SIZE, CELL_SIZE/10, RED);
            DrawRectangle((game_object.get_position().x+game_object.get_displacement().x)*CELL_SIZE, (game_object.get_position().y+game_object.get_displacement().y)*CELL_SIZE, CELL_SIZE * (static_cast<float>(creature->get_health())/static_cast<float>(creature->get_max_health())), CELL_SIZE/10, GREEN);
        }
    }

}

void View::set_window_size(const std::pair<int, int> &new_size_cells) {
    SetWindowSize((new_size_cells.first + FIELD_X) * CELL_SIZE, (new_size_cells.second + FIELD_Y) * CELL_SIZE);
    screen_height = (new_size_cells.first + FIELD_X) * CELL_SIZE;
    screen_width = (new_size_cells.second + FIELD_Y) * CELL_SIZE;
}

void View::set_window_title(const std::string& title) {
    SetWindowTitle(title.c_str());
}