// Za pomocą klasy izolujemy sposób poruszania kamerą od samej kamery
// i jak przyjdzie potrzeba będziemy mogli podmienić kontroler na inny np. na FPC (First Person Camera).
#include "camera.h"

using namespace glm;

class CameraControler {
public:
    CameraControler() :camera_(nullptr) {
        LMB_pressed_ = false;
        scale_ = 0.02; // Przelicznik pomiędzy pikselami a kątem.
    }
    CameraControler(Camera* camera) :camera_(camera) {
        LMB_pressed_ = false;
        scale_ = 0.02; // Jego wartość należy dobrać doświadczalnie dla danej rozdzielczości ekranu.
    }
    void set_camera(Camera* camera) { camera_ = camera; }

    // odczytaliśmy już zmianę położenia myszy dx i dy dokonujemy obrotu kamery za pomocą metody
    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, vec3{ 0.0f, 0.0f, 1.0f });
    }

    // obsługa ruchu myszy
    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    // metody obsługi zdarzenia naciśnięcia i puszczenia przycisku myszy
    void LMB_pressed(float x, float y) {
        LMB_pressed_ = true;
        x_ = x;
        y_ = y;
    };

    void LMB_released(float x, float y) {
        LMB_pressed_ = false;
        auto dx = x - x_;
        auto dy = y - y_;

        rotate_camera(dx, dy);
    };

private:
    Camera* camera_;
    float scale_;
    bool LMB_pressed_;

    float x_;
    float y_;
};