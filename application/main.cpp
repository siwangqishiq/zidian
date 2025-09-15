#include <iostream>
#include "zidian.h"

int main(int argc, char *argv[]){
    zidian::AppParams param;
    param.name = "sand_box_game";

    zidian::SandBox sandBox;
    sandBox.init(param);
    return sandBox.runLoop(argc, argv);
}