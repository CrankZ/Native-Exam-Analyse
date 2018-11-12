#include "widget.h"
#include "singleapplication.h"

int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    if (!a.isRunning()) {
        Widget w;
        //传入一个要激活程序的窗口，当多开时会激活已有进程的窗口，且多开失败
        a.mainWindow = &w;
        w.show();

        return a.exec();
    }
    return 0;
}
