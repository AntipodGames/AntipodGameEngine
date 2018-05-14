#include <iostream>
#include <QApplication>
#include <QFrame>
#include "core.h"


#define WORK_DIR "/home/leni/git/AGE/test/test_control/"

void Core::_init(){
        _engine->_init();
        _engine->load_config_file(std::string(WORK_DIR)+"config/config.yml");
        _load_all_graphics("config/graphics.yml");
        _load_level("config/level_test.yml");
        Player::Ptr player(new Player((System*)this));

        TO_STRING(player->access_property()["name"])->value = "gustave_1";
        TO_STRING(player->access_property()["sprite"])->value = "sprite_1";
        TO_STRING(player->access_property()["state"])->value = "idle";
        TO_DOUBLE(player->access_property()["x"])->value = 400;
        TO_DOUBLE(player->access_property()["y"])->value = 300;
        TO_INTEGER(player->access_property()["width"])->value = 160;
        TO_INTEGER(player->access_property()["height"])->value = 100;
        TO_DOUBLE(player->access_property()["velocity"])->value = 3;

        _engine->add_controller("gustave_1",player);
        _link_sprites_to_entities();
//        TO_DOUBLE(_engine->get_controllers()["gustave_1"]->access_property()["x"])->value = 400;
//        TO_DOUBLE(_engine->get_controllers()["gustave_1"]->access_property()["y"])->value = 300;

}

void Core::_update(){


}

int main(int argc, char** argv){
    srand(time(NULL));

    QApplication* App = new QApplication(argc,argv);
    QFrame* main_frame = new QFrame();
    main_frame->setWindowTitle("test control");
    main_frame->resize(800,600);
    main_frame->show();

    Core* core = new Core(std::string(WORK_DIR)+"config/config.yml",main_frame);

    core->show();

    return App->exec();
}
