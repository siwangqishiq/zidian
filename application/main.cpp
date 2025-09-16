#include <iostream>
#include "zidian.h"

int main(int argc, char *argv[]){
    zidian::AppParams param;
    param.name = "sand_box_game";

    zidian::SandBox sandBox;

    zidian::Log::i("app", "before init.");
    sandBox.init(param);

    zidian::Log::w("app", "before run loop");
    return sandBox.runLoop(argc, argv);
}