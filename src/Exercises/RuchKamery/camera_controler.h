// Za pomoc¹ klasy izolujemy sposób poruszania kamer¹ od samej kamery
// i jak przyjdzie potrzeba bêdziemy mogli podmieniæ kontroler na inny np. na FPC (First Person Camera).
class CameraControler {
public:
    CameraControler() :camera_(nullptr) {}
    CameraControler(Camera* camera) :camera_(camera) {}
    void set_camera(Camera* camera) { camera_ = camera; }

    // odczytaliœmy ju¿ zmianê po³o¿enia myszy dx i dy dokonujemy obrotu kamery za pomoc¹ metody
    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    // obs³uga ruchu myszy
    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    // metody obs³ugi zdarzenia naciœniêcia i puszczenia przycisku myszy
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

    // Przelicznik pomiêdzy pikselami a k¹tem. 
    // Jego wartoœæ nale¿y dobraæ doœwiadczalnie dla danej rozdzielczoœci ekranu.
    float scale_ = 1;

    // chcemy, aby obracanie dzia³a³o tylko jak ruszamy mysz¹ trzymaj¹c wciœniety lewy przycisk myszy
    bool LMB_pressed_;
}