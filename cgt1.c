#include "cg2d.h"

/* estabelece os limites do mundo */
void SetWorld(float xmax, float  xmin, float  ymax, float ymin){
	XWMax=xmax;
	XWMin=xmin;
	YWMax=ymax;
	YWMin=ymin;}

/* cria pontos e objetos no mundo */
point * SetPoint(float x, float y, int color){
	point * p = malloc(sizeof(point));
	p->x = x;
	p->y = y;
	p->color = color;
	
	return p;}

object * CreateObject(int n){
	object * obj = malloc(sizeof(object));
	obj->numbers_of_points=n;
	obj->points=NULL;
	
	point * ObjPoints = malloc(n*sizeof(point));
	obj->points = &ObjPoints[0];

	return obj;}

int SetObject(point * p, object * obj){
	static int i = 0;
	obj->points[i++] = *p;
	
	return (i-1);} 

/* sistemas de referencias */
window * CreateWindow(float xmin, float xmax, float ymin, float ymax){
	window * win = malloc(sizeof(window));
	win->xmin = xmin;
	win->xmax = xmax;
	win->ymin = ymin;
	win->ymax = ymax;

	return win;}

point * Sru2Srn(point *p, window *win){
	float x, y;
	float xmin, xmax, ymin, ymax;	
	x = p->x;
	y = p->y;
	xmin = win->xmin;
	xmax = win->xmax;
	ymin = win->ymin;
	ymax = win->ymax;
	
	p->x = (x - xmin)/(xmax - xmin);
	p->y = (y - ymin)/(ymax - ymin);

	return p;}
	
bufferdevice * CreateBuffer(int maxx, int maxy){
	bufferdevice * bufferd = malloc(sizeof(bufferdevice));
	bufferd->MaxX = maxx;
	bufferd->MaxY = maxy;

	return bufferd;}

point * Srn2Srd(point * p, bufferdevice * bufferd){
	float ncx, ncy;
	float ndh, ndv; 
	ncx = p->x;
	ncy = p->y;
	ndh = bufferd->MaxX;
	ndv = bufferd->MaxY;
	
	p->x = floorf(ncx * (ndh - 1));
	p->y = floorf(ncy * (ndv - 1));

	return p;}

/* criar e gerenciar uma paleta de cores */

palette * CreatePalette(int number_of_colors){
	palette * pal = malloc(sizeof(palette));
	p->numbers_of_colors = number_of_colors;
	
	return pal;		 	
}

int SetColor(float, float, float, palette *){
	
}

ColorValues * GetColor(int, palette *){
}

object * ChangeColor(object *, int){
}

int main(){	
	object * obj = CreateObject(3);
	point * p1 = SetPoint(0, 0, 0);
	point * p2 = SetPoint(1, 1, 0);

	printf("%d\n", SetObject(p1, obj));
	printf("%d\n", SetObject(p2, obj));
	
	return 0;}
