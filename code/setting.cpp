#include "setting.h"

//static Setting set;
Setting* Setting::me = new Setting();

Setting::Setting()
{
    json ljson;
    std::ifstream ifs("setting.json");
    ifs >> ljson;
    ifs.close();
    port = ljson["PORT"];
    dir = ljson["DIR"];
    user = ljson["USER"];
    threadNumber = ljson["THREAD"];
}

Setting::~Setting(){

}

Setting* Setting::getInstance(){
    return me;
}
