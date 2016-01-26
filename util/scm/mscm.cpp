// mscm.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

// #include "stdafx.h"

//#include "numtpl.hpp"
//#include "mstpl_cell_class.hpp"
#include <string>
#include <iostream>
#include "mscm.hpp"
//#include "mstpl_cell_class_inp.hpp"
//#include "mstpl_macro_last.hpp"

#define InitFile "init.scm"

int main(int argc, void* argv[])
{
    std::cout << "-------begin-------" << std::endl;
    scm::Scm<int, std::string> scm(10);
//     scm.start_interactive_shell(InitFile);
    scm.eval_str("(load \"init.scm\")(define a \'hoge)(display a)");
//     scm.eval_str("(define a \'hoge)");
//     scm.eval_str("(display a)");
    std::cout << "-------end-------" << std::endl;
    return 0;
}
