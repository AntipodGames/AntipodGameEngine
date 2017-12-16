#include "core.hpp"
#include "scripts.hpp"
#include <iostream>
#include <QApplication>
#include <QFrame>
#define WORK_DIR "/home/leni/git/AGE/test/test_1/"

void Core::_init(){

    _engine->_init();

    _engine->load_config_file(std::string(WORK_DIR)+"config.yml");

    _load_all_graphics("graphics.yml");

//    _load_level("level_test.yml");

    scripts::init_test(_engine->get_entities(),_am);

    std::cout<< "init !!!" << std::endl;
}

void Core::_update(){}

int main(int argc, char** argv){
    srand(time(NULL));

    QApplication* App = new QApplication(argc,argv);
    QFrame* main_frame = new QFrame();
    main_frame->setWindowTitle("test 1");
    main_frame->resize(800,600);
    main_frame->show();

    Core* core = new Core(std::string(WORK_DIR)+"config.yml",main_frame);

    core->show();

    return App->exec();


    return 0;
}
