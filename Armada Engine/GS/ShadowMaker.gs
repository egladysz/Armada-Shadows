#version 330 core
layout(triangles) in;
layout (triangle_strip, max_vertices = 10) out;

uniform mat4 worldToView;
uniform mat4 viewToProjection;

uniform vec4 lightPos;
uniform float shadRad;


vec4 lightTransformPos = (worldToView*lightPos);

out float shadowAlpha;

vec4 stretchShadow(vec4 point){
	vec4 distance = (point-lightTransformPos);
	distance = normalize(distance)*shadRad;
	vec4 extend = point+distance;
	return viewToProjection*extend;
}

void emitPair(vec4 point) {
	shadowAlpha = 0.0f;
	gl_Position = viewToProjection*point;
	EmitVertex();

	shadowAlpha = 1.f;
	vec4 dir = stretchShadow(point);
	gl_Position = dir;
	EmitVertex();
}

void project_out(vec4 pointA, vec4 pointB, vec4 pointC){
	emitPair(pointA);
	emitPair(pointB);
	emitPair(pointC);
}

void main() {


	//interior (original) triangle
	shadowAlpha = 0.0f;
	gl_Position = viewToProjection*gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = viewToProjection*gl_in[2].gl_Position;
	EmitVertex();

	//triangle walls
	project_out(gl_in[0].gl_Position,
				gl_in[1].gl_Position,
				gl_in[2].gl_Position);

	//Exterior (at infinity) triangle
	shadowAlpha = 1.f;
	gl_Position = stretchShadow(gl_in[0].gl_Position);
	EmitVertex();
	gl_Position = stretchShadow(gl_in[1].gl_Position);
	EmitVertex();
	EndPrimitive();
}