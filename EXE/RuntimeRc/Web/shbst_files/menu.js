function openScript(url, width, height){
    window.open(url,"_blank",'width=' + width + ',height=' + height + ',resizable=1,scrollbars=yes,menubar=no,status=yes' );
}
tPopWait=50;
tPopShow=5000;
showPopStep=20;
popOpacity=99;
sPop=null;
curShow=null;
tFadeOut=null;
tFadeIn=null;
tFadeWaiting=null;
document.write("<style type='text/css'id='defaultPopStyle'>");
document.write(".cPopText {  background-color: #F8F8F5;color:#000000; border: 1px #000000 solid;font-color: font-size: 12px; padding-right: 4px; padding-left: 4px; height: 20px; padding-top: 2px; padding-bottom: 2px; filter: Alpha(Opacity=0)}");
document.write("</style>");
//document.write("<div id='dypopLayer' style='position:absolute;z-index:1000;' class='cPopText'></div>");
function showPopupText(){
if (isvisible == true) HideMenu();	
var o=event.srcElement;
MouseX=event.x;
MouseY=event.y;
if(o.alt!=null && o.alt!=""){o.dypop=o.alt;o.alt=""};
if(o.title!=null && o.title!=""){o.dypop=o.title;o.title=""};
if(o.dypop!=sPop) {
sPop=o.dypop;
clearTimeout(curShow);
clearTimeout(tFadeOut);
clearTimeout(tFadeIn);
clearTimeout(tFadeWaiting);
if(sPop==null || sPop=="") {
dypopLayer.innerHTML="";
dypopLayer.style.filter="Alpha()";
dypopLayer.filters.Alpha.opacity=0;
}
else {
if(o.dyclass!=null) popStyle=o.dyclass
else popStyle="cPopText";
curShow=setTimeout("showIt()",tPopWait);
}
}
}
function showIt(){
dypopLayer.className=popStyle;
dypopLayer.innerHTML=sPop;
popWidth=dypopLayer.clientWidth;
popHeight=dypopLayer.clientHeight;
if(MouseX+12+popWidth>document.body.clientWidth) popLeftAdjust=-popWidth-24
	else popLeftAdjust=0;
if(MouseY+12+popHeight>document.body.clientHeight) popTopAdjust=-popHeight-24
	else popTopAdjust=0;
dypopLayer.style.left=MouseX+12+document.body.scrollLeft+popLeftAdjust;
dypopLayer.style.top=MouseY+12+document.body.scrollTop+popTopAdjust;
dypopLayer.style.filter="Alpha(Opacity=0)";
fadeOut();
}
function fadeOut(){
if(dypopLayer.filters.Alpha.opacity<popOpacity) {
dypopLayer.filters.Alpha.opacity+=showPopStep;
tFadeOut=setTimeout("fadeOut()",1);
}
else {
dypopLayer.filters.Alpha.opacity=popOpacity;
tFadeWaiting=setTimeout("fadeIn()",tPopShow);
}
}
function fadeIn(){
if(dypopLayer.filters.Alpha.opacity>0) {
dypopLayer.filters.Alpha.opacity-=1;
tFadeIn=setTimeout("fadeIn()",1);
}
}
document.onmouseover=showPopupText;
function CheckAll(form)  {
for (var i=0;i<form.elements.length;i++)    {
var e = form.elements[i];
if (e.name != 'chkall')       e.checked = form.chkall.checked;
}
}
var h;
var w;
var l;
var t;
var topMar = 1;
var leftMar = -2;
var space = 1;
var isvisible;
var MENU_SHADOW_COLOR='#FFF6D5';
var global = window.document
global.fo_currentMenu = null
global.fo_shadows = new Array
function HideMenu()
{
var mX;
var mY;
var vDiv;
var mDiv;
if (isvisible == true)
{
vDiv = document.all("menuDiv");
mX = window.event.clientX + document.body.scrollLeft;
mY = window.event.clientY + document.body.scrollTop;
if ((mX < parseInt(vDiv.style.left)) || (mX > parseInt(vDiv.style.left)+vDiv.offsetWidth) || (mY < parseInt(vDiv.style.top)-h) || (mY > parseInt(vDiv.style.top)+vDiv.offsetHeight)){
vDiv.style.visibility = "hidden";
isvisible = false;
}
}
}
function ShowMenu(vMnuCode,tWidth) {
vSrc = window.event.srcElement; //onmouseout='HideMenu()'
vMnuCode = "<table id='submenu' cellspacing=1 cellpadding=3 style='width:"+tWidth+"' class=tableborder1 ><tr height=23><td nowrap align=left class=tablebody1>" + vMnuCode + "</td></tr></table>";
h = vSrc.offsetHeight;
w = vSrc.offsetWidth;
l = vSrc.offsetLeft + leftMar+4;
t = vSrc.offsetTop + topMar + h + space-2;
vParent = vSrc.offsetParent;
while (vParent.tagName.toUpperCase() != "BODY")
{
l += vParent.offsetLeft;
t += vParent.offsetTop;
vParent = vParent.offsetParent;
}
menuDiv.innerHTML = vMnuCode;
menuDiv.style.top = t;
menuDiv.style.left = l;
menuDiv.style.visibility = "visible";
isvisible = true;
makeRectangularDropShadow(submenu, MENU_SHADOW_COLOR, 4)
}
function makeRectangularDropShadow(el, color, size)
{
var i;
for (i=size; i>0; i--)
{
var rect = document.createElement('div');
var rs = rect.style
rs.position = 'absolute';
rs.left = (el.style.posLeft + i) + 'px';
rs.top = (el.style.posTop + i) + 'px';
rs.width = el.offsetWidth + 'px';
rs.height = el.offsetHeight + 'px';
rs.zIndex = el.style.zIndex - i;
rs.backgroundColor = color;
var opacity = 1 - i / (i + 1);
rs.filter = 'alpha(opacity=' + (100 * opacity) + ')';
el.insertAdjacentElement('afterEnd', rect);
global.fo_shadows[global.fo_shadows.length] = rect;
}
}

//ͼƬ����������
var flag=false;
function DrawImage(ImgD,iwidth,iheight){
    //����(ͼƬ,����Ŀ��,����ĸ߶�)
     var image=new Image();
     image.src=ImgD.src;
    if(image.width>0 && image.height>0){
     flag=true;
    if(image.width/image.height>= iwidth/iheight){
        if(image.width>iwidth){  
         ImgD.width=iwidth;
         ImgD.height=(image.height*iwidth)/image.width;
         }else{
         ImgD.width=image.width;  
         ImgD.height=image.height;
         }
         }
    else{
        if(image.height>iheight){  
         ImgD.height=iheight;
         ImgD.width=(image.width*iheight)/image.height;        
         }else{
         ImgD.width=image.width;  
         ImgD.height=image.height;
         }
         }
     }
} 

