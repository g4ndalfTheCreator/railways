#include <iostream>
#include "ui.h"
#include "datastructures.hh"


using namespace std;

int main(){
    Ui main_ui;

    Datastructures main_datastrucuture;

    main_ui.ui_begin(main_datastrucuture);

    return 0;
}
