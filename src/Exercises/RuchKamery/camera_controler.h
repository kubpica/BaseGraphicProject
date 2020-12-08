// Za pomoc� klasy izolujemy spos�b poruszania kamer� od samej kamery
// i jak przyjdzie potrzeba b�dziemy mogli podmieni� kontroler na inny np. na FPC (First Person Camera).
class CameraControler {
public:
    CameraControler() :camera_(nullptr) {}
    CameraControler(Camera* camera) :camera_(camera) {}
    void set_camera(Camera* camera) { camera_ = camera; }

    // odczytali�my ju� zmian� po�o�enia myszy dx i dy dokonujemy obrotu kamery za pomoc� metody
    void rotate_camera(float dx, float dy) {
        camera_->rotate_around_center(-scale_ * dy, camera_->x());
        camera_->rotate_around_center(-scale_ * dx, camera_->y());
    }

    // obs�uga ruchu myszy
    void mouse_moved(float x, float y) {
        if (LMB_pressed_) {
            auto dx = x - x_;
            auto dy = y - y_;
            x_ = x;
            y_ = y;

            rotate_camera(dx, dy);
        }
    };

    // metody obs�ugi zdarzenia naci�ni�cia i puszczenia przycisku myszy
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

    // Przelicznik pomi�dzy pikselami a k�tem. 
    // Jego warto�� nale�y dobra� do�wiadczalnie dla danej rozdzielczo�ci ekranu.
    float scale_ = 1;

    // chcemy, aby obracanie dzia�a�o tylko jak ruszamy mysz� trzymaj�c wci�niety lewy przycisk myszy
    bool LMB_pressed_;
}