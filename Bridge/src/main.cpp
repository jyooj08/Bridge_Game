#include "global.h"
#include "render.h"
#include "animator.h"
#include "meshFactory.h"
#include "glBufferManager.h"
#include "BridgeMap.h"
#include "camera.h"
#include "myMesh.h"
#include "callBackManager.h"
#include <vector>
#include <iostream>
using namespace std;


//*************************************
// test objects

myMesh* sphereMesh;
BridgeMap* bridgeMap;
myMesh* directorMesh;

Basic3dObject* room;

material_t	material;
light_t		light;

//*************************************
void update();
int gl_init();
bool user_init();
void user_finalize();


int main( int argc, char* argv[] )
{
	gl_init();
	user_init();
	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events

		// NOTICE : update() and animateAll();
		// object which is ATTACHED on animated object should update position after animateAll() !!!
		animateAll();
		update();			

		renderAll();			// per-frame render
	}

	// normal termination
	user_finalize();
	cg_destroy_window(window);

	return 0;
}

//*************************************
void update()
{
	// update projection matrix
	setUniformVariable("color_mode", color_mode);
	// TODO : implement new function on glBufferManager

	glUniform4fv(glGetUniformLocation(program, "light_position"), 1, light.position);
	glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
	glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);

	// setup material properties
	glUniform4fv(glGetUniformLocation(program, "Ka"), 1, material.ambient);
	glUniform4fv(glGetUniformLocation(program, "Kd"), 1, material.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Ks"), 1, material.specular);
	glUniform1f (glGetUniformLocation(program, "shininess"), material.shininess);

	// setup texture
	//glActiveTexture(GL_TEXTURE0);								// select the texture slot to bind
	//glBindTexture(GL_TEXTURE_2D, pMesh->texture);
	//glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0
	//glUniform1i(glGetUniformLocation(program, "mode"), mode);

	applyCamera(cam);
}

int gl_init() {
	// anti-aliasing setting
	glfwWindowHint(GLFW_SAMPLES, 4);
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	//glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests

	// enable anti-aliasing
	glEnable(GL_MULTISAMPLE);

	// generate vertex_array vertex_bufer vertex_indices
	my_glBufferInit();
	return 1;
}


bool user_init()
{
	// log hotkeys
	print_help();

	sphereMesh = generateSphereMesh();
	bridgeMap = new BridgeMap();

	// generate director mesh
	myMesh* x = generateBoxMesh(vec3(10,0.5,0.5));
	myMesh* y = generateBoxMesh(vec3(0.5,10,0.5));
	myMesh* z = generateBoxMesh(vec3(0.5,0.5,10));
	x->paint(1,0,0); // red
	y->paint(0,1,0); // green
	z->paint(0,0,1); // blue
	directorMesh = mergeMesh(mergeMesh(x, y),z);
	attachRenderFunction([]() {								// render camera director
		mat4 model_matrix = mat4::translate(cam.at) *
			mat4::scale((cam.at - cam.eye).length()/200);
		applyModelMatrix(model_matrix);
		directorMesh->render();});

	// generate 3dObjects *********************

	// room 
	static myMesh* roomMesh = generateBoxMesh(vec3(10));
	roomMesh->paint(1, 0, 0);
	room = new Basic3dObject(roomMesh);
	room->setOrigin(5, 5, 5);
	room->scale(10);

	// ground 
	static Basic3dObject ground(generateBoxMesh(vec3(100,1,100)));
	ground.setOrigin(50, 0, 50);
	ground.scale(10);
	ground.translate(0, -20, 0);
	ground.getMesh()->paint(0, 1, 0);
	
	// light box 
	static Basic3dObject lightBox(generateBoxMesh(vec3(4)));
	lightBox.setOrigin(2, 2, 2);
	lightBox.setPosition(50, 50, 0);
	lightBox.getMesh()->flipNormal();
	lightBox.getMesh()->paint(0, 1, 0);

	// register functions ***********************

	// input callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// render 
	attachRenderFunction(bind(&BridgeMap::render, bridgeMap));	// bridge map
	//attachRenderFunction([]() { room->render(); });				// room
	attachRenderFunction([]() {lightBox.render(); });			// light box
	attachRenderFunction([]() {ground.render(); });			// light box


	// animation 

	//attachAnimator([](double t) {bridgeMap->animate(t); },1); // birdge map animation

	// light box
	attachAnimator([](double t) {
		lightBox.rotate(0, 1, 0, float(t));
		light.position = vec4(lightBox.getPosition(),1);
		light.ambient = abs(cos((float)glfwGetTime()));
	});

	// util ********************

	// frame counter
	attachAnimator([](double t) {
		static int past_frame = frame;
		static double fps = double((frame - past_frame) / t);
		const double a = 0.006f;
		fps = (frame - past_frame) / t * a + fps * (1 - a);
		printf("\r fps : %-4d frame_count : %d", (int)fps,frame);
		past_frame = frame;
	});

	return true;
}

void user_finalize()
{
}
