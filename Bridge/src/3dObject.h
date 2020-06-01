#ifndef _3D_OBJECT_H_
#define _3D_OBJECT_H_

#include "cgmath.h"
#include "myMesh.h"
#include "material.h"



class Basic3dObject {
private:
	vec3 position = vec3(0, 0, 0);
	vec3 origin = vec3(0, 0, 0);
	vec3 xa = vec3(1, 0, 0);
	vec3 ya = vec3(0, 1, 0);
	vec3 za = vec3(0, 0, 1);

	Mesh* mesh = nullptr;
	Material material = _silver;
	bool shadow = true;

public:
	Basic3dObject(Mesh* mesh=nullptr,Material material=_silver, bool shadow = true) {
		this->mesh = mesh;
		this->material = material;
		this->shadow = shadow;
	}
	mat4 getModelMatrix(float padding =0.0f) {
		mat4 model_matrix = mat4::scale(1.0f);
		// rotation
		mat4 O = {
			xa.x,	ya.x,	za.x,	0,
			xa.y,	ya.y,	za.y,	0,
			xa.z,	ya.z,	za.z,	0,
			0,		0,		0,		1,
		};
		O = O.transpose();
		O = O.inverse();
		//model_matrix *= O;
		model_matrix = mat4::scale(vec3(1+padding)) * O;
		return mat4::translate(position)*model_matrix *mat4::translate(-origin) ;
	}
	Mesh* getMesh()		{ return mesh; }
	void setMesh(Mesh* m)	{ mesh = m; }
	Material getMaterial() { return material; }
	void setMaterial(Material m) { material = m; }

	void render(float padding = -0.0f) {
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		applyModelMatrix(getModelMatrix(padding));
		applyMaterial(material);
		mesh->render();
	}

	// 3D object control
	void rotate(vec3 axis, float th,bool at_origin = false) {
		vec4 x, y, z;
		vec4 p = vec4(position, 0);
		mat4 R = mat4::rotate(axis.normalize(), th);
		if (at_origin) {
		}
		else {
			p = R * p;
		}
		x = R * vec4(xa, 0.0f);
		y = R * vec4(ya, 0.0f);
		z = R * vec4(za, 0.0f);
		position = vec3(p.x, p.y, p.z);
		xa = vec3(x.x,x.y,x.z);
		ya = vec3(y.x,y.y,y.z);
		za = vec3(z.x,z.y,z.z);
	}
	void turn(vec3 axis,float th) {
		axis = xa * axis.x + ya * axis.y + za * axis.z;
		rotate(axis, th, true);
	}
	void scale(vec3 s)	{ xa /= s; ya /= s; za /= s; }
	void scale(float s) { xa /= s; ya /= s; za /= s; }
	void translate(vec3 v)		{ position += v; }
	void setPosition(vec3 v)	{ position = v; }
	vec3 getPosition()			{ return position; }
	void setOrigin(vec3 v)		{ origin = v; }

	// easy vec3 use
	void scale		(float x, float y	 , float z = 1) { scale			(vec3(x, y, z)); }
	void translate	(float x, float y = 0, float z = 0) { translate		(vec3(x, y, z)); }
	void setPosition(float x, float y = 0, float z = 0) { setPosition	(vec3(x, y, z)); }
	void setOrigin	(float x, float y	 , float z = 0) { setOrigin		(vec3(x, y, z)); }
	void rotate		(float x, float y	 , float z	  ,float th) { rotate(vec3(x, y, z), th); }
};
#endif // _3D_OBJECT_ 