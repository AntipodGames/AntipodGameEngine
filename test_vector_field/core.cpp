#include <iostream>
#include "core.hpp"
#include "scripts.hpp"
#include <AGE/vector_field.hpp>
#include <QApplication>
#include <QFrame>
#define WORK_DIR "/home/leni/git/AGE/test_vector_field/"


void Environment::_init(){
    age::vector_field vf(10,10,800,600);

    double x = 0, y = 0;
    for(int i = 0 ; i < vf.size()[0]; i++){
        for(int j = 0; j < vf.size()[1]; j++){
            x = i*vf.get_scale()[0];
            y = j*vf.get_scale()[1];
            age::_property_t p;
            p.emplace("x",DOUBLE_PTR((400-x)/100.));
            p.emplace("y",DOUBLE_PTR((300-y)/100.));
            vf[i][j] = p;
        }
    }
//    add_vector_field(vf);
}

void Core::_init(){
    _engine->_init();
    _engine->load_config_file(std::string(WORK_DIR)+"config.yml");
    _load_all_graphics("graphics.yml");

    scripts::init_test(_engine,_am);
}

int main(int argc,char** argv)
{
    srand(time(NULL));

    QApplication* App = new QApplication(argc,argv);
    QFrame* main_frame = new QFrame();
    main_frame->setWindowTitle("test vector field");
    main_frame->resize(800,600);
    main_frame->show();

    Core* core = new Core(std::string(WORK_DIR)+"config.yml",main_frame);

    core->show();

    return App->exec();
}
