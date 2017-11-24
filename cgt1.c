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
	palette * pal; // = (palette*) malloc(sizeof(int) + sizeof(ColorValues) * number_of_colors);
	pal->colors = (ColorValues*) malloc(sizeof(ColorValues) * number_of_colors);
	pal->numbers_of_colors = number_of_colors;
	
	return pal;}

int SetColor(float red, float green, float blue, palette * pal){
	static int i = 0; // how many colors have been added so far
	if(i < pal->numbers_of_colors){
		pal->colors[i++] = (struct Color) {red, green, blue};}
	return i;} /* returns how many colors have been added so far */

ColorValues * GetColor(int color_code, palette * pal){
	ColorValues * c;
	c = &pal->colors[color_code];
	return c;}

object * ChangeColor(object * obj, int color_code){
	int i;
	point * p;
	point * q;
	
	for(i = 0; i < obj->numbers_of_points; i++){
		p = &obj->points[i];
		q = SetPoint(p->x, p->y, color_code);
		obj->points[i] = *q;}
	return obj;}

// funcoes para conversao matricial e preenchimento de objetos
 
// checar apostila
int DrawLine(point * p, point * q, window * w, bufferdevice * bufferd, int color_code){ // not sure about what the int should be
	// either of the points is outside of the window
	// mudar p/ desenhar o que esta dentro da janela mesmo q parte da linha n esteja 
	if(p->x < w->xmin || p->x > w->xmax){
		exit(EXIT_FAILURE);}
	else if(p->y < w->ymin || p->y > w->ymax){
		exit(EXIT_FAILURE);}
	else if(q->x < w->xmin || q->x > w->xmax){
		exit(EXIT_FAILURE);}
	else if(q->y < w->ymin || q->y > w->ymax){
		exit(EXIT_FAILURE);}
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
				bufferd->buffer[j][i] = color_code;
				while(e >= 0){
					j = j + 1;
					e = e - 1.0;
					}
				i = i + 1;
				e = e + m;}}}
		return EXIT_SUCCESS;}	

// checar apostila					
int DrawObject(object * obj, window * w, bufferdevice * bufferd){
	int i;
	int n = obj->numbers_of_points;
	for(i = 0; i < (n - 1); i++){
		DrawLine(&obj->points[i], &obj->points[i + 1], w, bufferd, DEFAULT_COLOR);}
	return EXIT_SUCCESS;}

// checar apostila				
int Fill(object * obj, window * w, bufferdevice * bufferd, int color_code){ // not sure about what the int should be
	int i, j, k;
	point line_start, line_end;
	DrawObject(obj, w, bufferd);
	for(i = 0; i < w->xmax; i++){
		j = 0;
		while(j++ < w->ymax){
			if(bufferd->buffer[i][j] == DEFAULT_COLOR){ // where the object starts 
				line_start.x = i;
				line_start.y = j;
				line_end.x = i;
				while(bufferd->buffer[i][++j] != DEFAULT_COLOR){
				}
				line_end.y = j;}}
		DrawLine(&line_start, &line_end, w, bufferd, color_code);}
	
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
	return obj;} 
	
object * Translate(object * obj, float dx, float dy){
	int i;
	float x;
	float y;
	for(i = 0; i < obj->numbers_of_points; i++){
		x = obj->points[i].x;
		y = obj->points[i].y;
		obj->points[i].x = x + dx;
		obj->points[i].y = y + dy;}
	return obj;}
	
object * Scale(object * obj, float sx, float sy){
	int i;
	float x;
	float y;
	for(i = 0; i < obj->numbers_of_points; i++){
		x = obj->points[i].x;
		y = obj->points[i].y;
		obj->points[i].x = x * sx;
		obj->points[i].y = x * sy;}
	return obj;}
	
hpoint * LinearTransf(hmatrix * m, hpoint * p){
	float x = p->x;
	float y = p->y;
	float w = p->w;
	p->x = m->a11 * x + m->a12 * y + m->a13 * w;
	p->y = m->a21 * x + m->a22 * y + m->a23 * w;
	p->w = m->a31 * x + m->a32 * y + m->a33 * w;
	
	return p;}

hmatrix * ComposeMatrix(hmatrix * m, hmatrix * n){
	hmatrix * A;
	
	float a11, a12, a13;
	float a21, a22, a23;
	float a31, a32, a33;
	
	float b11, b12, b13;
	float b21, b22, b23;
	float b31, b32, b33;

	a11 = m->a11;
	a12 = m->a12;
	a13 = m->a13;	
	
	a21 = m->a21;
	a22 = m->a22;
	a23 = m->a23;
	
	a31 = m->a31;
	a32 = m->a32;
	a33 = m->a33;

	b11 = n->a11;
	b12 = n->a12;
	b13 = n->a13;

	b21 = n->a21;
	b22 = n->a22;
	b23 = n->a23;

	b31 = n->a31;
	b32 = n->a32;
	b33 = n->a33;
	
	A->a11 = a11 * b11 + a12 * b21 + a13 * b31;
	A->a12 = a11 * b12 + a12 * b22 + a13 * b32;
	A->a13 = a11 * b13 + a12 * b23 + a13 * b33;

	A->a21 = a21 * b11 + a22 * b21 + a23 * b31;
	A->a22 = a21 * b12 + a22 * b22 + a23 * b32;
	A->a23 = a21 * b13 + a22 * b23 + a23 * b33;

	A->a31 = a31 * b11 + a32 * b21 + a33 * b31;
	A->a32 = a31 * b12 + a32 * b22 + a33 * b32;
	A->a33 = a31 * b13 + a32 * b23 + a33 * b33;
	
	return A;}

hmatrix * SetRotMatrix(float theta){
	hmatrix * A;
	A->a11 = cos(theta);
	A->a12 = -sin(theta);
	A->a21 = sin(theta);
	A->a22 = cos(theta);
	A->a31 = 0;
	A->a32 = 0;
	A->a33 = 1;
	A->a13 = 0;
	A->a23 = 0;

	return A;}

hmatrix * SetSclMatrix(float Sx, float Sy){
	hmatrix * A;
	A->a11 = Sx;
	A->a12 = 0;
	A->a13 = 0;
	
	A->a21 = 0;
	A->a22 = Sy;
	A->a23 = 0;

	A->a31 = 0;
	A->a32 = 0;
	A->a33 = 1;

	return A;}

// translacao
hmatrix * SetSftMatrix(float Dx, float Dy){
	hmatrix * A;
	
	A->a11 = 1;
	A->a12 = 0;
	A->a13 = Dx;

	A->a21 = 0;
	A->a22 = 1;
	A->a23 = Dy;

	A->a31 = 0;
	A->a32 = 0;
	A->a33 = 1;

	return A;}


int main(){	
	point * p1 = SetPoint(0, 0, 0);
	point * p2 = SetPoint(1, 1, 0);
	
	SetWorld(10, -20, 15, -20);
	
	window * w = CreateWindow(-8, 5, -7, 5);
	bufferdevice * bufferd = CreateBuffer(640, 480);
	
	palette * pal;
	pal = CreatePalette(1);
	SetColor(0, 0, 0, pal);
	
	// teste
	DrawLine(p1, p2, w, bufferd, 0); // "segmentation fault: 11" here
	
	free(p1);
	free(p2);
	free(w);
	free(bufferd);
	free(pal->colors);
	return 0;}
