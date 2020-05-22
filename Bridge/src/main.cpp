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
bool user_init();
void user_finalize();


int main( int argc, char* argv[] )
{
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!user_init()){ printf( "Failed to user_init()\n" ); glfwTerminate(); return 1; }						// user initialization


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
	// light position udpate
	light.position = vec4(cam.at, 1);
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
	glUniform1f(glGetUniformLocation(program, "shininess"), material.shininess);

	// setup texture
	//glActiveTexture(GL_TEXTURE0);								// select the texture slot to bind
	//glBindTexture(GL_TEXTURE_2D, pMesh->texture);
	//glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0
	//glUniform1i(glGetUniformLocation(program, "mode"), mode);

	applyCamera(cam);
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
	my_glBufferInit();
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

	// generate room mesh & object
	static myMesh* roomMesh = generateBoxMesh(vec3(10));

	roomMesh->paint(1, 0, 0);
	room = new Basic3dObject(roomMesh);
	room->setOrigin(5, 5, 5);
	room->scale(10);

	// register event callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// register render functions
	attachRenderFunction(bind(&BridgeMap::render, bridgeMap));	// bridge map
	attachRenderFunction([]() { room->render(); });				// room


	// register animation 
	//attachAnimator([](double t) {bridgeMap->animate(t); },1); // birdge map animation
	attachAnimator([](double t) {		// frame counter
		static int past_frame = frame;
		static double fps = double((frame - past_frame) / t);
		const double a = 0.006f;
		fps = (frame - past_frame) / t * a + fps * (1 - a);
		printf("\r fps : %-4d frame_count : %d", (int)fps,frame);
		past_frame = frame;
	});

	// light setting
	light.position = vec4(1,0,0,1);

	return true;
}

void user_finalize()
{
}
