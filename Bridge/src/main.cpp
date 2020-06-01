#include "global.h"
#include "render.h"
#include "animator.h"
#include "meshFactory.h"
#include "glBufferManager.h"
#include "BridgeMap.h"
#include "camera.h"
#include "myMesh.h"
#include "callBackManager.h"
#include "material.h"
#include "sound.h"
#include "text.h"
#include "model.h"
#include "logic.h"
#include <vector>
#include <iostream>
using namespace std;
#pragma comment(lib, "irrKlang/irrKlang.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//*************************************
// test objects

Mesh* sphereMesh;
BridgeMap* bridgeMap;
Mesh* directorMesh;

Basic3dObject* room;
Basic3dObject box;
Basic3dObject lightBall(generateSphereMesh());

light_t		light; // should be global!!

//*************************************
bool game_start = false;
int stage = 0;
string maps[3] = {
	"UURRRUU",
	"UUUURRUUULLLLUUUUR",
	"UUUURRRUULLUURRUUUURRRDDDRRRUU"
};
double time_delta[3] = { 1.0,0.6,0.4 };
renderID mapID;

//*************************************
void depth_map_fbo_setting();
void update();
int gl_init();
bool user_init();
void user_finalize();

int main( int argc, char* argv[] )
{
	gl_init();
	user_init();
	//start_game();
	// enters rendering/event loop
	for( frame=0; !glfwWindowShouldClose(window); frame++ )
	{
		glfwPollEvents();	// polling and processing of events
		if (!game_start) {
			if (stage == 0) printf("Press Enter to start game\n");
			else if (stage == -1) printf("Game Over\n");
			else if (stage >= 3) printf("Congratulation!!!\n");
			else printf("Stage %d\n", stage + 1);
			continue;
		}
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

	// light
	glUniform4fv(glGetUniformLocation(program, "light_position"), 1, light.position);
	glUniform4fv(glGetUniformLocation(program, "Ia"), 1, light.ambient);
	glUniform4fv(glGetUniformLocation(program, "Id"), 1, light.diffuse);
	glUniform4fv(glGetUniformLocation(program, "Is"), 1, light.specular);

	// material
	// applyMaterial(material);
	// shadow mapping

	// setup texture
	//glActiveTexture(GL_TEXTURE0);								// select the texture slot to bind
	//glBindTexture(GL_TEXTURE_2D, pMesh->texture);
	//glUniform1i(glGetUniformLocation(program, "TEX"), 0);	 // GL_TEXTURE0
	//glUniform1i(glGetUniformLocation(program, "mode"), mode);

	applyCamera(global_cam);
}
bool game_finalize() {
	detachRenderFunction(mapID);
	delete bridgeMap;
	return true;
}

bool game_init() {
	printf("start game init\n");
	game_finalize();
	init_logic(maps[stage], time_delta[stage]);

	global_cam = camera();

	bridgeMap = new BridgeMap(maps[stage]);
	mapID = attachRenderFunction(bind(&BridgeMap::render, bridgeMap));

	box = generateBoxMesh(vec3(9));
	box.setMaterial(_ruby);
	put2render(&box);

	// light ball
	lightBall.scale(10);
	lightBall.setPosition(0, 200, 200);
	lightBall.getMesh()->flipNormal();

	start_game();
	printf("end game init\n");
	return true;
}

int gl_init() {
	// anti-aliasing setting
	glfwWindowHint(GLFW_SAMPLES, 4);
	// create window and initialize OpenGL extensions
	if(!(window = cg_create_window( window_name, window_size.x, window_size.y ))){ glfwTerminate(); return 1; }
	if(!cg_init_extensions( window )){ glfwTerminate(); return 1; }	// version and extensions

	// input setting
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	else cout << "WARNING : no RawMouseInputSupport" << endl;

	// initializations and validations
	if(!(program=cg_create_program( vert_shader_path, frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	if(!(shadow_program=cg_create_program( shadow_vert_shader_path, shadow_frag_shader_path ))){ glfwTerminate(); return 1; }	// create and compile shaders/program
	// init GL states
	glClearColor( 39/255.0f, 40/255.0f, 34/255.0f, 1.0f );	// set clear color
	glEnable( GL_CULL_FACE );								// turn on backface culling
	glEnable( GL_DEPTH_TEST );								// turn on depth tests
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// enable anti-aliasing
	glEnable(GL_MULTISAMPLE);

	// shadow depth map setting
	depth_map_fbo_setting();

	// generate vertex_array vertex_bufer vertex_indices
	my_glBufferInit();
	return 1;
}


bool user_init()
{
	// log hotkeys
	print_help();

	game_init();

	// generate 3dObjects *********************
	sphereMesh = generateSphereMesh();
	//bridgeMap = new BridgeMap();

	// generate director mesh
	Mesh* x = generateBoxMesh(vec3(10,0.5,0.5));
	Mesh* y = generateBoxMesh(vec3(0.5,10,0.5));
	Mesh* z = generateBoxMesh(vec3(0.5,0.5,10));
	directorMesh = mergeMesh(mergeMesh(x, y),z);
	attachRenderFunction([]() {								// render camera director
		mat4 model_matrix = mat4::translate(global_cam.at) *
			mat4::scale((global_cam.at - global_cam.eye).length()/200);
		applyModelMatrix(model_matrix);
		directorMesh->render();});

	
	// ground 
	static Basic3dObject ground(generateBoxMesh(vec3(100,2,100)));
	ground.setOrigin(50, 1, 50);
	ground.setPosition(0, -10, 0);
	ground.scale(10);

	// register functions ***********************

	// input callbacks
	glfwSetWindowSizeCallback( window, reshape );	// callback for window resizing events
    glfwSetKeyCallback( window, keyboard );			// callback for keyboard events
	glfwSetMouseButtonCallback( window, mouse );	// callback for mouse click inputs
	glfwSetCursorPosCallback( window, motion );		// callback for mouse movement

	// render 
	//attachRenderFunction(bind(&BridgeMap::render, bridgeMap));	// bridge map
	//renderID lightBallRenderID = attachRenderFunction([]() { lightBall.render(); });			// light box
	//disableLightBlocking(lightBallRenderID);
	renderID lightBallID = put2render(&lightBall);
	disableLightBlocking(lightBallID);
	disableLightEffect(lightBallID);
	put2render(&ground);
	//put2render(&box);

	// animation 

	// light ball movement
	attachAnimator([](double t) {
		light.position = vec4(lightBall.getPosition(),1);
	});
	// box movement

	attachAnimator([](double t) {
		//box.rotate(vec3(0,1,0), float(t),true);
		//box.rotate(0,1,0, float(t));
		//ball.translate(vec3(0, 10, 0) );
	});

	// util ********************

	// frame counter
	/*
	attachAnimator([](double t) {
		static int past_frame = frame;
		static double fps = double((frame - past_frame) / t);
		const double a = 0.006f;
		fps = (frame - past_frame) / t * a + fps * (1 - a);
		printf("\r fps : %-4d frame_count : %d", (int)fps,frame);
		past_frame = frame;
	});
	*/

	if (!init_sound()) return false;
	//play_sound();

	if (!init_text()) return false;

	if (!init_model()) return false;

	return true;
}

void user_finalize()
{
	finalize_sound();
	finalize_model();
}

void depth_map_fbo_setting() {
	// source : learnopengl.com
	// configure depth map FBO
	// -----------------------
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
	//	SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// shader configuration
	// --------------------
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "shadowMap"), 0);
}
