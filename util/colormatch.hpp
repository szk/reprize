#ifndef COLORMATCH_HPP_
#define COLORMATCH_HPP_

namespace reprize
{
class Colormatch
{
public:
    Colormatch(void) {}
    virtual ~Colormatch(void) {}
protected:
    Vec3 result[6];
};
}
#endif

#ifdef
var hs=new Object();
var rg=new Object();
rg.r=rg.g=rg.b=0;

function setc(c)
{
    f=document.calculator;
    color=eval("f.t"+c).value;
    f.tPrim.value=color;
    calculate(f);
}
function init()
{
    calculate(document.forms["calculator"]);
}
function html2rg(html)
{
    // pass it a html color string like '#001122'
    rg.r = parseInt ( html.substring (1,3),16);
    rg.g = parseInt ( html.substring (3,5),16);
    rg.b = parseInt ( html.substring (5,7),16);
    // alert("r:"+rg.r+" g:)"+rg.g+" b:"+rg.b)

}
function update(whichPos, rg)
{
    f=document.calculator
    // alert("setting "+whichPos+" to "+rg2html(rg))
    eval("f.t"+whichPos).value=rg2html(rg);
    elem = eval("document.getElementById('c"+whichPos+"')");
    if(elem) { elem.style.backgroundColor=rg2html(rg); }
    else { alert(whichPos); }

}
function calculate(f)
{
    prim = f.tPrim.value;
    html2rg(prim);
    rg2hs(rg);
    update("Prim", rg);
    update("1", rg);
    z=new Object();
    y=new Object();
    yx=new Object();
    y.s=hs.s;
    y.h=hs.h;
    if(hs.v>70){ y.v=hs.v-30 }
    else{ y.v=hs.v+30 };
    z=h2r(y);
    update("2",z);
    if((hs.h>=0)&&(hs.h<30)){ 
        yx.h=y.h=hs.h+20;
        yx.s=y.s=hs.s;
        y.v=hs.v;
        if(hs.v>70){ yx.v=hs.v-30 }
        else{ yx.v=hs.v+30 }
    }
    if((hs.h>=30)&&(hs.h<60)){ 
        yx.h=y.h=hs.h+150;
        y.s=rc(hs.s-30,100);
        y.v=rc(hs.v-20,100);
        yx.s=rc(hs.s-70,100);
        yx.v=rc(hs.v+20,100);
    }
    if((hs.h>=60)&&(hs.h<180)){ 
        yx.h=y.h=hs.h-40;
        y.s=yx.s=hs.s;
        y.v=hs.v;
        if(hs.v>70){ yx.v=hs.v-30 }
        else{ yx.v=hs.v+30 }
    }
    if((hs.h>=180)&&(hs.h<220)){
        yx.h=hs.h-170;
        y.h=hs.h-160;
        yx.s=y.s=hs.s;
        y.v=hs.v;
        if(hs.v>70){ yx.v=hs.v-30 }
        else{ yx.v=hs.v+30 }
    }
    if((hs.h>=220)&&(hs.h<300)){
        yx.h=y.h=hs.h;
        yx.s=y.s=rc(hs.s-60,100);
        y.v=hs.v;
        if(hs.v>70){ yx.v=hs.v-30 }
        else{ yx.v=hs.v+30 }
    }
    if(hs.h>=300){
        if(hs.s>50){ y.s=yx.s=hs.s-40 }
        else { y.s=yx.s=hs.s+40 }
        yx.h=y.h=(hs.h+20)%360;
        y.v=hs.v;
        if(hs.v>70){ yx.v=hs.v-30 }
        else{ yx.v=hs.v+30 }
    }
    z=h2r(y);
    update("3",z);
    z=h2r(yx);
    update("4",z);
    y.h=0;
    y.s=0;
    y.v=100-hs.v;
    z=h2r(y);
    update("5",z);
    y.h=0;
    y.s=0;
    y.v=hs.v;
    z=h2r(y);
    update("6",z);
}
#endif
