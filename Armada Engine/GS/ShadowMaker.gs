#version 330 core
layout(triangles) in;
layout (triangle_strip, max_vertices = 10) out;

uniform mat4 worldToView;
uniform mat4 viewToProjection;

uniform vec4 lightPos;
uniform float shadRad;


vec4 lightTransformPos = (worldToView*lightPos);

out float shadowAlpha;

void emitCloseVertex(vec4 point){
	shadowAlpha = 0.0f;
	gl_Position = viewToProjection*point;
	EmitVertex();
}

void emitFarVertex(vec4 point){
	shadowAlpha = 1.0f;
	vec4 distance = (point-lightTransformPos);
	distance = normalize(distance)*shadRad;
	vec4 extend = point+distance;
	gl_Position = viewToProjection*extend;
	EmitVertex();
}

void emitPair(vec4 point) {
	emitCloseVertex(point);
	emitFarVertex(point);
}

void emitWalls(vec4 pointA, vec4 pointB, vec4 pointC){
	emitPair(pointA);
	emitPair(pointB);
	emitPair(pointC);
}

void main() {
	//interior (original) triangle
	emitCloseVertex(gl_in[1].gl_Position);
	emitCloseVertex(gl_in[2].gl_Position);
	
	//triangle walls
	emitWalls(	gl_in[0].gl_Position,
				gl_in[1].gl_Position,
				gl_in[2].gl_Position);

	//Exterior (at infinity) triangle
	emitFarVertex(gl_in[0].gl_Position);
	emitFarVertex(gl_in[1].gl_Position);
	EndPrimitive();
}