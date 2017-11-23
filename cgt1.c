#include "cg2d.h"
#define DEFAULT_COLOR 0 

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
	obj->numbers_of_points = n;
	obj->points = NULL;
	
	point * ObjPoints = malloc(n*sizeof(point));
	obj->points = &ObjPoints[0];

	return obj;}

int SetObject(point * p, object * obj){
	static int i = 0; // how many points have been added so far
	obj->points[i++] = *p;
	
	return i; /* returns number of points added so far */} 

/* sistemas de referencias */
window * CreateWindow(float xmin, float xmax, float ymin, float ymax){
	window * win = malloc(sizeof(window));
	win->xmin = xmin;
	win->xmax = xmax;
	win->ymin = ymin;
	win->ymax = ymax;

	return win;}

point * Sru2Srn(point * p, window * win){
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
	pal->numbers_of_colors = number_of_colors;
	
	return pal;		 	
}

int SetColor(float red, float green, float blue, palette * pal){
	static int i = 0; // how many colors have been added so far
	pal->colors[i++] = {red, green, blue};
	
	return i; /* returns how many colors have been added so far */}

ColorValues * GetColor(int color_code, palette * pal){
	return pal->colors[color_code];}

object * ChangeColor(object * obj, int color_code){
	// change the object's color
	return obj;}

// funcoes para conversao matricial e preenchimento de objetos
 
int DrawLine(point * p, point * q, window * w, bufferdevice * bufferd, int color_code){ // not sure about what the int should be
	// either of the points is outside of the window
	if(p->x < w->xmin || p->x > w->xmax){
		exit(EXIT_FAILURE);}
	else if(p->y < w->ymin || p->y > w->ymax){
		exit(EXIT_FAILURE);}
	else if(q->x < w->xmin || q->x > w->xmax){
		exit(EXIT_FAILURE;)}
	else if(q->y < w->ymin || q->y > w->ymax){
		exit(EXIT_FAILURE;)}
	else{
		float deltax = q->x - p->x;
		float deltay = q->y - p->y;
		int m;
		int e;
		int i;
		int j;
		int k;
		if(deltax != 0.0){
			m = deltay/deltax;
			e = m - 0.5;
			i = p->x;
			j = p->y;
			for(k = 1; k <= deltax; k++){
				bufferd[j][i] = color_code;
				while(e >= 0){
					j = j + 1;
					e = e - 1.0;
					}
				i = i + 1;
				e = e + m;}}
		return EXIT_SUCCESS;}	
					
int DrawObject(object * obj, window * w, bufferdevice * bufferd){
	int i;
	int n = obj->numbers_of_points;
	for(i = 0; i < (n - 1); i++){
		DrawLine(obj->points[i], obj->points[i + 1], w, bufferd, DEFAULT_COLOR);}
	return EXIT_SUCCESS;}
				
int Fill(object * obj, window * w, bufferdevice * bufferd, int color_code){ // not sure about what the int should be
	int i, j, k;
	point * line_start, line_end;
	DrawObject(obj, w, bufferd);
	for(i = 0; i < w->xmax; i++){
		j = 0;
		while(j++ < w->ymax){
			if(bufferd->[i][j] == DEFAULT_COLOR){ // where the object starts 
				line_start->x = i;
				line_start->y = j;
				line_end->x = i;
				while(bufferd->[i][++j] != DEFAULT_COLOR){
				}
				line_end->y = j;}}
		DrawLine(line_start, line_end, w, bufferd, color_code);}
	
	return EXIT_SUCCESS;}
			
// operacoes com objetos no mundo 
object * Rotate(object * obj, float theta){
	int i;
	float x, y, x_new, y_new;
	for(i = 0; i < obj->numbers_of_points; i++){
		x = obj->points[i].x;
		y = obj->points[i].y;
		x_new = x * cos(theta) - y * sin(theta);
		y_new = y * cos(theta) + x * sin(theta);
		obj->points[i].x = x_new;
		obj->points[i].y = y_new;}
	return EXIT_SUCCESS;} 
	
object * Translate(object * obj, float dx, float dy){
	int i;
	float x;
	float y;
	for(i = 0; i < obj->numbers_of_points; i++){
		x = obj->points[i].x;
		y = obj->points[i].y;
		obj->points[i].x = x + dx;
		obj-points[i].y = y + dy;}
	return EXIT_SUCCESS;}
	
object * Scale(object *, float, float){

hpoint * LinearTransf(hmatrix *, hmatrix *){

hmatrix * ComposeMatrix(hmatrix *, hmatrix *){

hmatrix * SetRotMatrix(float){

hmatrix * SetSclMatrix(float, float){

hmatrix * SetSftMatrix(float, float){



int main(){	
	object * obj = CreateObject(3);
	point * p1 = SetPoint(0, 0, 0);
	point * p2 = SetPoint(1, 1, 0);

	printf("%d\n", SetObject(p1, obj));
	printf("%d\n", SetObject(p2, obj));
	
	return 0;}
