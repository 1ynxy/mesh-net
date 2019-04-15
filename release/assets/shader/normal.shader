#ifdef COMP_VERT

layout(location = 0) in vec3 vPositionIn;
layout(location = 1) in vec3 vNormalIn;
layout(location = 2) in vec2 vTexCoordIn;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
	vPosition = vec3(modelMat * vec4(vPositionIn, 1.0f));
	vNormal = normalize(mat3(modelMat) * vNormalIn);
	vTexCoord = vec2(vTexCoordIn.x, 1 - vTexCoordIn.y);
	
	gl_Position = projMat * viewMat * vec4(vPosition, 1.0f);
}

#else

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform sampler2D diffuseSampler;

out vec4 fragColour;

void main() {
	fragColour = vec4(vNormal, 1.0f);
}

#endif