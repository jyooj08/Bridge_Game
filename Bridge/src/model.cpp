#include "model.h"
#include "cgut2.h"
#include "cgmath.h"
#include "assimp_loader.h"
#include "global.h"

static const char* model_vert_shader_path = "../bin/shaders/model.vert";
static const char* model_frag_shader_path = "../bin/shaders/model.frag";
static const char* mesh_obj = "../bin/mesh/atv/atv.obj";
static const char* mesh_3ds = "../bin/mesh/atv/atv.3ds";

GLuint	model_program = 0;	// ID holder for GPU program
mesh2* model_mesh = nullptr;
camera cam;

bool init_model() {
	model_mesh = load_model(mesh_obj);
	if (model_mesh == nullptr) return false;
	if (!(model_program = cg_create_program(model_vert_shader_path, model_frag_shader_path))) { glfwTerminate(); return false; }	// create and compile shaders/program
	return true;
}

void finalize_model() {
	delete model_mesh;
}

void render_model() {
	glUseProgram(model_program);

	mat4 model_matrix;

	// update uniform variables in vertex/fragment shaders
	GLint uloc;
	uloc = glGetUniformLocation(model_program, "view_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.view_matrix);
	uloc = glGetUniformLocation(model_program, "projection_matrix");	if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, cam.projection_matrix);
	uloc = glGetUniformLocation(model_program, "model_matrix");			if (uloc > -1) glUniformMatrix4fv(uloc, 1, GL_TRUE, model_matrix);

	//glActiveTexture(GL_TEXTURE0);								// select the texture slot to bind

	glBindBuffer(GL_ARRAY_BUFFER, model_mesh->vertex_buffer);
	glBindVertexArray(model_mesh->vertex_array);

	for (size_t k = 0, kn = model_mesh->geometry_list.size(); k < kn; k++) {
		geometry& g = model_mesh->geometry_list[k];
	
		/*if (g.mat->textures.diffuse) {
			glBindTexture(GL_TEXTURE_2D, g.mat->textures.diffuse->id);
			glUniform1i(glGetUniformLocation(model_program, "TEX"), 0);	 // GL_TEXTURE0
			glUniform1i(glGetUniformLocation(model_program, "use_texture"), true);
		}
		else {
			glUniform4fv(glGetUniformLocation(model_program, "diffuse"), 1, (const float*)(&g.mat->diffuse));
			glUniform1i(glGetUniformLocation(model_program, "use_texture"), false);
		}*/
		
		// render vertices: trigger shader programs to process vertex data
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_mesh->index_buffer);
		//glDrawElements(GL_TRIANGLES, g.index_count, GL_UNSIGNED_INT, (GLvoid*)(g.index_start * sizeof(GLuint)));
		
	}
	
	
}
