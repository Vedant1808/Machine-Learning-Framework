#include<GLFW/glfw3.h>
#include<stdio.h>
void draw_point(double x,double y,int window_width,int window_height)
{
	double _x,_y;
	_x=(1.0*x)/(window_width/2);
	_y=(1.0*y)/(window_height/2);
	glBegin(GL_POINTS);
	glVertex2f(_x,_y);
	glEnd();
}

void draw_line(double x1,double y1,double x2,double y2,int window_width,int window_height)
{
	double _x1,_y1,_x2,_y2;
	_x1=(1.0*x1)/(window_width/2.0);
	_y1=(1.0*y1)/(window_height/2.0);
	_x2=(1.0*x2)/(window_width/2.0);
	_y2=(1.0*y2)/(window_height/2.0);
	glBegin(GL_LINES);
	glVertex2f(_x1,_y1);
	glVertex2f(_x2,_y2);
	glEnd();
}

void draw_graph(int window_width,int window_height)
{
	FILE *f;
	double x,y;
	f=fopen("graph.data","r");
	if(f==NULL) return;
	glPointSize(2);
	glColor3f(1.0f,0.0f,0.0f);
	while(1)
	{
		fscanf(f,"%lf",&x);
		if(feof(f)) break;
		fgetc(f);  // to consume, that is after the iteration number
	        fscanf(f,"%lf",&y);
		draw_point(x*3.0,y*3.0,window_width,window_height);
	}
	fclose(f);
}

void draw_graph_paper(int window_width,int window_height)
{
	glClearColor(1.0f,1.0f,1.0f,0);
	glColor3f(0.5f,0.5f,0.5f);
	draw_line(0-window_width/2.0,0.0,window_width/2.0,0,window_width,window_height);
	draw_line(0,0-window_height/2.0,0,window_height/2,window_width,window_height);
}

void update_view(int window_width,int window_height)
{
	draw_graph_paper(window_width,window_height);
	draw_graph(window_width,window_height);
}

int main()
{
	GLFWwindow *window;
	GLFWmonitor *monitor;
	const GLFWvidmode *video_mode;
	int desktop_width,desktop_height;
	int window_position_x,window_position_y;
	int window_width,window_height;
	// initialize library
	if(!glfwInit())
	{
		printf("Unable to initialize GUI mode\n");
		return 0;
	}
	monitor=glfwGetPrimaryMonitor();
	video_mode=glfwGetVideoMode(monitor);
	desktop_height=video_mode->height;
	desktop_width=video_mode->width;
	window_width=1080;
	window_height=720;
	window_position_x=(desktop_width/2)-(window_width/2);
	window_position_y=(desktop_height/2)-(window_height/2);
	// create window
	window=glfwCreateWindow(window_width,window_height,"My Window",NULL,NULL);
	if(!window)
	{
		printf("Unable to create window\n");
		glfwTerminate();
		return 0;
	}
	glfwSetWindowPos(window,window_position_x,window_position_y);
	glfwMakeContextCurrent(window);
	while(!glfwWindowShouldClose(window))
	{
	     glClear(GL_COLOR_BUFFER_BIT);
	     update_view(window_width,window_height);
	     glfwSwapBuffers(window);
	     glfwPollEvents();  // process events ds
	}
	glfwDestroyWindow(window);    // release DS related to window
	glfwTerminate();
	return 0;
}
