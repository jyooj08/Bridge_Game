#include "global.h"
#include "render.h"
#include "glBufferManager.h"
#include "BridgeMap.h"
#include <vector>
#include <iostream>
using namespace std;





bool zoom_tracking = false;
bool padding = false;
bool tb_tracking = false;
float zoom_y_past = 0.0f;
dvec2 padding_past;

//*************************************

static const uint CIRCLE_N = 50;
static const uint SPHERE_N = 30;
vector<vertex> sphere_vertices;
vector<uint> sphere_indices;
glChunk* vertex_chunk;
glChunk* index_chunk;
myMesh* sphereMesh;
BridgeMap* bridgeMap;

bool wire_mode = false;
uint color_mode = 0;
bool rotate_mode = false;

//*************************************
typedef struct planet {
	float rad=10.0f;
	float dist = 0.0f;
	float rot_P = 1.0f;
	float orb_P = 4.0f;
}planet;

vector<planet> planets;

void generate_sphere() {

	for (uint h = 0; h <= SPHERE_N; h++) {
		float t = ((float)PI / SPHERE_N) * (float)h;
		for (uint k = 0; k <= CIRCLE_N; k++) {
			float pi = (2.0f * (float)PI / CIRCLE_N) * (float)k;
			vec3 norm = { sin(t) * cos(pi),sin(t) * sin(pi),cos(t) };
			sphere_vertices.push_back({ norm, vec3(0), vec2((float)k / CIRCLE_N, 1 - (float)h / SPHERE_N) });
		}
	}

	for (uint h = 0; h < SPHERE_N; h++) {
		for (uint k = 0; k < CIRCLE_N; k++) {
			int o = CIRCLE_N * h + k;
			sphere_indices.push_back(o);
			sphere_indices.push_back(o + CIRCLE_N);
			sphere_indices.push_back(o + CIRCLE_N + 1);
			sphere_indices.push_back(o);
			sphere_indices.push_back(o + CIRCLE_N + 1);
			sphere_indices.push_back(o + 1);
			if (o + CIRCLE_N + 1 >= (SPHERE_N + 1) * (CIRCLE_N + 1)) {
				cout << "sphere index error" << endl;
			}
		}
	}
	my_glBufferInit();

	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * sphere_vertices.size(), &sphere_vertices[0], GL_STATIC_DRAW);

	myMesh* tmp = new myMesh(10, 100);
	sphereMesh = new myMesh(sphere_vertices.size(), sphere_indices.size());
	sphereMesh->vertices = sphere_vertices;
	sphereMesh->indices = sphere_indices;
	sphereMesh->update();
}


//*************************************
void update()
{
	// update projection matrix
	cam.aspect = window_size.x/float(window_size.y);
	cam.projection_matrix = mat4::perspective( cam.fovy, cam.aspect, cam.dnear, cam.dfar );

	// build the model matrix for oscillating scale
	float t = float(glfwGetTime());
	float scale	= 1.0f+float(cos(t*1.5f))*0.05f;
	scale *= 100.0f;
	mat4 model_matrix = mat4::scale( scale, scale, scale );

	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation(program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
	uloc = glGetUniformLocation(program, "projection_matrix");		if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
	uloc = glGetUniformLocation(program, "model_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, model_matrix);
	uloc = glGetUniformLocation(program, "color_mode");				if (uloc > -1) glUniform1ui(uloc, color_mode);

}

class Tmp {

public:
void render(int a)
{
	float t = float(glfwGetTime());
	for (int i = a; i < a+1; i++) {
		float orb_w = 2*PI / planets[i].orb_P;
		float rot_w = 2*PI / planets[i].rot_P;
		mat4 model_matrix = mat4::scale(1.0f) *
			mat4::translate(cam.at);
			//mat4::rotate(vec3(0, 0, 1), orb_w * t) *
			//mat4::translate(planets[i].dist, 0, 0) *
			//mat4::scale(planets[i].rad, planets[i].rad, planets[i].rad) *
			//mat4::rotate(vec3(0, 0, 1), rot_w * t);
		applyModelMatrix(model_matrix);
		sphereMesh->render();
	}

}
};


void reshape( GLFWwindow* window, int width, int height )
{
	// set current viewport in pixels (win_x, win_y, win_width, win_height)
	// viewport: the window area that are affected by rendering 
	window_size = ivec2(width,height);
	glViewport( 0, 0, width, height );
}

void print_help()
{
	printf("\n\n=== U S A G E ===\n\n");
	printf("ESC or q to close window\n");
	printf("Use mouse to resize window\n");
	printf("W : Toggle WireMode\n");
	printf("D : Change Color\n");
	printf("Shift + Left : zooming (or Right Button)\n");
	printf("CTRL  + Left : panning (or Middle Button)\n");
	printf("\n\n=================\n\n");
}

void keyboard( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	if(action==GLFW_PRESS)
	{
		if(key==GLFW_KEY_ESCAPE||key==GLFW_KEY_Q)	glfwSetWindowShouldClose( window, GL_TRUE );
		else if(key==GLFW_KEY_H||key==GLFW_KEY_F1)	print_help();
		else if(key==GLFW_KEY_HOME)					cam=camera();
		else if (key == GLFW_KEY_D) {
			color_mode++;
			color_mode %= 3;
		}
		else if (key == GLFW_KEY_W) {
			wire_mode = !wire_mode;
			if (wire_mode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}
		else if (key == GLFW_KEY_R) {
			rotate_mode = !rotate_mode;
		}

	}
}

void mouse( GLFWwindow* window, int button, int action, int mods )
{
	dvec2 pos; glfwGetCursorPos(window,&pos.x,&pos.y);
	bool is_shift_pressed =
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
	bool is_ctrl_pressed =
		glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS
		|| glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;

	// track ball
	if((button==GLFW_MOUSE_BUTTON_LEFT&&!is_shift_pressed&&is_ctrl_pressed)
		||button==GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS) {
			tb_tracking = true;
			padding_past = pos;
		}
	}
	// zoom
	else if (button == GLFW_MOUSE_BUTTON_RIGHT||
		(button == GLFW_MOUSE_BUTTON_LEFT &&is_shift_pressed)) {
		if (action == GLFW_PRESS) { zoom_tracking = true; zoom_y_past = (float)pos.y; }
	}
	// padding
	else if (button == GLFW_MOUSE_BUTTON_LEFT) {
		padding = true;
		padding_past = pos;
	}

	if (action == GLFW_RELEASE) {
		zoom_tracking = false;
		tb_tracking = false;
		padding = false;
	}
}

void motion( GLFWwindow* window, double x, double y )
{
	float dx = float(x - padding_past.x);
	float dy = float(y - padding_past.y);
	if (tb_tracking) {
		vec3 re = cam.eye - cam.at;
		vec4 e = { re.x,re.y,re.z, 1 };
		float th = dy * 0.001f;
		float pi = dx * 0.001f;
		//e = mat4::rotate(cam.up.cross(re).cross(re).normalize(), pi) * e;
		e = mat4::rotate(-cam.up.cross(re).normalize(), th) * e;

		cam.eye = cam.at + vec3(e.x, e.y, e.z);

		padding_past = dvec2(x, y);
	}
	if (zoom_tracking) {
		float ratio = 0.5f;
		vec3 re = cam.eye - cam.at;
		if (re.length() < 30.0f && zoom_y_past - y < 0) {
			// do nothing eye is too close to the object
		}
		else {
			cam.eye += float(zoom_y_past - y)*re.normalize() * ratio;
			zoom_y_past = (float)y;
		}
	}
	if (padding) {
		vec3 re = cam.eye - cam.at;
		re = re.normalize();
		float ratio = 0.001f * (cam.eye -cam.at).length();
		cam.at	-=  cam.up.cross(re).cross(re).normalize() * dy * ratio;
		cam.eye -=  cam.up.cross(re).cross(re).normalize() * dy * ratio;
		cam.at	-= -cam.up.cross(re).cross(re).cross(re).normalize() * dx * ratio;
		cam.eye -= -cam.up.cross(re).cross(re).cross(re).normalize() * dx * ratio;
		//cam.at -= vec3(float(padding_past.y - y), float(padding_past.x - x), 0) * ratio;
		//cam.eye -= vec3(float(padding_past.y - y), float(padding_past.x - x), 0) * ratio;
		padding_past = dvec2(x, y);
	}
	cam.update();
}

bool user_init()
{
	// log hotkeys
	print_help();

	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	//glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests

	// generate vertex_array vertex_bufer vertex_indices

	generate_sphere();
	bridgeMap = new BridgeMap();

	// create 9 planets
	float rad_list[9] = { 60.0f,10.0f,15.0f,20.0f,30.0f,40.0f,35.0f,43.0f,30.0f };
	float dist_list[9] = { 0.0f,80.0f,110.0f,150.0f,210.0f,300.0f,400.0f,500.0f,600.0f};
	float orb_P_list[9] = { 100.0f, 3.0f, 4.0f, 5.0f, 10.0f, 20.0f, -10.0f, 40.0f,5.0f };
	float rot_P_list[9] = { 100.0f, 3.0f, 2.0f, 3.0f, 10.0f, 5.0f, -3.0f, 1.0f,0.5f };
	for (int i = 0; i < 9; i++) {
		planets.push_back(
			{	rad_list[i],
				dist_list[i],
				rot_P_list[i], 
				orb_P_list[i] }
		);
	}

	// birdge map

	return true;
}

void user_finalize()
{
}

int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }						// user initialization

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// enters rendering/event loop
	Tmp* tClass=new Tmp();
	renderID sphereRenderID =attachRenderFunction(bind(&Tmp::render,tClass,0));
	//detachRenderFunction(sphereRenderID);
	attachRenderFunction(bind(&BridgeMap::render, bridgeMap));
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		update();			// per-frame update
		renderAll();			// per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}
